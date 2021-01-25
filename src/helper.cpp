#pragma once

#include "helper.h"

void Helper::Read(std::ifstream* fs, unsigned int offset, unsigned long long size, void* ptr)
{
	fs->seekg(offset);
	fs->read((char*)ptr, size);
}

int Helper::LeafHash()
{
	return 0;
}

std::wstring Helper::printDate(unsigned long long filetime)
{
	// function that returns a string with the date from a windows FILETIME format
	std::map<int, int> monthkey;
	monthkey.insert(std::make_pair(1, 31));
	monthkey.insert(std::make_pair(2, 28));
	monthkey.insert(std::make_pair(3, 31));
	monthkey.insert(std::make_pair(4, 30));
	monthkey.insert(std::make_pair(5, 31));
	monthkey.insert(std::make_pair(6, 30));
	monthkey.insert(std::make_pair(7, 31));
	monthkey.insert(std::make_pair(8, 31));
	monthkey.insert(std::make_pair(9, 30));
	monthkey.insert(std::make_pair(10, 31));
	monthkey.insert(std::make_pair(11, 30));
	monthkey.insert(std::make_pair(12, 31));

	std::wstring outputTime;

	unsigned short year = 1601;
	short date = 1;
	unsigned char month = 0;

	// 315569520000000 is the number of hundred nanoseconds in 1 year where a year is 365.2425 days (1.26.... is 4* that number)
	while (filetime >= 315360000000000)
	{
		if ((year%4==0 && year%100!=0) || year%400==0)
		{
			if (filetime == 315360000000000)
			{
				break;
			}
			filetime -= 8.64e11;
		}
			
		year += 1;
		filetime -= 315360000000000;
	}

	date += (unsigned short)(filetime / 8.64e11); // populates with date of year (1-366)
	filetime %= 864000000000;

	// go through the really sad inefficient process of doing the month
	while (date > 0)
	{
		// subtract the relevant months worth of days (29 if its febuary in a leap year else from the monthkey map)
		month += 1;
		date -= (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) && (month == 2))?29:monthkey[month];
	}
	// undo the last run that took the date into negative
	date += (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) && (month == 2)) ? 29 : monthkey[month];

	outputTime = std::to_wstring(date) + L"-" + std::to_wstring(month) + L"-" + std::to_wstring(year);
	return outputTime;
}

std::wstring Helper::CharToWstring(const char* string, size_t len)
{
	// converts an ascii (1 byte) char array to a utf-8 (2 bytes) array
	wchar_t* w_array = new wchar_t[len+1];

	for (int i = 0; i < len; i++)
	{
		w_array[i] = string[i];
		((char*)w_array)[i * 2 + 1] = '\0';
	}
	w_array[len] = '\0\0';

	std::wstring wstring((wchar_t*)w_array);
	return wstring;
}
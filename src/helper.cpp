#pragma once

#include "helper.h"

void Helper::Read(std::istream* fs, unsigned int offset, unsigned long long size, void* ptr)
{
	fs->seekg(offset);
	fs->read((char*)ptr, size);
}

int Helper::LeafHash()
{
	return 0;
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
#pragma once
#include <fstream>
#include <codecvt>
#include <locale>
#include <string>
#include <cmath>
#include <map>

class Helper
{
public:
	static void Read(std::ifstream* fs, unsigned int offset, unsigned long long size, void* ptr);
	static int LeafHash();
	static std::wstring CharToWstring(const char* string, size_t len);
	static std::wstring printDate(unsigned long long filetime);
};
#pragma once
#include <fstream>
#include <codecvt>
#include <locale>
#include <string>
#include <pybind11/pybind11.h>

struct TIME
{
	unsigned int lowDateTime;
	unsigned int highDateTime;
};

class Helper
{
public:
	static void Read(std::istream* fs, unsigned int offset, unsigned long long size, void* ptr);
	static int LeafHash();
	static std::wstring CharToWstring(const char* string, size_t len);
};
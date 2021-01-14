#pragma once
#include <fstream>

struct FILETIME
{
	unsigned int lowDateTime;
	unsigned int highDateTime;
};

class Helper
{
public:
	static void Read(std::istream* fs, unsigned int offset, unsigned long long size, void* ptr);
	static int LeafHash();
};
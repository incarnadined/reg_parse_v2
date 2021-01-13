#pragma once
#include <fstream>

class Helper
{
public:
	static void Read(std::istream* fs, unsigned int offset, unsigned long long size, void* ptr);
};
#pragma once
#include <fstream>
#include <iostream>

#include "NK.h"

class Hive
{
public:
	Hive(const char* filepath);
	~Hive();


	// auto here should be replaced with a data type that contains the value data (struct)
	auto GetFileData();

	auto GetValue(char* keypath, char* valuename);
	auto GetValues(char* keypath);

	auto ListSubkeys(char* keypath);

private:
	int m_magic_bytes;
	unsigned int m_root_cell_offset;
};


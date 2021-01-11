#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <Windows.h>

#include "NK.h"

class Hive
{
public:
	Hive(const char* filepath);
	~Hive();


	// auto here should be replaced with a data type that contains the value data (struct)
	auto GetFileData();

	int GetValue(std::string keypath, char* valuename);
	int GetValues(std::string keypath);

	void GetSubkeys(std::string keypath);

private:
	std::shared_ptr<NK> ProcessSubkeys(std::string keypath);

	std::ifstream fs;
	std::shared_ptr<NK> m_root;

	int m_magic_bytes;
	unsigned int m_primary_sequence_number;
	unsigned int m_secondary_sequence_number;
	FILETIME m_last_written;
	unsigned int m_major_version_number;
	unsigned int m_minor_version_number;
	unsigned int m_root_cell_offset;

	LPSYSTEMTIME m_timestamp;
};


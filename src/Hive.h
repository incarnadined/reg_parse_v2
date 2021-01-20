#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <tuple>
#include <string>

#include "NK.h"

class Hive
{
public:
	Hive(const char* filepath);
	~Hive();

	std::wstring GetVersion();
	unsigned int GetMajor();
	unsigned int GetMinor();

	// auto here should be replaced with a data type that contains the value data (struct)
	auto GetFileData();

	std::shared_ptr<VK> GetValue(std::wstring keypath, std::wstring valuename);
	std::vector<std::shared_ptr<VK>> GetValues(std::wstring keypath);

	std::vector<std::shared_ptr<NK>> GetSubkeys(std::wstring keypath);

private:
	std::shared_ptr<NK> ProcessSubkeys(std::wstring keypath);

	std::ifstream fs;
	std::shared_ptr<NK> m_root;

	int m_magic_bytes;
	unsigned int m_primary_sequence_number;
	unsigned int m_secondary_sequence_number;
	long long m_last_written;
	unsigned int m_major_version_number;
	unsigned int m_minor_version_number;
	unsigned int m_root_cell_offset;
};


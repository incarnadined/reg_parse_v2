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

	VK* GetValue(std::wstring keypath, std::wstring valuename);
	std::vector<VK*> GetValues(std::wstring keypath);
	std::vector<VK*> GetValues(NK* key);

	NK* GetKey(NK* parent, std::wstring keypath);
	NK* GetKey(std::wstring keypath);
	std::vector<NK*> GetSubkeys(NK* parent);
	std::vector<NK*> GetSubkeys(std::wstring keypath);
	NK* GetRoot();

private:
	NK* ProcessSubkeys(std::wstring keypath);
	NK* ProcessSubkeys(NK* parent, std::wstring relativeKeyPath);

	std::ifstream fs;
	NK* m_root;

	int m_magic_bytes;
	unsigned int m_primary_sequence_number;
	unsigned int m_secondary_sequence_number;
	long long m_last_written;
	unsigned int m_major_version_number;
	unsigned int m_minor_version_number;
	unsigned int m_root_cell_offset;
};


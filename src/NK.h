#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include "list.h"
#include "VK.h"

class NK
{
public:
	NK(std::istream* fs, unsigned int offset);
	~NK();

	std::shared_ptr<NK> Tunnel(const char* keyname);
	void ProcessValues();

	std::vector<std::shared_ptr<NK>> subkeys;
	std::vector<std::shared_ptr<VK>> values;
	char* m_name;

private:
	std::istream* m_fs;
	unsigned int m_offset;

	int m_size;
	unsigned short m_flags;
	long long m_last_write;
	unsigned int m_parent_offset;
	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned short m_name_length;
};

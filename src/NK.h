#pragma once
#include <fstream>
#include <vector>
#include <memory>

#include "list.h"

class NK
{
public:
	NK(std::ifstream* fs, unsigned int offset);
	~NK();

	std::shared_ptr<NK> Tunnel(const char* keyname);

	std::vector<std::shared_ptr<NK>> subkeys;
	char* m_name;

private:
	std::ifstream* m_fs;
	unsigned long long m_offset;

	int m_size;
	unsigned short m_flags;
	unsigned int m_parent_offset;
	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned short m_name_length;
};


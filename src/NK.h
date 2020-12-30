#pragma once
#include <fstream>
#include <vector>

class NK
{
public:
	NK(std::ifstream* fs, unsigned int offset);
	~NK();

private:
	NK* Tunnel(char* keyname);

	unsigned long long m_offset;
	std::vector<NK> subkeys;

	int m_size;
	unsigned short m_flags;
	unsigned int m_parent_offset;
	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned short m_name_length;
	char m_name[];
};


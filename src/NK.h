#pragma once
#include <fstream>

class NK
{
public:
	NK(std::ifstream* fs, unsigned int offset);
	~NK();

private:
	unsigned int m_offset;

	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned int m_parent_offset;
	unsigned short m_name_length;
	char m_name[];
};


#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include "list.h"

class VK
{
public:
	VK(std::ifstream* fs, unsigned int offset);
	~VK();

	char* m_name;

private:
	std::ifstream* m_fs;
	unsigned long long m_offset;
	bool m_resident;

	int m_size;
	unsigned short m_name_length;
	unsigned int m_data_length;
	unsigned int m_data;
	unsigned int m_type;
	unsigned short m_flags;
};


#pragma once

#include <fstream>

#include "includes.h"

class DataNode
{
public:
	DataNode(std::istream* fs, unsigned int offset);
	~DataNode();

	unsigned int* pointers;
	unsigned char* raw_data;

private:
	int m_size;
};


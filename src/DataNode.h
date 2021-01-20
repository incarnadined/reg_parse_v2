#pragma once

#include <fstream>

#include "includes.h"

class DataNode
{
public:
	DataNode();
	DataNode(std::istream* fs, unsigned int offset);
	~DataNode();

	unsigned int* pointers = new unsigned int;
	unsigned char* raw_data = new unsigned char;

private:
	int m_size;
};


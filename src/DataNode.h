#pragma once

#include <fstream>
#include <memory>

#include "includes.h"

class DataNode
{
public:
	DataNode();
	DataNode(std::ifstream* fs, unsigned int offset);
	~DataNode();

	unsigned int* pointers = new unsigned int;
	unsigned char* raw_data = new unsigned char;

private:
	int m_size;
};


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

	unsigned int* pointers = nullptr;
	unsigned char* raw_data = nullptr;

private:
	int m_size;
};


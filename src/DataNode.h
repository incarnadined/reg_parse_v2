#pragma once

#include <fstream>

#include "includes.h"

class DataNode
{
public:
	DataNode(std::istream* fs, unsigned int offset);
	~DataNode();

	unsigned int* pointers;

private:
	int m_size;
};


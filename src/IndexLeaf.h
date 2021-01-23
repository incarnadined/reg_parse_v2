#pragma once

#include <fstream>

#include "helper.h"

class IndexLeaf
{
public:
	IndexLeaf(std::ifstream* fs, unsigned int offset);
	~IndexLeaf();

	unsigned int* pointers;

	short m_entries_count;

private:
	unsigned int m_size;
	short m_signature;
};
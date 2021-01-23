#pragma once

#include <fstream>

#include "helper.h"

class FastLeaf
{
public:
	FastLeaf(std::ifstream* fs, unsigned int offset);
	~FastLeaf();

	unsigned int* pointers;
	unsigned int* hints;

	short m_entries_count;

private:
	unsigned int m_size;
	short m_signature;
};

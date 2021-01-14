#pragma once

#include <fstream>

#include "helper.h"

class HashLeaf
{
public:
	HashLeaf(std::istream* fs, unsigned int offset);
	~HashLeaf();

	unsigned int* pointers;
	int* hashes;

	short m_entries_count;

private:
	unsigned int m_size;
	short m_signature;
};

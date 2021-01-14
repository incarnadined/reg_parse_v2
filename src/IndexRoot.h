#pragma once

#include <fstream>
#include <vector>

#include "helper.h"
#include "FastLeaf.h"
#include "HashLeaf.h"
#include "IndexLeaf.h"

class IndexRoot
{
public:
	IndexRoot(std::istream* fs, unsigned int offset);
	~IndexRoot();

	std::vector<unsigned int> pointersv;
	unsigned int* pointers;

private:
	unsigned int m_size;
	short m_signature;
	short m_entries_count;
};
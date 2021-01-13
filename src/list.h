#pragma once
#include <fstream>
#include <vector>

#include "helper.h"

struct record
{
	// if hash is -1 then the list type doesn't have hashes
	unsigned int offset;
	int hash;
};

class list
{
	// NOTE: all list classes should be temporary to find data and should NOT exceed the scope of one function
public:
	list(std::istream* fs, unsigned int offset);
	~list();

	std::vector<record> records;

	char* data;

private:

	int m_size;
	short m_signature;
	short m_entries_count;
};
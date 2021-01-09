#pragma once
#include <fstream>
#include <vector>

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
	list(std::ifstream* fs, unsigned long long offset);
	~list();

	std::vector<record> records;

private:

	int m_size;
	short m_signature;
	short m_entries_count;
};
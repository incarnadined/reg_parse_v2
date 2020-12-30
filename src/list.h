#pragma once
#include <fstream>
#include <vector>

struct record
{
	unsigned int offset;
	int hash;
};

class list
{
	// NOTE: all list classes should be temporary to find data and should NOT exceed the scope of one function
public:
	list(std::ifstream* fs, unsigned long long offset);
	~list();

private:
	std::vector<record> records;

	int m_size;
	short m_signature;
	short m_entries_count;
};


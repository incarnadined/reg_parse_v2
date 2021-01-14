#include "IndexLeaf.h"

IndexLeaf::IndexLeaf(std::istream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);
	Helper::Read(fs, 0x1000 + offset + 0x4, sizeof(short), &m_signature);
	Helper::Read(fs, 0x1000 + offset + 0x6, sizeof(short), &m_entries_count);

	// if the signature is not 'li' (26988) something has gone wrong
	if (m_signature != 26988)
	{
		throw;
	}

	pointers = new unsigned int[m_entries_count];
	for (int i = 0; i < m_entries_count; i++)
	{
		Helper::Read(fs, 0x1000 + offset + (0x8 + i * 4), sizeof(int), &pointers[i]);
	}
}

IndexLeaf::~IndexLeaf()
{
	delete[] pointers;
}

#include "FastLeaf.h"

FastLeaf::FastLeaf(std::istream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);
	Helper::Read(fs, 0x1000 + offset + 0x4, sizeof(short), &m_signature);
	Helper::Read(fs, 0x1000 + offset + 0x6, sizeof(short), &m_entries_count);

	// if the signature is not 'lf' (26732) something has gone wrong
	if (m_signature != 26732)
	{
		throw;
	}

	// read the pointers (4 bytes) and name hints (first 4 chars of name) for each key
	pointers = new unsigned int[m_entries_count];
	hints = new unsigned int[m_entries_count];
	for (int i = 0; i < m_entries_count; i++)
	{
		Helper::Read(fs, 0x1000 + offset + 0x8 + (i * 8), sizeof(int), &pointers[i]);
		Helper::Read(fs, 0x1000 + offset + 0xC + (i * 8), sizeof(int), &hints[i]);
	}
}

FastLeaf::~FastLeaf()
{
	delete[] pointers;
	delete[] hints;
}

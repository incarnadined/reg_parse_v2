#include "IndexRoot.h"

IndexRoot::IndexRoot(std::ifstream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);
	Helper::Read(fs, 0x1000 + offset + 0x4, sizeof(short), &m_signature);
	Helper::Read(fs, 0x1000 + offset + 0x6, sizeof(short), &m_entries_count);

	// if the signature is not 'ri' (26994) something has gone wrong
	if (m_signature != 26994)
	{
		throw;
	}

	unsigned int ptr;
	short signature;
	for (int i = 0; i < m_entries_count; i++)
	{
		Helper::Read(fs, 0x1000 + offset + (0x8 + i * 4), sizeof(int), &ptr);

		Helper::Read(fs, 0x1000 + ptr + 0x4, sizeof(short), &signature);

		if (signature == 26732)
		{
			HashLeaf* templist = new HashLeaf(fs, ptr);
			for (int j = 0; j < templist->m_entries_count; j++)
			{
				pointersv.push_back(templist->pointers[j]);
			}
		}
		else if (signature == 26988)
		{
			IndexLeaf* templist = new IndexLeaf(fs, ptr);
			for (int j = 0; j < templist->m_entries_count; j++)
			{
				pointersv.push_back(templist->pointers[j]);
			}
			break;
		}
	}

	// convert pointersv to an array so it can be treated the same as the other lists
	pointers = new unsigned int[pointersv.size()];
	for (int i = 0; i < pointersv.size(); i++)
	{
		pointers[i] = (unsigned int)pointersv[i];
	}
}

IndexRoot::~IndexRoot()
{
}

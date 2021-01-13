#include "list.h"

list::list(std::istream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);
	Helper::Read(fs, 0x1000 + offset + 0x4, sizeof(short), &m_signature);
	Helper::Read(fs, 0x1000 + offset + 0x6, sizeof(short), &m_entries_count);


	// if the signature shows an lf (26220) or lh (26732) list type, the records strucutre is
	// 4 bytes unsigned int offset
	// 4 bytes int hash
	// the two types differ on their hashing algorithms
	if (m_signature == 26732 || m_signature == 26220)
	{
		for (int i = 0; i < m_entries_count; i++)
		{
			record record_instance;

			Helper::Read(fs, 0x1000 + offset + 0x8 + (unsigned int)sizeof(record) * i, sizeof(record), &record_instance);

			records.push_back(record_instance);
		}
	}
	/*else // no recognised signature means a data node - m_signature and m_entries_count are garbage now
	{
		// structure of a data node is (not including 4 byte size)
		// 4 bytes pointer or a whole bunch of data (for values)

		// create a length var (m_size is negative), subtract 4 bytes for the size int, add one for a null byte
		int length = abs(m_size) - 4 + 1;

		// seek back to the just after the end of the size
		fs->seekg(0x1000 + offset + 0x4);

		data = new char[length];
		fs->read((char*)data, sizeof(length-1));

		// add a null byte to the end incase there isn't one already (not a string)
		data[length-1] = '\0';

		// in case this points to a value list, populate the records
		for (int i = 0; i < (length - 1) / 4; i++)
		{
			record record_instace;

			// get the ith int from data
			record_instace.offset = *((int*)data + i);

			record_instace.hash = 0xFFFFFFFF;

			records.push_back(record_instace);
		}
	}*/
}

list::~list()
{
	delete[] data;
}

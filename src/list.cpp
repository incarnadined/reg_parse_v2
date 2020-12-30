#include "list.h"

list::list(std::ifstream* fs, unsigned long long offset)
{
	fs->seekg(0x1000 + offset);
	fs->read((char*)m_size, sizeof(int));
	fs->read((char*)m_signature, sizeof(short));
	fs->read((char*)m_entries_count, sizeof(short));


	// if the signature shows an lf (26220) or lh (26732) list type, the records strucutre is
	// 4 bytes unsigned int offset
	// 4 bytes int hash
	// the two types differ on their hashing algorithms
	if (m_signature == 26732 || m_signature == 26220)
	{

	}
}

list::~list()
{
}

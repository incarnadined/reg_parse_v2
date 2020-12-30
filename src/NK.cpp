#include "NK.h"

NK::NK(std::ifstream* fs, unsigned int offset) : m_offset(offset)
{
	fs->seekg(0x1000 + m_offset); // all offsets are relative to the first hbin (located at 0x1000)

	fs->read((char*)&m_size, sizeof(int));
	fs->seekg(0x1000 + m_offset + 0x06);
	fs->read((char*)&m_flags, sizeof(unsigned short));
	fs->seekg(0x1000 + m_offset + 0x14);
	fs->read((char*)&m_parent_offset, sizeof(unsigned int));
	fs->read((char*)&m_subkey_count, sizeof(unsigned int));
	fs->seekg(0x1000 + m_offset + 0x20);
	fs->read((char*)&m_subkey_offset, sizeof(unsigned int));
	fs->seekg(0x1000 + m_offset + 0x28);
	fs->read((char*)&m_value_count, sizeof(unsigned int));
	fs->read((char*)&m_value_offset, sizeof(unsigned int));
	fs->seekg(0x1000 + m_offset + 0x4C);
	fs->read((char*)&m_name_length, sizeof(unsigned short));
	fs->seekg(0x1000 + m_offset + 0x50);
	fs->read((char*)&m_name, m_name_length);
}

NK::~NK()
{
}

NK* NK::Tunnel(char* keyname)
{
	// finds the next route for the tunnel leading from this key
	// if this key this function has already been ran for this key, returns the entry from the vector
	// returns a pointer to the subkey with the given name
	// returns -1 if the key was not found

	if (m_subkey_count == 0)
	{
		// if the key has no subkeys then this key cannot exist
		return (NK*)-1;
	}

	// if the subkeys vector is already populated, return a pointer to the relevant key
	if (!subkeys.size())
	{
		for (int i = 0; i < subkeys.size(); i++)
		{
			if (subkeys[i].m_name == keyname)
			{
				return &subkeys[i];
			}
		}
		return (NK*)-1;
	}

	// if you get this far, there are subkeys yet to be found, so find them
	// make the vector big enough to store all of the subkeys
	subkeys.reserve(m_subkey_count);

	return (NK*)-1;
}

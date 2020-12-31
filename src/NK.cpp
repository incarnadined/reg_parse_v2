#include "NK.h"

NK::NK(std::ifstream* fs, unsigned int offset) : m_offset(offset), m_fs(fs)
{
	m_fs->seekg(0x1000 + m_offset); // all offsets are relative to the first hbin (located at 0x1000)

	m_fs->read((char*)&m_size, sizeof(int));
	m_fs->seekg(0x1000 + m_offset + 0x06);
	m_fs->read((char*)&m_flags, sizeof(unsigned short));
	m_fs->seekg(0x1000 + m_offset + 0x14);
	m_fs->read((char*)&m_parent_offset, sizeof(unsigned int));
	m_fs->read((char*)&m_subkey_count, sizeof(unsigned int));
	m_fs->seekg(0x1000 + m_offset + 0x20);
	m_fs->read((char*)&m_subkey_offset, sizeof(unsigned int));
	m_fs->seekg(0x1000 + m_offset + 0x28);
	m_fs->read((char*)&m_value_count, sizeof(unsigned int));
	m_fs->read((char*)&m_value_offset, sizeof(unsigned int));
	m_fs->seekg(0x1000 + m_offset + 0x4C);
	m_fs->read((char*)&m_name_length, sizeof(unsigned short));
	m_fs->seekg(0x1000 + m_offset + 0x50);
	m_fs->read((char*)&m_name, m_name_length);
}

NK::~NK()
{
}

std::shared_ptr<NK> NK::Tunnel(char* keyname)
{
	// finds the next route for the tunnel leading from this key
	// if this key this function has already been ran for this key, returns the entry from the vector
	// returns a pointer to the subkey with the given name
	// returns -1 if the key was not found

	if (m_subkey_count == 0)
	{
		// if the key has no subkeys then this key cannot exist
		return std::shared_ptr<NK>();
	}

	// if the subkeys vector is already populated, return a pointer to the relevant key
	if (!subkeys.size())
	{
		for (int i = 0; i < subkeys.size(); i++)
		{
			if (subkeys[i]->m_name == keyname)
			{
				return subkeys[i];
			}
		}
		return std::shared_ptr<NK>();
	}

	// if you get this far, there are subkeys yet to be found, so find them
	// make the vector big enough to store all of the subkeys
	subkeys.reserve(m_subkey_count);

	std::shared_ptr<NK> return_key;
	list* list_instance = new list(m_fs, m_subkey_offset);
	for (int i = 0; i < m_subkey_count; i++)
	{
		std::shared_ptr<NK> temp_subkey = std::make_shared<NK>(m_fs, list_instance->records[i].offset);
		subkeys.push_back(temp_subkey);

		if (temp_subkey->m_name == keyname)
		{
			return_key = temp_subkey;
		}
	}

	return return_key;


	return std::shared_ptr<NK>();
}
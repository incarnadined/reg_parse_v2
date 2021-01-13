#include "NK.h"

NK::NK(std::istream* fs, unsigned int offset) : m_offset(offset), m_fs(fs)
{
	Helper::Read(m_fs, 0x1000 + m_offset + 0x00, sizeof(int), &m_size);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x08, sizeof(long long), &m_last_write);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x06, sizeof(unsigned short), &m_flags);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x14, sizeof(int), &m_parent_offset);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x18, sizeof(int), &m_subkey_count);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x20, sizeof(int), &m_subkey_offset);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x28, sizeof(int), &m_value_count);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x2C, sizeof(int), &m_value_offset);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x4C, sizeof(int), &m_name_length);

	m_name = new char[(long long)m_name_length + 1];
	Helper::Read(m_fs, 0x1000 + m_offset + 0x50, m_name_length, m_name);
	m_name[m_name_length] = '\0';
}

NK::~NK()
{
	delete[] m_name;
}

std::shared_ptr<NK> NK::Tunnel(const char* keyname)
{
	// finds the next subkey from this key
	// if this key this function has already been ran for this key, returns the entry from the vector
	// returns a pointer to the subkey with the given name
	// returns -1 if the key was not found

	// if the key has no subkeys then this key cannot exist
	if (m_subkey_count == 0)
	{
		return std::shared_ptr<NK>();
	}

	// if the subkeys vector is already populated, return a pointer to the relevant key
	if (subkeys.size() != 0)
	{
		for (int i = 0; i < subkeys.size(); i++)
		{
			if (!strcmp(subkeys[i]->m_name, keyname))
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
	for (unsigned int i = 0; i < m_subkey_count; i++)
	{
		std::shared_ptr<NK> temp_subkey = std::make_shared<NK>(m_fs, list_instance->records[i].offset);
		subkeys.push_back(temp_subkey);

		if (!strcmp(temp_subkey->m_name, keyname))
		{
			return_key = temp_subkey;
		}
	}
	delete list_instance;
	return return_key;


	return std::shared_ptr<NK>();
}

void NK::ProcessValues()
{
	// populate the values vector with the values of this key

	// if the values have already been found or there aren't any, stop here
	if (m_value_count == 0 || values.size() != 0)
		return;

	// make the vector big enough to store all of the values
	values.reserve(m_value_count);

	list* list_instance = new list(m_fs, m_value_offset);
	for (unsigned int i = 0; i < m_value_count; i++)
	{
		std::shared_ptr<VK> temp_value = std::make_shared<VK>(m_fs, list_instance->records[i].offset);
		values.push_back(temp_value);
	}
	delete list_instance;
}

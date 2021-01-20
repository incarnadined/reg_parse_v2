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

	// if the name is ascii
	if (m_flags & Flags::KEY_COMP_NAME)
	{
		char* temp_name = new char[(long long)m_name_length+1];

		Helper::Read(m_fs, 0x1000 + m_offset + 0x50, m_name_length, temp_name);
		temp_name[m_name_length] = '\0';

		m_name = Helper::CharToWstring(temp_name, m_name_length);

		delete[] temp_name;
	}
	// else it must be unicode
	else
	{
		Helper::Read(m_fs, 0x1000 + m_offset + 0x50, m_name_length, &m_name);
	}
}

NK::~NK()
{
}

std::shared_ptr<NK> NK::Tunnel(std::wstring keyname)
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
			if (!wcscmp(subkeys[i]->GetName().c_str(), keyname.c_str()))
			{
				return subkeys[i];
			}
		}
		return std::shared_ptr<NK>();
	}

	// if you get this far, there are subkeys yet to be found, so find them
	// make the vector big enough to store all of the subkeys
	subkeys.reserve(m_subkey_count);

	// create fake var ready for deletion later
	int* templist = new int;
	short temp_signature;
	unsigned int* pointers = new unsigned int[0];
	Helper::Read(m_fs, 0x1000 + m_subkey_offset + 0x4, sizeof(short), &temp_signature);
	if (temp_signature == 26220)
	{
		FastLeaf* templist = new FastLeaf(m_fs, m_subkey_offset);
		pointers = templist->pointers;
	}
	else if (temp_signature == 26732)
	{
		HashLeaf * templist = new HashLeaf(m_fs, m_subkey_offset);
		pointers = templist->pointers;
	}
	else if (temp_signature == 26994)
	{
		IndexRoot* templist = new IndexRoot(m_fs, m_subkey_offset);
		pointers = templist->pointers;
	}

	std::shared_ptr<NK> return_key;
	for (unsigned int i = 0; i < m_subkey_count; i++)
	{
		std::shared_ptr<NK> temp_subkey = std::make_shared<NK>(m_fs, pointers[i]);
		subkeys.push_back(temp_subkey);

		if (!wcscmp(temp_subkey->GetName().c_str(), keyname.c_str()))
		{
			return_key = temp_subkey;
		}
	}

	delete templist;

	return return_key;
}

void NK::ProcessValues()
{
	// populate the values vector with the values of this key

	// if the values have already been found or there aren't any, stop here
	if (m_value_count == 0 || values.size() != 0)
		return;

	// make the vector big enough to store all of the values
	values.reserve(m_value_count);

	DataNode* data_node = new DataNode(m_fs, m_value_offset);
	for (unsigned int i = 0; i < m_value_count; i++)
	{
		std::shared_ptr<VK> temp_value = std::make_shared<VK>(m_fs, data_node->pointers[i]);
		values.push_back(temp_value);
	}
	delete data_node;
}

std::wstring NK::GetName()
{
	return m_name;
}

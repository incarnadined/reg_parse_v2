#include "NK.h"


NK::NK(std::ifstream* fs, unsigned int offset) : m_offset(offset), m_fs(fs)
{
	Helper::Read(m_fs, 0x1000 + m_offset + 0x00, sizeof(int), &m_size);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x08, sizeof(unsigned long long), &m_last_write);
	Helper::getDateTime(m_last_write);
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
	for (int i = 0; i < subkeys.size(); i++)
	{
		delete subkeys[i];
	}

	for (int i = 0; i < values.size(); i++)
	{
		delete values[i];
	}
}

NK* NK::Tunnel(std::wstring keyname)
{
	// finds the next subkey from this key
	// if this key this function has already been ran for this key, returns the entry from the vector
	// returns a pointer to the subkey with the given name
	// returns -1 if the key was not found

	// if the key has no subkeys then this key cannot exist
	if (m_subkey_count == 0)
	{
		return nullptr;
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
		return nullptr;
	}

	// if you get this far, there are subkeys yet to be found, so find them
	// make the vector big enough to store all of the subkeys
	subkeys.reserve(m_subkey_count);

	NK* return_key = nullptr;
	short temp_signature;
	Helper::Read(m_fs, 0x1000 + m_subkey_offset + 0x4, sizeof(short), &temp_signature);
	if (temp_signature == 26220)
	{
		FastLeaf* templist = new FastLeaf(m_fs, m_subkey_offset);
		unsigned int* pointers = templist->pointers;

		for (unsigned int i = 0; i < m_subkey_count; i++)
		{
			NK* temp_subkey = new NK(m_fs, pointers[i]);
			temp_subkey->SetParent(this);
			subkeys.push_back(temp_subkey);

			if (!wcscmp(temp_subkey->GetName().c_str(), keyname.c_str()))
			{
				return_key = temp_subkey;
			}
		}

		delete templist;

	}
	else if (temp_signature == 26732)
	{
		HashLeaf * templist = new HashLeaf(m_fs, m_subkey_offset);
		unsigned int* pointers = templist->pointers;

		for (unsigned int i = 0; i < m_subkey_count; i++)
		{
			NK* temp_subkey = new NK(m_fs, pointers[i]);
			temp_subkey->SetParent(this);
			subkeys.push_back(temp_subkey);

			if (!wcscmp(temp_subkey->GetName().c_str(), keyname.c_str()))
			{
				return_key = temp_subkey;
			}
		}

		delete templist;

	}
	else if (temp_signature == 26994)
	{
		IndexRoot* templist = new IndexRoot(m_fs, m_subkey_offset);
		unsigned int* pointers = templist->pointers;

		for (unsigned int i = 0; i < m_subkey_count; i++)
		{
			NK* temp_subkey = new NK(m_fs, pointers[i]);
			temp_subkey->SetParent(this);
			subkeys.push_back(temp_subkey);

			if (!wcscmp(temp_subkey->GetName().c_str(), keyname.c_str()))
			{
				return_key = temp_subkey;
			}
		}

		delete templist;

	}

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
		VK* temp_value = new VK(m_fs, data_node->pointers[i]);
		values.push_back(temp_value);
	}
	delete data_node;
}

std::wstring NK::GetName()
{
	return m_name;
}

void NK::SetParent(NK* parent_key)
{
	m_parent = parent_key;
}

NK* NK::GetParent()
{
	return m_parent;
}

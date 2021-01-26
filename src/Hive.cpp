#include "Hive.h"

Hive::Hive(const char* filepath) : m_magic_bytes(0), m_root_cell_offset(0)
{
	fs.open(filepath, std::ifstream::in | std::ifstream::binary);

	Helper::Read(&fs, 0x00, sizeof(int), &m_magic_bytes);
	if (m_magic_bytes != 1718052210)
	{
		//ERROR!!! file magic bytes are not regf
		throw;
	}

	Helper::Read(&fs, 0x04, sizeof(unsigned int), &m_primary_sequence_number);
	Helper::Read(&fs, 0x08, sizeof(unsigned int), &m_secondary_sequence_number);
	if (m_primary_sequence_number != m_secondary_sequence_number)
	{
		// the hive is dirty and should be cleaned with transaction logs
		//throw;
	}

	// read the last written timestamp of the file
	Helper::Read(&fs, 0x0C, sizeof(long long), &m_last_written);
	Helper::getDateTime(m_last_written);

	// read the major and minor version numbers of the hive
	Helper::Read(&fs, 0x14, sizeof(unsigned int), &m_major_version_number);
	Helper::Read(&fs, 0x18, sizeof(unsigned int), &m_minor_version_number);

	Helper::Read(&fs, 0x24, sizeof(unsigned int), &m_root_cell_offset);
	m_root = new NK(&fs, m_root_cell_offset);
}

Hive::~Hive()
{
	delete m_root;
}

std::wstring Hive::GetVersion()
{
	return std::to_wstring(m_major_version_number) + std::wstring(L".") + std::to_wstring(m_minor_version_number);
}

unsigned int Hive::GetMajor()
{
	return m_major_version_number;
}

unsigned int Hive::GetMinor()
{
	return m_minor_version_number;
}


auto Hive::GetFileData()
{
	// returns data about the file including
	// file signature
	// last write timestamp
	// major and minor version numbers
	// root cell offset
	// length
	// file name
}

VK* Hive::GetValue(std::wstring keypath, std::wstring valuename)
{
	NK* key = ProcessSubkeys(keypath);

	key->ProcessValues();

	for (int i = 0; i < key->values.size(); i++)
	{
		if (!wcscmp(key->values[i]->GetName().c_str(), valuename.c_str()))
		{
			return key->values[i];

		}
	}
	

	return nullptr;
}

std::vector<VK*> Hive::GetValues(std::wstring keypath)
{
	std::vector<VK*> values;

	NK* key = ProcessSubkeys(keypath);
	key->ProcessValues();


	for (int i = 0; i < key->values.size(); i++)
	{
		values.push_back(key->values[i]);
	}

	return values;
}

std::vector<NK*> Hive::GetSubkeys(std::wstring keypath)
{
	// function that returns a list of all of the subkeys of a specific key
	std::vector<NK*> keys;

	NK* key = ProcessSubkeys(keypath);

	// fake call for an *imaginary* subkey to load the subkey vector
	key->Tunnel(L"blank");

	for (int i = 0; i < key->subkeys.size(); i++)
	{
		keys.push_back(key->subkeys[i]);
	}

	return keys;
}

NK* Hive::ProcessSubkeys(std::wstring keypath)
{
	// function that loads all of the subkeys for a specified path and returns a ptr to the final key
	// with massive thanks to https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	std::wstring delimiter = L"/";
	std::vector<std::wstring> keys;
	std::vector<NK*> key_pointers;
	key_pointers.push_back(m_root);

	size_t pos = 0;
	std::wstring token;
		while ((pos = keypath.find(delimiter)) != std::string::npos) {
		token = keypath.substr(0, pos);
		keys.push_back(token);
		keypath.erase(0, pos + delimiter.length());
	}

	// remove the root key from the list of keys to find
	keys.erase(keys.begin());

	for (int i = 0; i < keys.size(); i++)
	{
		// for each key name, search the previous keys subkey and add that to the end of the list
		key_pointers.push_back(key_pointers[i]->Tunnel(keys[i]));
	}

	return key_pointers[key_pointers.size()-1];
}
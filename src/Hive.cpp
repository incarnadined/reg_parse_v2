#include "Hive.h"

#include <stdexcept>

Hive::Hive(const char* filepath) : m_magic_bytes(0), m_root_cell_offset(0)
{
	fs.open(filepath, std::ifstream::in | std::ifstream::binary);
	if (fs.fail())
	{
		throw std::exception("File failed to open");
	}

	Helper::Read(&fs, 0x00, sizeof(int), &m_magic_bytes);
	if (m_magic_bytes != 1718052210)
	{
		//ERROR!!! file magic bytes are not regf
		throw std::exception("File loaded but was not a hive (bad magic bytes)");
	}

	Helper::Read(&fs, 0x04, sizeof(unsigned int), &m_primary_sequence_number);
	Helper::Read(&fs, 0x08, sizeof(unsigned int), &m_secondary_sequence_number);
	if (m_primary_sequence_number != m_secondary_sequence_number)
	{
		// the hive is dirty and should be cleaned with transaction logs
		// throw std::exception("Hive is dirty");
	}

	// read the last written timestamp of the file
	Helper::Read(&fs, 0x0C, sizeof(long long), &m_last_written);
	Helper::getDateTime(m_last_written);

	// read the major and minor version numbers of the hive
	Helper::Read(&fs, 0x14, sizeof(unsigned int), &m_major_version_number);
	Helper::Read(&fs, 0x18, sizeof(unsigned int), &m_minor_version_number);

	Helper::Read(&fs, 0x24, sizeof(unsigned int), &m_root_cell_offset);
	m_root = new NK(this, &fs, m_root_cell_offset);
}

Hive::~Hive()
{
	fs.close();
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

std::vector<VK*> Hive::GetValues(NK* key)
{
	std::vector<VK*> values;

	key->ProcessValues();


	for (int i = 0; i < key->values.size(); i++)
	{
		values.push_back(key->values[i]);
	}

	return values;
}

NK* Hive::GetKey(NK* parent, std::wstring keypath)
{
	// function that returns a list of all of the subkeys of a specific key

	return ProcessSubkeys(parent, keypath);
}

NK* Hive::GetKey(std::wstring keypath)
{
	// function that returns a list of all of the subkeys of a specific key

	return ProcessSubkeys(m_root, keypath);
}

std::vector<NK*> Hive::GetSubkeys(NK* parent)
{
	// function that returns a list of all of the subkeys of a specific key
	std::vector<NK*> keys;

	if (parent == nullptr)
		return keys;

	// fake call for an *imaginary* subkey to load the subkey vector
	parent->Tunnel(L"blank");

	for (int i = 0; i < parent->subkeys.size(); i++)
	{
		keys.push_back(parent->subkeys[i]);
	}

	return keys;
}

std::vector<NK*> Hive::GetSubkeys(std::wstring keypath)
{
	// function that returns a list of all of the subkeys of a specific key
	std::vector<NK*> keys;

	NK* key = ProcessSubkeys(keypath);
	return GetSubkeys(key);
}

NK* Hive::GetRoot()
{
	return m_root;
}

NK* Hive::ProcessSubkeys(std::wstring keypath)
{
	return ProcessSubkeys(m_root, keypath);
}

NK* Hive::ProcessSubkeys(NK* parent, std::wstring relativeKeyPath)
{
	// function that loads all of the subkeys for a specified path and returns a ptr to the final key
	// with massive thanks to https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	std::wstring delimiter = L"/";
	std::vector<std::wstring> keys;
	std::vector<NK*> key_pointers;
	key_pointers.push_back(parent);

	if (relativeKeyPath.back() != L'/')
		relativeKeyPath.append(L"/");

	size_t pos = 0;
	std::wstring token;
	while ((pos = relativeKeyPath.find(delimiter)) != std::string::npos) {
		token = relativeKeyPath.substr(0, pos);
		keys.push_back(token);
		relativeKeyPath.erase(0, pos + delimiter.length());
	}

	if (keys.size() == 0) {
		throw std::exception("Invalid key path");
		return nullptr;
	}

	// remove the root key from the list of keys to find
	// keys.erase(keys.begin());

	for (int i = 0; i < keys.size(); i++)
	{
		// for each key name, search the previous keys subkey and add that to the end of the list
		key_pointers.push_back(key_pointers[i]->Tunnel(keys[i]));
	}

	if (key_pointers[key_pointers.size() - 1] == nullptr)
		key_pointers.pop_back();

	return key_pointers[key_pointers.size() - 1];
}
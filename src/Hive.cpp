#include "Hive.h"

Hive::Hive(const char* filepath) : m_magic_bytes(0), m_root_cell_offset(0)
{
	fs.open(filepath, std::ifstream::in | std::ifstream::binary);

	fs.read((char *)&m_magic_bytes, sizeof(int));
	if (m_magic_bytes != 1718052210)
	{
		//ERROR!!! file magic bytes are not regf
		throw;
	}

	fs.read((char *)&m_primary_sequence_number, sizeof(unsigned int));
	fs.read((char *)&m_secondary_sequence_number, sizeof(unsigned int));
	if (m_primary_sequence_number != m_secondary_sequence_number)
	{
		// the hive is dirty and should be cleaned with transaction logs
		//throw;
	}

	// read the last written timestamp of the file (FILETIME format) and convert to SYSTEMTIME
	fs.read((char*)&m_last_written, sizeof(FILETIME));
	FileTimeToSystemTime(&m_last_written, m_timestamp);

	// read the major and minor version numbers of the hive
	fs.read((char*)&m_major_version_number, sizeof(unsigned int));
	fs.read((char*)&m_minor_version_number, sizeof(unsigned int));

	fs.seekg(0x24);
	fs.read((char*)&m_root_cell_offset, sizeof(unsigned int));

	m_root = std::make_shared<NK>(&fs, m_root_cell_offset);
}

Hive::~Hive()
{
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

int Hive::GetValue(std::string keypath, char* valuename)
{
	// returns a tuple containing data for a specific value from a keypath

	std::shared_ptr<NK> key = ProcessSubkeys(keypath);
	key->ProcessValues();

	for (int i = 0; i < key->values.size(); i++)
	{
		if (!strcmp(key->values[i]->m_name, valuename))
		{
			std::cout << key->values[i]->m_name;
		}
	}

	return 0;
}

int Hive::GetValues(std::string keypath)
{
	// returns a list of tuples with a tuple for each value of the subkey

	return 0;
}

void Hive::GetSubkeys(std::string keypath)
{
	// function that prints all of the subkeys of a specific key
	std::shared_ptr<NK> key = ProcessSubkeys(keypath);

	// fake call for an *imaginary* subkey to load the subkey vector
	key->Tunnel("blank");

	for (int i = 0; i < key->subkeys.size(); i++)
	{
		std::cout << key->subkeys[i]->m_name << std::endl;
	}
}

std::shared_ptr<NK> Hive::ProcessSubkeys(std::string keypath)
{
	// function that loads all of the subkeys for a specified path and returns a ptr to the final key
	// with massive thanks to https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	std::string delimiter = "/";
	std::vector<std::string> keys;
	std::vector<std::shared_ptr<NK>> key_pointers;
	key_pointers.push_back(m_root);

	size_t pos = 0;
	std::string token;
		while ((pos = keypath.find(delimiter)) != std::string::npos) {
		token = keypath.substr(0, pos);
		keys.push_back(token);
		keypath.erase(0, pos + delimiter.length());
	}

	// remove the root key from the list of keys to find
	keys.erase(keys.begin());

	// don't look for the imaginary subkey of the final key
	for (int i = 0; i < keys.size(); i++)
	{
		// for each key name, search the previous keys subkey and add that to the end of the list
		key_pointers.push_back(key_pointers[i]->Tunnel(keys[i].c_str()));
	}

	return key_pointers[key_pointers.size()-1];
}
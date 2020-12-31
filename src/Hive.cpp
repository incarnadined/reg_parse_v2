#include "Hive.h"

Hive::Hive(const char* filepath) : m_magic_bytes(0), m_root_cell_offset(0)
{
	std::ifstream fs;
	fs.open(filepath, std::ifstream::in | std::ifstream::binary);

	fs.read((char *)&m_magic_bytes, sizeof(int));
	if (m_magic_bytes != 1718052210)
	{
		//ERROR!!! file magic bytes are not regf
	}

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

auto Hive::GetValue(char* keypath, char* valuename)
{
}

auto Hive::GetValues(char* keypath)
{
}

int Hive::ListSubkeys(std::string keypath)
{
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
	std::cout << keypath << std::endl;

	for (int i = 0; i < keys.size(); i++)
	{
		key_pointers.push_back(key_pointers[i]->Tunnel(keys[i].c_str()));
	}

	for (int i = 0; i < ((key_pointers[key_pointers.size() - 1])->subkeys).size(); i++)
	{
		std::cout << key_pointers[key_pointers.size() - 1]->subkeys.at(i)->m_name << std::endl;
	}

	return 0;
}

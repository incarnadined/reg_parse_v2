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

	NK* root = new NK(&fs, m_root_cell_offset);
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

auto Hive::ListSubkeys(char* keypath)
{
}

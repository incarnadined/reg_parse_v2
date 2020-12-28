#include "NK.h"

NK::NK(std::ifstream* fs, unsigned int offset) : m_offset(offset)
{
	fs->seekg(0x1000 + m_offset); // all offsets are relative to the first hbin (located at 0x1000)

	fs->seekg(0x1000 + m_offset + 0x18);
	fs->read((char*)&m_subkey_count, sizeof(unsigned int));
	fs->read((char*)&m_subkey_offset, sizeof(unsigned int));
	fs->read((char*)&m_value_count, sizeof(unsigned int));
	fs->read((char*)&m_value_offset, sizeof(unsigned int));
	fs->read((char*)&m_parent_offset, sizeof(unsigned int));
	fs->seekg(0x1000 + m_offset + 0x50);
	fs->read((char*)&m_name_length, sizeof(unsigned short));
	fs->read((char*)&m_name, m_name_length);
}

NK::~NK()
{
}

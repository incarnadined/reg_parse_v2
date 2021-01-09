#include "VK.h"

VK::VK(std::ifstream* fs, unsigned int offset) : m_offset(offset), m_fs(fs)
{
	m_fs->seekg(0x1000 + m_offset); // all offsets are relative to the first hbin (located at 0x1000)

	m_fs->read((char*)&m_size, sizeof(int));
	m_fs->read((char*)&m_name_length, sizeof(unsigned short));
	m_fs->read((char*)&m_data_length, sizeof(unsigned int));
	m_fs->read((char*)&m_data, sizeof(unsigned int));
	m_fs->read((char*)&m_type, sizeof(unsigned int));
	m_name = new char[(long long)m_name_length + 1];
	m_fs->seekg(0x1000 + m_offset + 0x18);
	m_fs->read((char*)m_name, m_name_length);
	m_name[m_name_length] = '\0';

	// check if data is resident (first bit of m_data_length is set)
	if (m_data_length >= 0x80000000)
	{
		m_data_length = m_data_length - 0x80000000;
		m_resident = true;
	}
}

VK::~VK()
{
	delete[] m_name;
}

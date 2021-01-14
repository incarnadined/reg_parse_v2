#include "VK.h"

VK::VK(std::istream* fs, unsigned int offset) : m_offset(offset), m_fs(fs), m_resident(false), m_retrieved(false)
{
	Helper::Read(m_fs, 0x1000 + m_offset + 0x00, sizeof(int), &m_size);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x06, sizeof(unsigned short), &m_name_length);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x08, sizeof(int), &m_data_length);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x0C, sizeof(unsigned int), &m_data);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x10, sizeof(RegType), &m_type);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x14, sizeof(unsigned short), &m_flags);

	m_name = new char[(long long)m_name_length + 1];
	Helper::Read(m_fs, 0x1000 + m_offset + 0x18, m_name_length, m_name);
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
	delete data_node;
}

RegType VK::GetType()
{
	return m_type;
}

unsigned char* VK::LoadData()
{
	// creates an array of the data retrieved from the ptr, or from the data var if it is resident

	if (m_resident)
	{
		switch (m_data_length)
		{
		case 4:
			return (unsigned char *)&m_data;

		case 3:
			return (unsigned char *)&m_data + 1;

		case 2:
			return (unsigned char *)&m_data + 2;

		case 1:
			return (unsigned char *)&m_data + 3;
		}
	}
	else
	{
		if (m_retrieved)
		{
			// if data has already been retrieved
			return data_node->raw_data;
		}

		data_node = new DataNode(m_fs, m_data);
		m_retrieved = true;

		return data_node->raw_data;
	}

	return nullptr;
}

void VK::PrettyPrintData()
{
	switch (m_type)
	{
	case RegType::RegNone:
		break;

	case RegType::RegSz:
		wchar_t* data = (wchar_t*)this->LoadData();
		std::cout << data << std::endl;

		break;
	}
}

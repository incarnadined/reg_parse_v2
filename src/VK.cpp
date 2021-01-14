#include "VK.h"

VK::VK(std::istream* fs, unsigned int offset) : m_offset(offset), m_fs(fs), retrieved_data(0), m_resident(false)
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
	delete[] retrieved_data;
}

RegType VK::GetType()
{
	return m_type;
}

char* VK::GetData()
{
	// creates an array of the data retrieved from the ptr, or from the data var if it is resident

	if (m_resident)
	{
		switch (m_data_length)
		{
		case 4:
			return (char *)&m_data;

		case 3:
			return (char *)&m_data + 1;

		case 2:
			return (char *)&m_data + 2;

		case 1:
			return (char *)&m_data + 3;
		}
	}
	else
	{
		if (retrieved_data)
		{
			// if retrieved data is already populated return it
			return retrieved_data;
		}

		list* list_instance = new list(m_fs, m_data);

		retrieved_data = list_instance->data;

		delete list_instance;

		return retrieved_data;
	}

	return nullptr;
}

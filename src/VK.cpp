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

char* VK::GetName()
{
	return m_name;
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

std::wstring VK::GetData()
{
	switch (m_type)
	{
	case RegType::RegNone:
		break;

	case RegType::RegSz:
	{
		std::wstring* data = (std::wstring *)this->LoadData();
		return *data;
		break;
	}

	case RegType::RegExpandSz:
	{
		std::wstring* data = (std::wstring*)this->LoadData();
		return *data;
		break;
	}

	case RegType::RegBinary:
	{
		std::wstringstream stream;
		unsigned int* data = (unsigned int*)this->LoadData();
		stream << std::hex << *data;
		return stream.str();
		break;
	}

	case RegType::RegDword:
	{
		unsigned int* data = (unsigned int*)this->LoadData();
		return std::to_wstring(*data);
		break;
	}

	case RegType::RegDwordBigEndian:
	{
		//write some big endian code

		unsigned char* buffer = this->LoadData();
		//int num = (int)&buffer[0] | (int)&buffer[1] << 8 | (int)&buffer[2] << 16 | (int)&buffer[3] << 24;
		break;
	}

	case RegType::RegLink:
	{
		// i don't want to
		break;
	}

	case RegType::RegMultiSz:
	{
		//i still don't want to
	}

	case RegType::RegResourceList:
	{
		// just nope
		break;
	}

	case RegType::RegFullResourceDescription:
	{
		// guess what
		break;
	}

	case RegType::RegResourceRequirementsList:
	{
		break;
	}

	case RegType::RegQWord:
	{
		unsigned long long* data = (unsigned long long*)this->LoadData();
		return std::to_wstring(*data);
		break;
	}

	case RegType::RegFileTime:
	{
		//TIME* data = (unsigned long long*)this->LoadData();
		break;
	}
	}
}

#include "VK.h"

#include <numeric>

VK::VK(std::ifstream* fs, unsigned int offset) : m_offset(offset), m_fs(fs), m_resident(false), m_retrieved(false)
{
	unsigned int type;
	Helper::Read(m_fs, 0x1000 + m_offset + 0x00, sizeof(int), &m_size);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x06, sizeof(unsigned short), &m_name_length);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x08, sizeof(int), &m_data_length);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x0C, sizeof(unsigned int), &m_data);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x10, sizeof(unsigned int), &type);
	Helper::Read(m_fs, 0x1000 + m_offset + 0x14, sizeof(unsigned short), &m_flags);

	m_type = static_cast<RegType>(type & 0x00FF); // if the value has the upper bytes set then why on earth? pretend they aren't

	// if no name is stored then the name should be (Default)
	if (m_name_length == 0)
	{
		m_name = L"(Default)";
	}
	// if the name is ascii
	else if (m_flags > 0)
	{
		// read the name into a char array and then convert that array
		// into a wstring by adding null bytes with the CharToWstring function

		char* temp_name = new char[(long long)m_name_length + 1];

		Helper::Read(m_fs, 0x1000 + m_offset + 0x18, m_name_length, temp_name);
		temp_name[m_name_length] = '\0';

		m_name = Helper::CharToWstring(temp_name, m_name_length);

		delete[] temp_name;
	}
	// else it must be unicode
	else
	{
		Helper::Read(m_fs, 0x1000 + m_offset + 0x18, m_name_length, &m_name);
	}

	// check if data is resident (first bit of m_data_length is set)
	if (m_data_length >= 0x80000000)
	{
		m_data_length = m_data_length - 0x80000000;
		m_resident = true;
	}
}

VK::~VK()
{
}

RegType VK::GetType()
{
	return m_type;
}

std::wstring VK::GetName()
{
	return m_name;
}

bool VK::GetResidence()
{
	return m_resident;
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

		data_node = std::make_unique<DataNode>(m_fs, m_data);
		m_retrieved = true;

		return data_node->raw_data;
	}

	return nullptr;
}

std::wstring VK::GetData()
{
	switch (m_type)
	{
	case RegType::RegExpandSz:
	case RegType::RegSz:
	{
		unsigned char* data = this->LoadData();
		if (*data == 0)
			return std::wstring(L"(Default)");
		return std::wstring((wchar_t *)data);
		break;
	}

	case RegType::RegNone:
	case RegType::RegBinary:
	{
		std::wstringstream stream;
		unsigned short* data = (unsigned short*)this->LoadData();
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
		unsigned char* buffer = this->LoadData();
		int num = (int)&buffer[0] | (int)&buffer[1] << 8 | (int)&buffer[2] << 16 | (int)&buffer[3] << 24;
		return std::to_wstring(num);
		break;
	}

	case RegType::RegLink:
	{
		// i don't want to
		break;
	}

	case RegType::RegMultiSz:
	{
		std::vector<std::wstring> strings;
		unsigned char* data = this->LoadData();
		unsigned char* ptr = data;
		while (data - ptr < m_data_length - 2) { // the last two bytes are null to mark the end
			strings.push_back(std::wstring((wchar_t *)data));
			data += strings[strings.size() - 1].size() * 2 + 2;
		}
		return std::accumulate(
			std::next(strings.begin()),
			strings.end(),
			strings[0],
			[](std::wstring a, std::wstring b) {
				return a + L", " + b;
			}
		);
		break;
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
		unsigned long long* data = (unsigned long long*)this->LoadData();
		return Helper::getDateTime(*data);
		break;
	}
	}

	return L"(Not implemented)";
}

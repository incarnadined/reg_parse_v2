#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

#include "includes.h"
#include "VK.h"

class NK
{
public:
	NK(std::istream* fs, unsigned int offset);
	~NK();

	std::shared_ptr<NK> Tunnel(std::wstring keyname);
	void ProcessValues();
	std::wstring GetName();

	std::vector<std::shared_ptr<NK>> subkeys;
	std::vector<std::shared_ptr<VK>> values;

private:
	std::istream* m_fs;
	unsigned int m_offset;

	enum class Flags : unsigned short {
		KEY_VOLATILE = 0x01,
		KEY_HIVE_EXIT = 0x02,
		KEY_HIVE_ENTRY = 0x04,
		KEY_HIVE_NO_DELETE = 0x08,
		KEY_SYM_LINK = 0x10,
		KEY_COMP_NAME = 0x20,
		KEY_PREDEF_HANDLE = 0x40,
	};
	inline friend bool operator&(unsigned short a, Flags b)
	{
		return a&(unsigned short)b;
	}

	std::wstring m_name;
	std::shared_ptr<NK> parent;

	int m_size;
	unsigned short m_flags;
	long long m_last_write;
	unsigned int m_parent_offset;
	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned short m_name_length;
};

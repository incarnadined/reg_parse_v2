#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "includes.h"
#include "VK.h"

class NK
{
public:
	NK(std::ifstream* fs, unsigned int offset);
	~NK();

	NK* Tunnel(std::wstring keyname);
	void ProcessValues();

	std::wstring GetName();
	void SetParent(NK* parent_key);
	NK* GetParent();

	std::vector<NK*> subkeys;
	std::vector<VK*> values;

private:
	std::ifstream* m_fs;
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
	NK* m_parent;

	int m_size;
	unsigned short m_flags;
	unsigned long long m_last_write;
	unsigned int m_parent_offset;
	unsigned int m_subkey_count;
	unsigned int m_subkey_offset;
	unsigned int m_value_count;
	unsigned int m_value_offset;
	unsigned short m_name_length;
};

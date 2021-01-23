#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include "includes.h"

enum class RegType {
	RegNone,
	RegSz,
	RegExpandSz,
	RegBinary,
	RegDword,
	RegDwordBigEndian,
	RegLink,
	RegMultiSz,
	RegResourceList,
	RegFullResourceDescription,
	RegResourceRequirementsList,
	RegQWord,
	RegFileTime = 0x10
};

class VK
{
public:
	VK(std::ifstream* fs, unsigned int offset);
	~VK();

	RegType GetType();
	std::wstring GetName();
	bool GetResidence();

	unsigned char* LoadData();
	std::wstring GetData();

	unsigned int m_data_length;

private:
	std::ifstream* m_fs;
	unsigned int m_offset;
	bool m_resident;
	bool m_retrieved;
	std::unique_ptr<DataNode> data_node;

	int m_size;
	unsigned short m_name_length;
	std::wstring m_name;
	unsigned int m_data;
	unsigned short m_flags;
	RegType m_type;
};
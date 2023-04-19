#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include "includes.h"

enum class RegType {
	RegNone = 0x0000,
	RegSz = 0x0001,
	RegExpandSz = 0x0002,
	RegBinary = 0x0003,
	RegDword = 0x0004,
	RegDwordBigEndian = 0x0005,
	RegLink = 0x0006,
	RegMultiSz = 0x0007,
	RegResourceList = 0x0008,
	RegFullResourceDescription = 0x0009,
	RegResourceRequirementsList = 0x000A,
	RegQWord = 0x000B,
	RegFileTime = 0x0010
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
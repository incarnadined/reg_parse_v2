#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include "includes.h"

enum class RegType{
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
	RegFileTime=0x10
};

class VK
{
public:
	VK(std::istream* fs, unsigned int offset);
	~VK();

	RegType GetType();
	unsigned char* LoadData();
	void PrettyPrintData();

	char* m_name;
	unsigned int m_data_length;

private:
	std::istream* m_fs;
	unsigned int m_offset;
	bool m_resident;
	bool m_retrieved;
	DataNode* data_node;

	int m_size;
	unsigned short m_name_length;
	unsigned int m_data;
	unsigned short m_flags;
	RegType m_type;
};
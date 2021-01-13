#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include "list.h"

enum class RegType{
	RegNone,  RegSz, RegExpandSz, RegBinary,
	RegDword, RegDwordBigEndian, RegLink, RegMultiSz,
	RegResourceList, RegFullResourceDescription,
	RegResourceRequirementsList, RegQWord, RegFileTime=0x10
};

class VK
{
public:
	VK(std::ifstream* fs, unsigned int offset);
	~VK();

	RegType GetType();
	char* GetData();

	char* m_name;
	unsigned int m_data_length;

private:
	std::ifstream* m_fs;
	unsigned long long m_offset;
	bool m_resident;

	char* retrieved_data;

	int m_size;
	unsigned short m_name_length;
	unsigned int m_data;
	unsigned short m_flags;
	RegType m_type;
};
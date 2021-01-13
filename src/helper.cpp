#pragma once

#include "helper.h"

void Helper::Read(std::istream* fs, unsigned int offset, unsigned long long size, void* ptr)
{
	fs->seekg(offset);
	fs->read((char*)ptr, size);
}
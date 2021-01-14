#include "DataNode.h"

DataNode::DataNode(std::istream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);

	pointers = new unsigned int[(abs(m_size) - 4) / 4];
	for (int i = 0; i < (abs(m_size) - 4) / 4; i++)
	{
		Helper::Read(fs, 0x1000 + offset + 0x4 + i * 4, sizeof(unsigned int), &pointers[i]);
	}
}

DataNode::~DataNode()
{
	delete[] pointers;
}

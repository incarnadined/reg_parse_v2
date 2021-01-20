#include "DataNode.h"

DataNode::DataNode()
{
}

DataNode::DataNode(std::istream* fs, unsigned int offset)
{
	Helper::Read(fs, 0x1000 + offset, sizeof(int), &m_size);

	// sometimes a data node will hold pointers
	delete pointers;
	pointers = new unsigned int[((long long)abs(m_size) - 4) / 4];
	for (int i = 0; i < (abs(m_size) - 4) / 4; i++)
	{
		Helper::Read(fs, 0x1000 + offset + 0x4 + i * 4, sizeof(unsigned int), &pointers[i]);
	}

	// sometimes the data node will hold a size and a load of raw data
	delete raw_data;
	raw_data = new unsigned char[(long long)abs(m_size)-4];
	Helper::Read(fs, 0x1000 + offset + 0x4, (long long)abs(m_size) - 4, raw_data);
}

DataNode::~DataNode()
{
	delete[] pointers;
	delete[] raw_data;
}

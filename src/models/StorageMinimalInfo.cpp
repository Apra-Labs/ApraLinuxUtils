/*
 * StorageInfo.cpp
 *
 *  Created on: 18-Apr-2023
 *      Author: developer
 */

#include <models/StorageMinimalInfo.h>

namespace apra
{

StorageMinimalInfo::StorageMinimalInfo(std::string partition, uint64_t size,
		std::string fsType) :
		m_partition(partition), m_size(size), m_fsType(fsType)
{
	// TODO Auto-generated constructor stub

}

StorageMinimalInfo::StorageMinimalInfo() :
		m_partition(""), m_size(0), m_fsType("")
{

}

StorageMinimalInfo::~StorageMinimalInfo()
{
	// TODO Auto-generated destructor stub
}

StorageMinimalInfo& StorageMinimalInfo::operator=(
		const StorageMinimalInfo &other)
{
	m_partition = other.m_partition;
	m_size = other.m_size;
	m_fsType = other.m_fsType;
	return *this;
}
} /* namespace apra */


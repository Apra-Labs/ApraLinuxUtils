/*
 * StorageInfo.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <models/StorageMinimalInfo.h>

namespace apra
{

StorageMinimalInfo::StorageMinimalInfo(std::string partition, uint64_t size,
		std::string fsType) :
		m_partition(partition), m_size(size), m_fsType(fsType)
{
}

StorageMinimalInfo::StorageMinimalInfo() :
		m_partition(""), m_size(0), m_fsType("")
{

}

StorageMinimalInfo::~StorageMinimalInfo()
{
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


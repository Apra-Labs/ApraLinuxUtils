/*
 * StorageInfo.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_MODELS_STORAGEMINIMALINFO_H_
#define INCLUDES_APRA_MODELS_STORAGEMINIMALINFO_H_
#include <cstdint>
#include <string>

namespace apra
{

class StorageMinimalInfo
{
public:
	StorageMinimalInfo(std::string partition, uint64_t size, std::string fsType);
	StorageMinimalInfo();
	virtual ~StorageMinimalInfo();

	StorageMinimalInfo& operator=(const StorageMinimalInfo& other);
	std::string m_partition;
	uint64_t m_size;
	std::string m_fsType;
};

} /* namespace apra */

#endif /* INCLUDES_APRA_MODELS_STORAGEMINIMALINFO_H_ */

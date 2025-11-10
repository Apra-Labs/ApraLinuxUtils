/*
 * StorageInfo.h
 *
 *  Created on: 18-Apr-2023
 *      Author: developer
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

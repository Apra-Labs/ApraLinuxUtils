/*
 * StorageType.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_
#define INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_
#include <string>

namespace apra
{
enum STORAGE_TYPE
{
	FAT32, NTFS, EXT4, UNSUPPORTED
};

class STORAGE_TYPE_STRING
{
public:
	static std::string getString(STORAGE_TYPE type);
	static STORAGE_TYPE getEnum(std::string typeStr);
};

} /* namespace apra */

#endif /* INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_ */

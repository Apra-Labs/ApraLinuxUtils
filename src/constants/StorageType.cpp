/*
 * StorageType.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <constants/StorageType.h>

namespace apra
{

std::string STORAGE_TYPE_STRING::getString(STORAGE_TYPE type)
{
	switch (type)
	{
	case FAT32:
		return "vfat";
	case NTFS:
		return "ntfs";
	case EXT4:
		return "ext4";
	default:
		return "";
	}
}

STORAGE_TYPE STORAGE_TYPE_STRING::getEnum(std::string typeStr)
{
	if (typeStr == getString(NTFS))
	{
		return NTFS;
	}
	else if (typeStr == getString(EXT4))
	{
		return EXT4;
	}
	else if (typeStr == getString(FAT32))
	{
		return FAT32;
	}
	else
	{
		return static_cast<STORAGE_TYPE>(-1);
	}
}

} /* namespace apra */

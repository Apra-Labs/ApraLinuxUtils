/*
 * StorageType.cpp
 *
 *  Created on: 17-Apr-2023
 *      Author: developer
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
	else
	{
		return FAT32;
	}
}

} /* namespace apra */

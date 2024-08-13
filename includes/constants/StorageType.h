/*
 * StorageType.h
 *
 *  Created on: 17-Apr-2023
 *      Author: developer
 */

#ifndef INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_
#define INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_
#include <string>

namespace apra
{
enum STORAGE_TYPE
{
	FAT32, NTFS, EXT4
};

class STORAGE_TYPE_STRING
{
public:
	static std::string getString(STORAGE_TYPE type);
	static STORAGE_TYPE getEnum(std::string typeStr);
};

} /* namespace apra */

#endif /* INCLUDES_APRA_CONSTANTS_STORAGETYPE_H_ */

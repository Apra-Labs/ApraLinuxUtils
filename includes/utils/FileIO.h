/*
 * FileIOUtils.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef SRC_APRA_UTILS_FILEIO_H_
#define SRC_APRA_UTILS_FILEIO_H_
#include <string>

using namespace std;

namespace apra
{
class FileIO
{
private:
	FileIO();
	virtual ~FileIO();
public:
	static bool isFileExist(const string &path);
	static bool isDirectoryExist(const string &path);
};
}

#endif /* SRC_APRA_UTILS_FILEIO_H_ */

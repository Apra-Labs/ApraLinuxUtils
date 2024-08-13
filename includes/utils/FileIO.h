/*
 * FileIOUtils.h
 *
 *  Created on: 01-Feb-2023
 *      Author: developer
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

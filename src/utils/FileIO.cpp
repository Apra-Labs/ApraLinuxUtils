/*
 * FileIOUtils.cpp
 *
 *  Created on: 01-Feb-2023
 *      Author: developer
 */

#include "utils/FileIO.h"
#include <sys/stat.h>

using namespace apra;

FileIO::FileIO()
{
	// TODO Auto-generated constructor stub

}

FileIO::~FileIO()
{
	// TODO Auto-generated destructor stub
}

bool FileIO::isFileExist(const string &path)
{
	bool returnValue;
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
	{
		returnValue = false;
	}
	else
	{
		returnValue = S_ISREG(info.st_mode) != 0;
	}
	return returnValue;
}

bool FileIO::isDirectoryExist(const string &path)
{
	bool returnValue;
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
	{
		returnValue = false;
	}
	else
	{
		returnValue = S_ISDIR(info.st_mode) != 0;
	}
	return returnValue;
}


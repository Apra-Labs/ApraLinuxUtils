/*
 * FileIO.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include "utils/FileIO.h"
#include <sys/stat.h>

using namespace apra;

FileIO::FileIO()
{

}

FileIO::~FileIO()
{
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


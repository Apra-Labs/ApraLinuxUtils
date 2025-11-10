/*
 * Utils.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <stdio.h>
#include <memory>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <cstring>
#include <fstream>
#include "models/Range.h"
#include "utils/Utils.h"

using namespace apra;
Utils::Utils()
{

}

Utils::~Utils()
{
}

bool Utils::saveRawFile(string fileName, uint8_t *data, size_t size)
{
	FILE *fp = fopen(fileName.c_str(), "wb");
	if (!fp)
	{
		return false;
	}
	size_t ret = fwrite(data, 1, size, fp);
	fclose(fp);
	return ret > 0;
}

string Utils::makeDir(string path)
{
	string cmd = "mkdir -p \"";
	cmd += path + "\"";
	string errStr;
	try
	{
		errStr = exec(cmd);
	} catch (std::runtime_error &e)
	{
		printf("mkdir error: %s\n", e.what());
	}
	if (errStr.length())
	{
		printf("Utils::makeDir %s\n", errStr.c_str());
		return "";
	}
	return path;
}

std::string Utils::exec(const std::string &cmd, bool debug)
{
	if (debug)
	{
		printf("CMD To be Run %s \n", cmd.c_str());
	}
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen((cmd + " 2>&1").c_str(), "r"), pclose);
	if (!pipe)
	{
		throw std::runtime_error(string("popen() failed for") + cmd);
	}
	while (!feof(pipe.get()))
	{
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
		{
			result += buffer.data();
		}
	}
	return result;
}

vector<uint8_t> Utils::extractBytes(uint64_t hexData, uint8_t numberOfBytes)
{
	vector<uint8_t> extractedBytes;
	numberOfBytes = numberOfBytes > 8 ? 8 : numberOfBytes;
	for (uint8_t count = 0; count < numberOfBytes; count++)
	{
		uint8_t extractByte = (hexData >> ((numberOfBytes - 1 - count) * 8))
				& 0xFF;
		extractedBytes.push_back(extractByte);
	}
	return extractedBytes;
}

uint64_t Utils::combineBytes(vector<uint8_t> byteArray)
{
	uint64_t combinedResult = 0;
	for (uint8_t count = 0; count < byteArray.size(); count++)
	{
		combinedResult = ((combinedResult << 8) & 0xFFFFFFFFFFFFFF00)
				| byteArray[count];
	}
	return combinedResult;
}

bool Utils::inRange(int64_t value, Range range)
{
	return (value >= range.m_min && value <= range.m_max);
}

bool Utils::directoryExists(const std::string &path)
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

bool Utils::fileExists(const std::string &path)
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

bool Utils::caseInsensitiveSearch(std::string const str,
		std::string const pattern)
{
	std::string::const_iterator it = std::search(str.begin(), str.end(),
			pattern.begin(), pattern.end(),
			[](unsigned char ch1, unsigned char ch2)
			{	return std::toupper(ch1) == std::toupper(ch2);});
	return (it != str.end());
}

string Utils::trim(string str)
{
	string WHITESPACE = " \n\r\t\f\v";
	string returnValue;
	size_t first = str.find_first_not_of(WHITESPACE);
	if (first == std::string::npos)
	{
		returnValue = "";
	}
	else
	{
		size_t last = str.find_last_not_of(WHITESPACE);
		returnValue = str.substr(first, (last - first + 1));
	}
	return returnValue;
}

void Utils::getFilesInDirectoryRecursive(const std::string &directoryPath,
		std::vector<std::string> &fileList)
{
	DIR *dir = opendir(directoryPath.c_str());
	if (dir == nullptr)
	{
		printf("Error opening directory: %s\n", directoryPath.c_str());
		return;
	}

	dirent *entry;
	while ((entry = readdir(dir)) != nullptr)
	{
		if (std::string(entry->d_name) != "."
				&& std::string(entry->d_name) != "..")
		{
			std::string fullPath = directoryPath + "/" + entry->d_name;

			if (entry->d_type == DT_DIR)
			{
				getFilesInDirectoryRecursive(fullPath, fileList); // Recursively call for subdirectory
			}
			else
			{
				fileList.push_back(fullPath);  // Add file path to the list
			}
		}
	}

	closedir(dir);
}

void Utils::getFilesInDirectory(string path, vector<string> &files,
		bool recursive)
{
	files.clear();
	if (recursive)
	{
		getFilesInDirectoryRecursive(path, files);
		return;
	}
	DIR *dir = opendir(path.c_str());
	if (dir == nullptr)
	{
		printf("Error opening directory: %s\n", path.c_str());
		return;
	}
	dirent *entry;
	while ((entry = readdir(dir)) != nullptr)
	{
		if (entry->d_type == DT_REG)
		{
			files.push_back(entry->d_name); // Add file name to the list
		}
	}
	closedir(dir);
}

string Utils::readTextFile(string filePath)
{
	string response;
	bool fileExistCheck = fileExists(filePath);
	if (!fileExistCheck)
	{
		return response;
	}
	ifstream inFile;
	inFile.open(filePath.c_str());
	if (!inFile)
	{
		return response;
	}
	char x;
	string data;
	while (inFile >> x)
	{
		data = data + x;
	}
	inFile.close();

	if (data.length() > 0)
	{
		response = data;
	}
	return response;
}
uint16_t Utils::convertToU12p4(double value)
{
	uint16_t decVal = value;
	uint16_t intVal = value;
	value = value - intVal;

	for (int16_t idx = 0; idx < 4; idx++)
	{
		value *= 2;
		intVal = value;
		if (intVal == 1)
		{
			value = value - intVal;
			decVal = (decVal << 1) + 1;
		}
		else
		{
			decVal = decVal << 1;
		}
	}
	return decVal;
}
double Utils::convertFrom12p4(uint16_t value)
{
	double returnValue;
	double decVal = 0;
	uint16_t fractionValue = 0;
	decVal = value >> 4;
	fractionValue = value & 0x000F;
	double temp = 0.00;
	double twos = 2.00;
	for (int32_t idx = 3; idx >= 0; idx--)
	{
		temp += (fractionValue >> idx & 0x1) / twos;
		twos = twos * 2;
	}
	returnValue = (decVal + temp);
	return returnValue;
}
uint16_t Utils::convertTo10p6(double value)
{
	uint16_t decVal = value;
	uint16_t intVal = value;
	value = value - intVal;

	for (int16_t idx = 0; idx < 6; idx++)
	{
		value *= 2;
		intVal = value;
		if (intVal == 1)
		{
			value = value - intVal;
			decVal = (decVal << 1) + 1;
		}
		else
		{
			decVal = decVal << 1;
		}
	}
	return decVal;

}
double Utils::convertFrom10p6(uint16_t value)
{
	double returnValue;
	double decVal = 0;
	uint16_t fractionValue = 0;
	decVal = value >> 6;
	fractionValue = value & 0x0003F;
	double temp = 0.00;
	double twos = 2.00;
	for (int32_t idx = 5; idx >= 0; idx--)
	{
		temp += (fractionValue >> idx & 0x1) / twos;
		twos = twos * 2;
	}
	returnValue = (decVal + temp);
	return returnValue;

}
uint16_t Utils::convertToUFormat(double value, uint8_t format)
{
	uint16_t decVal = value;
	uint16_t intVal = value;
	value = value - intVal;

	for (int16_t idx = 0; idx < format; idx++)
	{
		value *= 2;
		intVal = value;
		if (intVal == 1)
		{
			value = value - intVal;
			decVal = (decVal << 1) + 1;
		}
		else
		{
			decVal = decVal << 1;
		}
	}
	return decVal;

}
double Utils::convertFromUFormat(uint16_t value, uint8_t format)
{
	double returnValue;
	double decVal = 0;
	uint16_t fractionValue = 0;
	decVal = value >> format;
	fractionValue = value & format;
	double temp = 0.00;
	double twos = 2.00;
	for (int32_t idx = (format - 1); idx >= 0; idx--)
	{
		temp += (fractionValue >> idx & 0x1) / twos;
		twos = twos * 2;
	}
	returnValue = (decVal + temp);
	return returnValue;
}
uint64_t Utils::mergefrom8Bytes(uint8_t * bytes)
{
	uint64_t response = 0;
	if (bytes)
	{
		for (uint8_t cnt = 0; cnt < 8; cnt++)
		{
			uint64_t pushValue = bytes[cnt];
			response |= pushValue << (8 * cnt);
		}
	}
	return response;
}

void Utils::extractTo8Bytes(uint64_t timeInSec, uint8_t *bytes)
{
	if (bytes)
	{
		for (uint8_t cnt = 0; cnt < 8; cnt++)
		{
			uint64_t extractedBytes = (timeInSec >> (8 * cnt));
			bytes[cnt] = extractedBytes & 0xFF;
		}
	}
}

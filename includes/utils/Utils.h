/*
 * Utils.h
 *
 *  Created on: Nov 17, 2020
 *      Author: developer
 */

#ifndef SRC_APRA_UTILS_H_
#define SRC_APRA_UTILS_H_

#include <array>
#include <string>
#include <vector>
using namespace std;

namespace apra
{
class Range;
class Utils
{
	Utils();
	virtual ~Utils();
public:
	static bool saveRawFile(string fileName, uint8_t *data, size_t size);
	static string makeDir(string path);
	static string exec(const string &cmd, bool debug = false);
	static vector<uint8_t> extractBytes(uint64_t hexData,
			uint8_t numberOfBytes);
	static uint64_t combineBytes(vector<uint8_t> byteArray);
	static bool inRange(int64_t value, Range range);
	static bool fileExists(const std::string &path);
	static bool directoryExists(const std::string &path);
	static bool caseInsensitiveSearch(std::string const str,
			std::string const pattern);
	static string trim(string str);
	static void getFilesInDirectory(string path, vector<string> &files,
			bool recursive = true);
	static string readTextFile(string filePath);
	static uint16_t convertToU12p4(double value);
	static double convertFrom12p4(uint16_t value);
	static uint16_t convertTo10p6(double value);
	static double convertFrom10p6(uint16_t value);
	static uint16_t convertToUFormat(double value, uint8_t format);
	static double convertFromUFormat(uint16_t value, uint8_t format);
private:
	static void getFilesInDirectoryRecursive(const std::string &directoryPath,
			std::vector<std::string> &fileList);
};
}

#endif /* SRC_APRA_UTILS_H_ */

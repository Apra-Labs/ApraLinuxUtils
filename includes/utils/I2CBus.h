/*
 * I2CBus.h
 *
 *  Created on: 05-Feb-2023
 *      Author: developer
 */

#ifndef SRC_UTILS_I2CBUS_H_
#define SRC_UTILS_I2CBUS_H_
#include <string>
#include <vector>

using namespace std;

#define CONSEQUENT_I2C_TIME_LIMIT_US 1000
namespace apra
{
class I2CError;
class I2C_Bus
{
public:
	I2C_Bus(string i2cPath, bool shouldPrint);
	virtual ~I2C_Bus();
	I2CError openBus();
	void closeBus();
	void setSize(uint8_t registerSize, uint8_t dataSize);
	I2CError writeOnce(uint8_t chipAddress, uint64_t registerAddress,
			uint64_t data);
	I2CError readOnce(uint8_t chipAddress, uint64_t registerAddress,
			uint64_t &data);
	I2CError writeOnI2C(uint8_t chipAddress, uint64_t registerAddress,
			uint64_t data);
	I2CError readOnI2C(uint8_t chipAddress, uint64_t registerAddress,
			uint64_t &data);
	I2CError genericWrite(uint8_t chipAddress, vector<uint8_t> registerAddress,
			vector<uint8_t> data);
	I2CError genericRead(uint8_t chipAddress, vector<uint8_t> registerAddress,
			vector<uint8_t> &readData);
	bool isI2CExecRecommended();
private:
	string m_i2cPath;
	bool m_shouldPrint;
	int32_t m_i2cFileDescriptor;
	uint8_t m_registerSize;
	uint8_t m_dataSize;
	uint64_t m_lastI2COperationTs;
};
}

#endif /* SRC_UTILS_I2CBUS_H_ */

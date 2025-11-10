/*
 * I2CMessage.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: developer
 */

#include "models/I2CMessage.h"
#include "utils/Utils.h"

namespace apra
{

I2C_Message::I2C_Message() :
		m_error(), m_type(I2C_READ), m_registerNumber(), m_data(), m_compareData(), m_retryCount(
				0), m_delayInUsec(0), m_retryDelayInUsec(
		I2C_RETRY_FAILURE_DELAY), m_allowOtherProcessOnIdle(false), m_registerSize(
				0), m_dataSize(0)
{
	// TODO Auto-generated constructor stub

}

I2C_Message::~I2C_Message()
{
	// TODO Auto-generated destructor stub
}

void I2C_Message::configureWrite(vector<uint8_t> registerNumber,
		vector<uint8_t> data)
{
	m_registerNumber = registerNumber;
	m_registerSize = registerNumber.size();
	m_data = data;
	m_dataSize = data.size();
	m_type = I2C_WRITE;
}
void I2C_Message::configureWrite(uint64_t registerNumber, uint64_t data,
		uint64_t registerSize, uint64_t dataSize)
{
	m_registerNumber = Utils::extractBytes(registerNumber, registerSize);
	m_registerSize = registerSize;
	m_data = Utils::extractBytes(data, dataSize);
	m_dataSize = dataSize;
	m_type = I2C_WRITE;
}
void I2C_Message::configureRead(vector<uint8_t> registerNumber,
		uint64_t expectedDataSize)
{
	m_registerNumber = registerNumber;
	m_registerSize = registerNumber.size();
	m_dataSize = expectedDataSize;
	m_type = I2C_READ;
}
void I2C_Message::configureRead(uint64_t registerNumber, uint64_t registerSize,
		uint64_t expectedDataSize)
{
	m_registerNumber = Utils::extractBytes(registerNumber, registerSize);
	m_registerSize = registerSize;
	m_dataSize = expectedDataSize;
	m_type = I2C_READ;
}

void I2C_Message::configureReadWithComparison(vector<uint8_t> registerNumber,
		uint64_t expectedDataSize, const vector<uint8_t> dataCompare,
		bool compareEquals)
{
	m_registerNumber = registerNumber;
	m_registerSize = registerNumber.size();
	m_dataSize = expectedDataSize;
	m_compareData = dataCompare;
	m_type =
			compareEquals ? I2C_READ_COMPARE_EQUAL : I2C_READ_COMPARE_NOT_EQUAL;
}
void I2C_Message::configureReadWithComparison(uint64_t registerNumber,
		uint64_t registerSize, uint64_t expectedDataSize,
		const uint64_t dataCompare, bool compareEquals)
{
	m_registerNumber = Utils::extractBytes(registerNumber, registerSize);
	m_registerSize = registerSize;
	m_dataSize = expectedDataSize;
	m_compareData = Utils::extractBytes(dataCompare, expectedDataSize);
	m_type =
			compareEquals ? I2C_READ_COMPARE_EQUAL : I2C_READ_COMPARE_NOT_EQUAL;

}
void I2C_Message::addDelay(uint64_t delayInUsec)
{
	if (delayInUsec)
	{
		m_delayInUsec = delayInUsec;
	}
}
void I2C_Message::setRetries(uint64_t retryCount)
{
	if (retryCount)
	{
		m_retryCount = retryCount;
	}
}

uint64_t I2C_Message::getCombinedData()
{
	return Utils::combineBytes(m_data);
}

uint64_t I2C_Message::getCombinedRegister()
{
	return Utils::combineBytes(m_registerNumber);
}

uint64_t I2C_Message::getDataSize()
{
	return m_dataSize;
}

} /* namespace apra */

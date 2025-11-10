/*
 * I2CMessage.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef SRC_APRA_MODELS_I2CMESSAGE_H_
#define SRC_APRA_MODELS_I2CMESSAGE_H_

#include <stdint.h>
#include <vector>
#include <models/I2CError.h>
#include <constants/I2CMessageType.h>

#define I2C_RETRY_FAILURE_DELAY 5000

using namespace std;
namespace apra
{
class I2C_Message
{
public:
	I2C_Message();
	virtual ~I2C_Message();
	void configureWrite(vector<uint8_t> registerNumber, vector<uint8_t> data);
	void configureWrite(uint64_t registerNumber, uint64_t data,
			uint64_t registerSize, uint64_t dataSize);
	void configureRead(vector<uint8_t> registerNumber, uint64_t expectedDataSize);
	void configureRead(uint64_t registerNumber, uint64_t registerSize,
			uint64_t expectedDataSize);
	void configureReadWithComparison(vector<uint8_t> registerNumber,
			uint64_t expectedDataSize, const vector<uint8_t> dataCompare,
			bool compareEquals);
	void configureReadWithComparison(uint64_t registerNumber,
			uint64_t registerSize, uint64_t expectedDataSize,
			const uint64_t dataCompare, bool compareEquals);
	void addDelay(uint64_t delayInUsec);
	void setRetries(uint64_t retryCount);
	uint64_t getCombinedData();
	uint64_t getCombinedRegister();
	uint64_t getDataSize();

	I2CError m_error;
	I2C_MESSAGE_TYPE m_type;
	vector<uint8_t> m_registerNumber;
	vector<uint8_t> m_data;
	vector<uint8_t> m_compareData;
	uint64_t m_retryCount;
	uint64_t m_delayInUsec;
	uint64_t m_retryDelayInUsec;
	bool m_allowOtherProcessOnIdle;
protected:
	uint64_t m_registerSize;
	uint64_t m_dataSize;

};

} /* namespace apra */

#endif /* SRC_APRA_MODELS_I2CMESSAGE_H_ */

/*
 * I2CInterface.h
 *
 *  Created on: Aug 5, 2024
 *      Author: developer
 */

#ifndef SRC_APRA_CONTROLLERS_I2CINTERFACE_H_
#define SRC_APRA_CONTROLLERS_I2CINTERFACE_H_

#include <map>
#include <string>
#include <utils/ProcessThread.h>
#include <models/I2CTransactionMessage.h>
#include "utils/I2CBus.h"
#include "utils/Mutex.h"

namespace apra
{

class I2C_Interface: public ProcessThread
{
public:
	I2C_Interface(string i2cPath, string processName, uint64_t processFpsHz,
			bool shouldPrint);
	virtual ~I2C_Interface();
	virtual void process(Message *obj);
	uint64_t registerEvent(I2C_Transaction_Message message);
	void unregisterEvent(uint64_t messageHandle);
	I2CError reSetupI2CBus();
	bool isSuccessfullSetup();
protected:
	virtual void processEvents();
	virtual void processSingleEvent();
	void processMessage(I2C_Transaction_Message *txMessage);
	void processI2CTransaction(I2C_Transaction_Message *txMessage);

	I2CError performRead(uint8_t chipNumber, I2C_Message &message);
	I2CError performCompareRead(uint8_t chipNumber, I2C_Message &message,
			bool compareEquals);
	I2CError performWrite(uint8_t chipNumber, I2C_Message &message);
	void performTransactionDelay(const uint64_t timeDelay);
	uint64_t getNormalizedDelay(int64_t largerTime, int64_t smallerTime, uint64_t timeDelay);

	string m_i2cPath;
	I2C_Bus m_i2cBus;
	map<uint64_t, I2C_Transaction_Message> m_registeredEvents;
	vector<uint64_t> m_processedEvents;
	apra::Mutex m_eventMessageLock;
	int64_t m_lastProcessedEventTs;
	bool m_setupSuccess;
	apra::Mutex m_processLock;
};

} /* namespace apra */

#endif /* SRC_APRA_CONTROLLERS_I2CINTERFACE_H_ */

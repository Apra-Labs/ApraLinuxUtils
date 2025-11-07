/*
 * I2CInterface.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: Apra Labs
 */

#include <algorithm>
#include "utils/Macro.h"
#include "utils/ScopeLock.h"
#include "controllers/I2CInterface.h"

namespace apra
{

I2C_Interface::I2C_Interface(string i2cPath, string name, uint64_t fpsHz,
		bool shouldPrint) :
		ProcessThread(name, fpsHz), m_i2cPath(i2cPath), m_i2cBus(i2cPath,
				shouldPrint), m_lastProcessedEventTs(0), m_setupSuccess(false)
{
	// TODO Auto-generated constructor stub
	I2CError i2cError = m_i2cBus.openBus();
	if (i2cError.isError())
	{
		string error = "Unable to open i2c bus " + m_i2cPath + "\n";
		error += i2cError.getMessage() + "\n";
		error += i2cError.getDebugMessage();
		throw std::invalid_argument(error);
	}
	else
	{
		m_setupSuccess = true;
	}
}

I2C_Interface::~I2C_Interface()
{
	// TODO Auto-generated destructor stub
	m_i2cBus.closeBus();
}

I2CError I2C_Interface::reSetupI2CBus()
{
	ScopeLock lock(m_processLock);
	I2CError response;
	m_i2cBus.closeBus();
	m_setupSuccess = false;
	response = m_i2cBus.openBus();
	m_setupSuccess = !response.isError();
	return response;
}

bool I2C_Interface::isSuccessfullSetup()
{
	return m_setupSuccess;
}

uint64_t I2C_Interface::registerEvent(I2C_Transaction_Message message)
{
	ScopeLock scopeLock(m_eventMessageLock);
	m_registeredEvents[message.getHandle()] = message;
	return message.getHandle();
}

void I2C_Interface::unregisterEvent(uint64_t messageHandle)
{
	ScopeLock scopeLock(m_eventMessageLock);
	m_registeredEvents.erase(messageHandle);
}

void I2C_Interface::process(Message *obj)
{
	if (!m_setupSuccess)
	{
		return;
	}
	processEvents();
	if (!obj)
	{
		return;
	}
	I2C_Transaction_Message *txMessage = (I2C_Transaction_Message*) obj;
	processMessage(txMessage);
	std::queue<I2C_Transaction_Message*> requests;
	{
		ScopeLock lock(m_requestLock);
		while (!m_requestQueue.empty())
		{
			Message *item = m_requestQueue.front();
			m_requestQueue.pop();
			if (item)
			{
				requests.push((I2C_Transaction_Message*) item);
			}
		}
	}
	while (!requests.empty())
	{
		I2C_Transaction_Message *item = requests.front();
		requests.pop();
		if (item)
		{
			processMessage(item);
		}
	}
}

void I2C_Interface::processEvents()
{
	MONOCURRTIME(timeNow);
	if ((timeNow - m_lastProcessedEventTs) < m_frequSec)
	{
		return;
	}
	map<uint64_t, I2C_Transaction_Message> eventMessages;
	{
		ScopeLock lock(m_eventMessageLock);
		eventMessages = m_registeredEvents;
	}
	bool isAnyEventProcessed = false;
	for (map<uint64_t, I2C_Transaction_Message>::iterator eventItr =
			eventMessages.begin(); eventItr != eventMessages.end(); eventItr++)
	{
		if (std::find(m_processedEvents.begin(), m_processedEvents.end(),
				eventItr->first) != m_processedEvents.end())
		{
			continue;
		}
		isAnyEventProcessed = true;
		I2C_Transaction_Message i2cTxMessage = eventItr->second;
		processI2CTransaction(&i2cTxMessage);
		i2cTxMessage.publishTransaction();
	}

	m_processedEvents.clear();
	if (isAnyEventProcessed)
	{
		MONOTIMEUS(m_lastProcessedEventTs);
	}
}

void I2C_Interface::processSingleEvent()
{
	MONOCURRTIME(timeNow);
	if ((timeNow - m_lastProcessedEventTs) < m_frequSec)
	{
		return;
	}
	map<uint64_t, I2C_Transaction_Message> eventMessages;
	{
		ScopeLock lock(m_eventMessageLock);
		eventMessages = m_registeredEvents;
	}
	for (map<uint64_t, I2C_Transaction_Message>::iterator eventItr =
			eventMessages.begin(); eventItr != eventMessages.end(); eventItr++)
	{
		if (std::find(m_processedEvents.begin(), m_processedEvents.end(),
				eventItr->first) != m_processedEvents.end())
		{
			continue;
		}
		I2C_Transaction_Message i2cTxMessage = eventItr->second;
		processI2CTransaction(&i2cTxMessage);
		i2cTxMessage.publishTransaction();
		m_processedEvents.push_back(eventItr->first);
		break;
	}
	if (m_processedEvents.size() >= m_registeredEvents.size())
	{
		m_processedEvents.clear();
		MONOTIMEUS(m_lastProcessedEventTs);
	}
}

void I2C_Interface::processMessage(I2C_Transaction_Message *txMessage)
{
	processI2CTransaction(txMessage);
	enqueResponse(txMessage);
	if (txMessage->m_transactionDelayUsec > 0)
	{
		MONOCURRTIME(timeNow);
		uint64_t timeDelay = txMessage->m_transactionDelayUsec;
		if ((timeNow - m_lastProcessedEventTs) > m_frequSec)
		{
			processEvents();
			timeDelay = getNormalizedDelay(m_lastProcessedEventTs, timeNow,
					txMessage->m_transactionDelayUsec);
		}
		if (timeDelay)
		{
			usleep(timeDelay);
		}
	}
	else
	{
		processEvents();
	}
}

I2CError I2C_Interface::performRead(uint8_t chipNumber, I2C_Message &message)
{
	I2CError response;
	uint64_t retryCount = message.m_retryCount;
	do
	{
		if (retryCount != message.m_retryCount)
		{
			if (message.m_allowOtherProcessOnIdle)
			{
				performTransactionDelay(message.m_retryDelayInUsec);
			}
			else if (message.m_retryDelayInUsec)
			{
				usleep(message.m_retryDelayInUsec);
			}
		}
		{
			ScopeLock lock(m_processLock);
			m_i2cBus.setSize(message.m_registerNumber.size(),
					message.getDataSize());
			response = m_i2cBus.genericRead(chipNumber,
					message.m_registerNumber, message.m_data);
		}
		if (!response.isError())
		{
			break;
		}
	} while (retryCount-- > 0);
	message.m_error = response;
	return response;
}

I2CError I2C_Interface::performCompareRead(uint8_t chipNumber,
		I2C_Message &message, bool compareEquals)
{
	I2CError response;
	uint64_t retryCount = message.m_retryCount;
	do
	{
		if (retryCount != message.m_retryCount)
		{
			if (message.m_allowOtherProcessOnIdle)
			{
				performTransactionDelay(message.m_retryDelayInUsec);
			}
			else if (message.m_retryDelayInUsec)
			{
				usleep(message.m_retryDelayInUsec);
			}
		}
		{
			ScopeLock lock(m_processLock);
			m_i2cBus.setSize(message.m_registerNumber.size(),
					message.getDataSize());
			response = m_i2cBus.genericRead(chipNumber,
					message.m_registerNumber, message.m_data);
		}
		if (!response.isError())
		{
			if (compareEquals
					&& (message.m_data.size() == message.m_compareData.size())
					&& (message.m_data == message.m_compareData))
			{
				break;
			}
			else if (!compareEquals
					&& ((message.m_data.size() != message.m_compareData.size())
							|| (message.m_data == message.m_compareData)))
			{
				break;
			}
		}
	} while (retryCount-- > 0);
	message.m_error = response;
	return response;
}

I2CError I2C_Interface::performWrite(uint8_t chipNumber, I2C_Message &message)
{
	I2CError response;
	uint64_t retryCount = message.m_retryCount;
	do
	{
		if (retryCount != message.m_retryCount)
		{
			if (message.m_allowOtherProcessOnIdle)
			{
				performTransactionDelay(message.m_retryDelayInUsec);
			}
			else if (message.m_retryDelayInUsec)
			{
				usleep(message.m_retryDelayInUsec);
			}
		}
		{
			ScopeLock lock(m_processLock);
			m_i2cBus.setSize(message.m_registerNumber.size(),
								message.m_data.size());
			response = m_i2cBus.genericWrite(chipNumber,
					message.m_registerNumber, message.m_data);
		}
		if (!response.isError())
		{
			break;
		}
	} while (retryCount-- > 0);
	message.m_error = response;
	return response;
}

void I2C_Interface::processI2CTransaction(I2C_Transaction_Message *txMessage)
{
	I2CError transactionError;
	for (size_t messageIndex = 0; messageIndex < txMessage->m_messages.size();
			messageIndex++)
	{
		I2CError i2cError;
		switch (txMessage->m_messages[messageIndex].m_type)
		{
		case I2C_READ:
		{
			i2cError = performRead(txMessage->m_chipNumber,
					txMessage->m_messages[messageIndex]);
			break;
		}
		case I2C_READ_COMPARE_EQUAL:
		case I2C_READ_COMPARE_NOT_EQUAL:
		{
			i2cError = performCompareRead(txMessage->m_chipNumber,
					txMessage->m_messages[messageIndex],
					txMessage->m_messages[messageIndex].m_type
							== I2C_READ_COMPARE_EQUAL);
			break;
		}
		case I2C_WRITE:
		{
			i2cError = performWrite(txMessage->m_chipNumber,
					txMessage->m_messages[messageIndex]);
			break;
		}
		}
		if (i2cError.isError())
		{
			transactionError = i2cError;
			if (txMessage->m_stopOnAnyTransactionFailure)
			{
				break;
			}
		}
		if (txMessage->m_messages[messageIndex].m_delayInUsec)
		{
			if (txMessage->m_messages[messageIndex].m_allowOtherProcessOnIdle)
			{
				performTransactionDelay(
						txMessage->m_messages[messageIndex].m_delayInUsec);
			}
			else
			{
				usleep(txMessage->m_messages[messageIndex].m_delayInUsec);
			}
		}
	}
	txMessage->setError(transactionError);
}

void I2C_Interface::performTransactionDelay(const uint64_t timeDelay)
{
	if (!timeDelay)
	{
		return;
	}
	MONOCURRTIME(startTime);
	int64_t timeNow = startTime;
	uint64_t pendingEvents = m_registeredEvents.size()
			- m_processedEvents.size();

	uint64_t delayInUsec = getNormalizedDelay(timeNow, startTime, timeDelay);
	while ((delayInUsec > 0) && (pendingEvents-- > 0))
	{
		processSingleEvent();
		MONOTIMEUS(timeNow);
		delayInUsec = getNormalizedDelay(timeNow, startTime, timeDelay);
	}
	if (delayInUsec > 0)
	{
		usleep(delayInUsec);
	}
}

uint64_t I2C_Interface::getNormalizedDelay(int64_t largerTime,
		int64_t smallerTime, uint64_t timeDelay)
{
	int64_t signedDiff = largerTime - smallerTime;
	uint64_t timeDiff = signedDiff > -1 ? signedDiff : 0;
	return (timeDiff < timeDelay) ? (timeDelay - timeDiff) : 0;
}

} /* namespace apra */


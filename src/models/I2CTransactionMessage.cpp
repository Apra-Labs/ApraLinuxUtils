/*
 * I2CTransactionMessage.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: developer
 */

#include <models/I2CTransactionMessage.h>

namespace apra
{

I2C_Transaction_Message::I2C_Transaction_Message() :
		Message(), m_error(), m_chipNumber(0), m_stopOnAnyTransactionFailure(
				true), m_transactionDelayUsec(0), m_messages(), m_callbackContext(
		NULL), m_callback(NULL)
{

}
I2C_Transaction_Message::I2C_Transaction_Message(uint8_t chipNumber,
		vector<I2C_Message> messageQueue, uint64_t transactionDelayUsec) :
		Message(), m_error(), m_chipNumber(chipNumber), m_stopOnAnyTransactionFailure(
				true), m_transactionDelayUsec(transactionDelayUsec), m_messages(
				messageQueue), m_callbackContext(NULL), m_callback(NULL)
{
	// TODO Auto-generated constructor stub
	setType(REQUEST_RESPONSE);
}

I2C_Transaction_Message::~I2C_Transaction_Message()
{
	// TODO Auto-generated destructor stub
}

I2C_Transaction_Message& I2C_Transaction_Message::operator=(
		const I2C_Transaction_Message &other)
{
	m_error = other.m_error;
	m_chipNumber = other.m_chipNumber;
	m_stopOnAnyTransactionFailure = other.m_stopOnAnyTransactionFailure;
	m_transactionDelayUsec = other.m_transactionDelayUsec;
	m_messages = other.m_messages;
	m_callbackContext = other.m_callbackContext;
	m_callback = other.m_callback;
	return *this;
}

I2CError I2C_Transaction_Message::getError()
{
	return m_error;
}

void I2C_Transaction_Message::setError(I2CError error)
{
	m_error = error;
}

vector<I2C_Message>& I2C_Transaction_Message::getAllMessages()
{
	return m_messages;
}

void I2C_Transaction_Message::registerEventHandle(I2CEventCallback *callback,
		void *context)
{
	m_callback = callback;
	m_callbackContext = context;
}

void I2C_Transaction_Message::publishTransaction()
{
	if (m_callback)
	{
		(*m_callback)(m_callbackContext, *this);
	}
}

} /* namespace apra */

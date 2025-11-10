/*
 * I2CTransactionMessage.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <models/I2CTransactionMessage.h>
#include "utils/Macro.h"
#include <iostream>
#include <inttypes.h>
#include <stdio.h>

namespace apra
{

I2C_Transaction_Message::I2C_Transaction_Message() :
		Message(), m_error(), m_chipNumber(0), m_stopOnAnyTransactionFailure(
				true), m_transactionDelayUsec(0), m_messages(), m_callbackContext(
		NULL), m_callback(NULL)
{
	setType(REQUEST_RESPONSE);
}
I2C_Transaction_Message::I2C_Transaction_Message(uint16_t chipNumber,
		vector<I2C_Message> messageQueue, uint64_t transactionDelayUsec) :
		Message(), m_error(), m_chipNumber(chipNumber), m_stopOnAnyTransactionFailure(
				true), m_transactionDelayUsec(transactionDelayUsec), m_messages(
				messageQueue), m_callbackContext(NULL), m_callback(NULL)
{
	setType(REQUEST_RESPONSE);
}

I2C_Transaction_Message::~I2C_Transaction_Message()
{
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
	m_handle = other.m_handle;
	m_type = other.m_type;
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

void I2C_Transaction_Message::registerEventHandle(void *callback, void *context)
{
	m_callback = (I2CEventCallback *) (callback);
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

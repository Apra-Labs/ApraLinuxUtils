/*
 * GenericError.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <models/GenericError.h>
using namespace apra;

GenericError::GenericError() :
		m_message(), m_debugMessage(), m_isError(false)
{
}

GenericError::GenericError(std::string message) :
		m_message(message), m_debugMessage(), m_isError(true)
{

}

GenericError::GenericError(std::string message, std::string debugMessage) :
		m_message(message), m_debugMessage(debugMessage), m_isError(true)
{

}

GenericError::~GenericError()
{
}

bool GenericError::isError()
{
	return m_isError;
}

std::string GenericError::getMessage()
{
	return m_message;
}

std::string GenericError::getDebugMessage()
{
	return m_debugMessage;
}

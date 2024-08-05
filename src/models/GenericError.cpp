/*
 * GenericError.cpp
 *
 *  Created on: 02-Mar-2023
 *      Author: developer
 */

#include <models/GenericError.h>
using namespace apra;

GenericError::GenericError() :
		m_message(), m_debugMessage(), m_isError(false)
{
	// TODO Auto-generated constructor stub

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
	// TODO Auto-generated destructor stub
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

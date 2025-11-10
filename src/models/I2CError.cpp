/*
 * I2CErrorCode.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <models/I2CError.h>
#include <models/GenericError.h>
namespace apra
{

I2CError::I2CError() :
		GenericError(), m_code(NO_ERROR)
{
}

I2CError::I2CError(std::string message) :
		GenericError(message), m_code(NO_ERROR)
{

}

I2CError::I2CError(std::string message, I2C_ERROR_CODE code) :
		GenericError(message), m_code(code)
{
}

I2CError::I2CError(I2C_ERROR_CODE code) :
		GenericError(""), m_code(code)
{
}

I2CError::I2CError(std::string message, std::string debugMessage) :
		GenericError(message, debugMessage), m_code(NO_ERROR)
{

}
I2CError::I2CError(std::string message, std::string debugMessage,
		I2C_ERROR_CODE code) :
		GenericError(message, debugMessage), m_code(code)
{

}
I2CError::I2CError(I2C_ERROR_CODE code, std::string debugMessage) :
		GenericError("", debugMessage), m_code(code)
{

}

I2CError& I2CError::operator=(I2CError other)
{
	m_code = other.getCode();
	m_debugMessage = other.getDebugMessage();
	m_isError = other.isError();
	m_message = other.getMessage();
	return *this;
}

I2C_ERROR_CODE I2CError::getCode()
{
	return m_code;
}

I2CError::~I2CError()
{
	// TODO Auto-generated destructor stub
}

} /* namespace apra */

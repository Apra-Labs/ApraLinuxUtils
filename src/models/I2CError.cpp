/*
 * I2CErrorCode.cpp
 *
 *  Created on: 02-Mar-2023
 *      Author: developer
 */

#include <models/I2CError.h>
#include <models/GenericError.h>
namespace apra
{

I2CError::I2CError() :
		GenericError(), m_code(NO_ERROR)
{
	// TODO Auto-generated constructor stub

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

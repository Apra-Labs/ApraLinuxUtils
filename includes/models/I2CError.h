/*
 * I2CErrorCode.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_MODELS_I2CERROR_H_
#define INCLUDES_APRA_MODELS_I2CERROR_H_

#include <models/GenericError.h>

namespace apra
{

enum I2C_ERROR_CODE
{
	NO_ERROR, OPEN_BUS_ERROR, WRITE_ERROR, READ_ERROR, BUS_UNOPENED
};

class I2CError: public GenericError
{
public:
	I2CError();
	I2CError(std::string message);
	I2CError(std::string message, I2C_ERROR_CODE code);
	I2CError(I2C_ERROR_CODE code);
	I2CError(std::string message, std::string debugMessage);
	I2CError(std::string message, std::string debugMessage,
			I2C_ERROR_CODE code);
	I2CError(I2C_ERROR_CODE code, std::string debugMessage);
	virtual ~I2CError();
	I2CError& operator=(I2CError other);
	I2C_ERROR_CODE getCode();
protected:
	I2C_ERROR_CODE m_code;
};

} /* namespace apra */

#endif /* INCLUDES_APRA_MODELS_I2CERROR_H_ */

/*
 * GenericError.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_GENERICERROR_H_
#define INCLUDES_APRA_GENERICERROR_H_
#include <string>
#include <vector>

namespace apra
{

class GenericError
{
public:
	GenericError();
	GenericError(std::string message);
	GenericError(std::string message, std::string debugMessages);
	virtual ~GenericError();
	bool isError();
	std::string getMessage();
	std::string getDebugMessage();
protected:
	std::string m_message;
	std::string m_debugMessage;
	bool m_isError;
};

}  // namespace apra

#endif /* INCLUDES_APRA_GENERICERROR_H_ */

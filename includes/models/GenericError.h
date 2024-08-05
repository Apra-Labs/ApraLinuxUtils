/*
 * GenericError.h
 *
 *  Created on: 02-Mar-2023
 *      Author: developer
 */

#ifndef INCLUDES_GENERICERROR_H_
#define INCLUDES_GENERICERROR_H_
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

#endif /* INCLUDES_GENERICERROR_H_ */

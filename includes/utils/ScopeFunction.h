/*
 * ScopeFunction.h
 *
 *  Created on: 15-Feb-2023
 *      Author: developer
 */

#ifndef INCLUDES_SCOPEFUNCTION_H_
#define INCLUDES_SCOPEFUNCTION_H_
#include <string>

namespace apra
{
class ScopeFunction
{
public:
	ScopeFunction(std::string fName);
	virtual ~ScopeFunction();
private:
	std::string m_functionName;
};
}

#endif /* INCLUDES_SCOPEFUNCTION_H_ */

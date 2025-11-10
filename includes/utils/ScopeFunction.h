/*
 * ScopeFunction.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_SCOPEFUNCTION_H_
#define INCLUDES_APRA_SCOPEFUNCTION_H_
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

#endif /* INCLUDES_APRA_SCOPEFUNCTION_H_ */

/*
 * ScopeFunction.cpp
 *
 *  Created on: 15-Feb-2023
 *      Author: developer
 */

#include <utils/ScopeFunction.h>
using namespace apra;
ScopeFunction::ScopeFunction(std::string fName) :
		m_functionName(fName)
{
	// TODO Auto-generated constructor stub
	printf("%s::in\n", m_functionName.c_str());
}

ScopeFunction::~ScopeFunction()
{
	// TODO Auto-generated destructor stub
	printf("%s::out\n", m_functionName.c_str());
}


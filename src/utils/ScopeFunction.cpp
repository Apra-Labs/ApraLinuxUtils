/*
 * ScopeFunction.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <utils/ScopeFunction.h>
using namespace apra;
ScopeFunction::ScopeFunction(std::string fName) :
		m_functionName(fName)
{
	printf("%s::in\n", m_functionName.c_str());
}

ScopeFunction::~ScopeFunction()
{
	printf("%s::out\n", m_functionName.c_str());
}


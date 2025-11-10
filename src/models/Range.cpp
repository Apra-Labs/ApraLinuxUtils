/*
 * Range.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <models/Range.h>
using namespace apra;

Range::Range() :
		m_min(0), m_max(0), m_isReversed(false)
{
}

Range::Range(int64_t min, int64_t max, bool isReversed) :
		m_min(min), m_max(max), m_isReversed(isReversed)
{

}

Range::~Range()
{
}
Range Range::operator=(const Range &t)
{
	m_min = t.m_min;
	m_max = t.m_max;
	return *this;
}


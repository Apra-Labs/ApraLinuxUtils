/*
 * Range.cpp
 *
 *  Created on: 02-Mar-2023
 *      Author: developer
 */

#include <models/Range.h>
using namespace apra;

Range::Range() :
		m_min(0), m_max(0), m_isReversed(false)
{
	// TODO Auto-generated constructor stub

}

Range::Range(int64_t min, int64_t max, bool isReversed) :
		m_min(min), m_max(max), m_isReversed(isReversed)
{

}

Range::~Range()
{
	// TODO Auto-generated destructor stub
}


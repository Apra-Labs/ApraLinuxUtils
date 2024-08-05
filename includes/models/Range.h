/*
 * Range.h
 *
 *  Created on: 02-Mar-2023
 *      Author: developer
 */

#ifndef INCLUDES_MODELS_RANGE_H_
#define INCLUDES_MODELS_RANGE_H_
#include <stdint.h>

namespace apra
{

class Range
{
public:
	Range();
	Range(int64_t min, int64_t max, bool isReversed = false);
	virtual ~Range();
	int64_t m_min;
	int64_t m_max;
	bool m_isReversed;
};

}  // namespace apra

#endif /* INCLUDES_MODELS_RANGE_H_ */

/*
 * Range.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_MODELS_RANGE_H_
#define INCLUDES_APRA_MODELS_RANGE_H_
#include <stdint.h>

namespace apra
{

class Range
{
public:
	Range();
	Range(int64_t min, int64_t max, bool isReversed = false);
	virtual ~Range();
	Range operator=(const Range &t);
	int64_t m_min;
	int64_t m_max;
	bool m_isReversed;
};

}  // namespace apra

#endif /* INCLUDES_APRA_MODELS_RANGE_H_ */

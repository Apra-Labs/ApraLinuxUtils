/*
 * RealHexParser.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_REALHEXPARSER_H_
#define INCLUDES_APRA_REALHEXPARSER_H_
#include <stdint.h>
namespace apra
{
class RealHexParser
{
public:
	RealHexParser(uint8_t realPrecisionDigits);
	virtual ~RealHexParser();
	uint32_t toHex(double realNumber);
	double toReal(uint32_t hexNumber);
private:
	uint8_t m_readDigits;
	uint32_t m_bitsMask;
};
}

#endif /* INCLUDES_APRA_REALHEXPARSER_H_ */

/*
 * RealHexParser.h
 *
 *  Created on: 29-Mar-2022
 *      Author: developer
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

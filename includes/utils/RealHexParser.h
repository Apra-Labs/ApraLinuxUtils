/*
 * RealHexParser.h
 *
 *  Created on: 29-Mar-2022
 *      Author: developer
 */

#ifndef REALHEXPARSER_H_
#define REALHEXPARSER_H_
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

#endif /* REALHEXPARSER_H_ */

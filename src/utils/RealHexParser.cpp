/*
 * RealHexParser.cpp
 *
 *  Created on: 29-Mar-2022
 *      Author: developer
 */

#include "utils/RealHexParser.h"
using namespace apra;

RealHexParser::RealHexParser(uint8_t realPrecisionDigits) :
		m_readDigits(realPrecisionDigits), m_bitsMask(0) {
	// TODO Auto-generated constructor stub
	if (realPrecisionDigits > 17) {
		throw "Invalid value. Supported realDigits is upto 17. Ex 15.17";
	}
	for (uint32_t i = 0; i < realPrecisionDigits; i++) {
		m_bitsMask = (m_bitsMask << 1) | 0x1;
	}
}

RealHexParser::~RealHexParser() {
	// TODO Auto-generated destructor stub
}

uint32_t RealHexParser::toHex(double realNumber) {
	uint32_t decimalVal = realNumber;
	uint32_t integerVal = realNumber;
	realNumber = realNumber - integerVal;
	for (int16_t i = 0; i < m_readDigits; i++) {
		realNumber *= 2;
		integerVal = realNumber;
		if (integerVal == 1) {
			realNumber = realNumber - integerVal;
			decimalVal = (decimalVal << 1) + 1;
		} else {
			decimalVal = decimalVal << 1;
		}
	}
	return decimalVal;
}

double RealHexParser::toReal(uint32_t hexNumber) {
	double decimalVal = 0;
	uint16_t fractionValue = 0;
	decimalVal = hexNumber >> m_readDigits;
	fractionValue = hexNumber & m_bitsMask;
	double temp = 0.00;
	double twos = 2.00;
	for (int i = (m_readDigits-1); i >= 0; i--) {
		temp += (fractionValue >> i & 0x1) / twos;
		twos = twos * 2;
	}
	return (decimalVal + temp);
}

/*
 * I2CMessageType.h
 *
 *  Created on: Aug 5, 2024
 *      Author: developer
 */

#ifndef INCLUDES_APRA_CONSTANTS_I2CMESSAGETYPE_H_
#define INCLUDES_APRA_CONSTANTS_I2CMESSAGETYPE_H_

namespace apra
{

enum I2C_MESSAGE_TYPE
{
	I2C_WRITE, I2C_READ, I2C_READ_COMPARE_EQUAL, I2C_READ_COMPARE_NOT_EQUAL
};

} /* namespace apra */

#endif /* INCLUDES_APRA_CONSTANTS_I2CMESSAGETYPE_H_ */

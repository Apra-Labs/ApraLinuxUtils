/*
 * EventCallbackI2C.h
 *
 *  Created on: Aug 7, 2024
 *      Author: developer
 */

#ifndef INCLUDES_APRA_CALLBACK_EVENTCALLBACKS_H_
#define INCLUDES_APRA_CALLBACK_EVENTCALLBACKS_H_

namespace apra
{

class I2C_Transaction_Message;

}  // namespace apra

typedef void* I2CEventCallback(void *context, apra::I2C_Transaction_Message message);

#endif /* INCLUDES_CALLBACK_EVENTCALLBACKS_H_ */

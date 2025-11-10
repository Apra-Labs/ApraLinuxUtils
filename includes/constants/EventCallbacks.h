/*
 * EventCallbackI2C.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_CALLBACK_EVENTCALLBACKS_H_
#define INCLUDES_APRA_CALLBACK_EVENTCALLBACKS_H_

namespace apra
{

class I2C_Transaction_Message;

}  // namespace apra

typedef void* I2CEventCallback(void *context,
		apra::I2C_Transaction_Message message);

#endif /* INCLUDES_CALLBACK_EVENTCALLBACKS_H_ */

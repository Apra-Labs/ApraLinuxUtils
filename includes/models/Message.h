/*
 * Message.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_MESSAGE_H_
#define INCLUDES_APRA_MESSAGE_H_

#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include "constants/MessageType.h"

using namespace std;
namespace apra
{

class Message
{
public:
	Message();
	virtual ~Message();
	void setType(MESSAGE_TYPE t);
	MESSAGE_TYPE getType();
	uint64_t getHandle();
protected:
	MESSAGE_TYPE m_type;
	uint64_t m_handle;
};
}

#endif /* INCLUDES_APRA_MESSAGE_H_ */


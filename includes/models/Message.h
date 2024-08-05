/*
 * Message.h
 *
 *  Created on: Dec 19, 2018
 *      Author: developer
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

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

#endif /* MESSAGE_H_ */


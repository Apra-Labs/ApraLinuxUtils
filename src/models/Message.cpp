/*
 * Message.cpp
 *
 *  Created on: Dec 19, 2018
 *      Author: developer
 */

#include "models/Message.h"
#include "utils/Macro.h"
namespace apra
{
Message::Message() :
		m_type(NO_TYPE)
{
	GTMONOTIMENS(m_handle)
}

Message::~Message()
{

}

void Message::setType(MESSAGE_TYPE t)
{
	m_type = t;
}

MESSAGE_TYPE Message::getType()
{
	return m_type;
}

}

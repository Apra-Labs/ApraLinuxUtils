/*
 * Message.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include "models/Message.h"
#include "utils/Macro.h"
namespace apra
{
Message::Message() :
		m_type(REQUEST_ONLY)
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

uint64_t Message::getHandle()
{
	return m_handle;
}
}

/*
 * I2CTransactionMessage.h
 *
 *  Created on: Aug 5, 2024
 *      Author: developer
 */

#ifndef INCLUDES_MODELS_I2CTRANSACTIONMESSAGE_H_
#define INCLUDES_MODELS_I2CTRANSACTIONMESSAGE_H_

#include <models/Message.h>
#include <models/I2CMessage.h>
#include <models/I2CError.h>
#include "constants/EventCallbacks.h"

namespace apra
{
class I2C_Transaction_Message: public Message
{
public:
	I2C_Transaction_Message();
	I2C_Transaction_Message(uint8_t chipNumber,
			vector<I2C_Message> messageQueue,
			uint64_t transactionDelayUsec = 0);
	virtual ~I2C_Transaction_Message();
	I2C_Transaction_Message& operator=(const I2C_Transaction_Message &other);
	I2CError getError();
	void setError(I2CError error);
	vector<I2C_Message>& getAllMessages();
	void registerEventHandle(I2CEventCallback *callback, void *context);
	void publishTransaction();
	I2CError m_error;
	uint8_t m_chipNumber;
	bool m_stopOnAnyTransactionFailure;
	uint64_t m_transactionDelayUsec;
	vector<I2C_Message> m_messages;
protected:
	void *m_callbackContext;
	I2CEventCallback *m_callback;
};

} /* namespace apra */

#endif /* INCLUDES_MODELS_I2CTRANSACTIONMESSAGE_H_ */

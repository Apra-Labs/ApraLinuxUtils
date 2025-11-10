/*
 * ProcessThread.h
 *
 *  Created on: Dec 19, 2018
 *      Author: developer
 */

#ifndef INCLUDES_APRA_PROCESSTHREAD_H_
#define INCLUDES_APRA_PROCESSTHREAD_H_

#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string>
#include <queue>

#include "models/Message.h"
#include "utils/Mutex.h"
#include "constants/ThreadType.h"

using namespace std;

namespace apra
{

class ProcessThread
{
public:
	ProcessThread(string name, int64_t freq = 0);
	virtual ~ProcessThread();
	void setType(THREAD_TYPE t);
	int32_t begin();
	virtual int32_t end();
	virtual void process(Message *obj)=0;
	bool shouldIquit();
	string getName();
	void enque(Message *p);
	THREAD_TYPE getType();
	void setFPS(int64_t fps);
	Message* dequeue();

protected:
	int32_t mainLoop();
	static void* beginProxy(void *arg);
	void someFunction(bool &executedOnce);
	void enqueResponse(Message *message);
	void trimQueue(std::queue<Message*> &queue);
	string m_threadname;
	pthread_t m_threadID;
	int64_t m_frequSec;
	std::queue<Message*> m_requestQueue;
	std::queue<Message*> m_responseQueue;
	THREAD_TYPE m_typeofThread;
	Mutex m_requestLock;
	Mutex m_responseLock;
	bool m_shouldIquit;
	uint32_t m_queueSizeLimit;
};
}

#endif /* INCLUDES_APRA_PROCESSTHREAD_H_ */

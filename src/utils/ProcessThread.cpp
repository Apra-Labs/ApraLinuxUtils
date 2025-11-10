/*
 * ProcessThread.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */
#include "utils/ProcessThread.h"

#include <stdio.h>
#include <iostream>
#include <exception>
#include "utils/Macro.h"
#include "utils/ScopeLock.h"

namespace apra
{

ProcessThread::ProcessThread(string name, int64_t freq) :
		m_threadname(name), m_frequSec(0), m_requestQueue(), m_responseQueue(), m_typeofThread(
				FREERUNNING), m_requestLock(), m_responseLock(), m_shouldIquit(
				false), m_queueSizeLimit(1000)
{
	setFPS(freq);
}

ProcessThread::~ProcessThread()
{
	printf("END******************************************%32s \n",
			getName().c_str());

}

void ProcessThread::setFPS(int64_t fps)
{
	if (fps > 0)
	{
		m_frequSec = 1000000.0 / fps;
	}
}

bool ProcessThread::shouldIquit()
{
	return m_shouldIquit;
}
string ProcessThread::getName()
{
	return m_threadname;
}

void ProcessThread::setType(THREAD_TYPE t)
{
	m_typeofThread = t;
}

THREAD_TYPE ProcessThread::getType(void)
{
	return m_typeofThread;
}

void* ProcessThread::beginProxy(void *arg)
{
	uint32_t status = 0;
	ProcessThread *pHThread = (ProcessThread*) arg;
	if (pHThread)
	{
		uint32_t tid = syscall(SYS_gettid);
		printf("*********************************************%32s::%lu::%u\n",
				pHThread->getName().c_str(), pHThread->m_threadID, tid);
		status = pHThread->mainLoop();
	}
	return (void*) status;
}

int32_t ProcessThread::begin()
{
	m_shouldIquit = true;
	int32_t m_Error = pthread_create(&m_threadID, NULL,
			ProcessThread::beginProxy, (void*) (this));
	return m_Error;
}

int32_t ProcessThread::end()
{
	int32_t ret = -1;
	std::string name = getName() + "::";
	try
	{
		m_shouldIquit = false;
		void *retVal = NULL;
		usleep(200000);
		ret = pthread_join(m_threadID, &retVal);
	} catch (std::exception &ex)
	{
		cout << name << ex.what() << endl;
		cout << name << endl;
	} catch (const char *msg)
	{
		cout << name << msg << endl;
		cout << name << "ending thread" << endl;
	} catch (...)
	{
		cout << name << "Unknown exception! Ending thread" << endl;
	}
	return ret;
}

void ProcessThread::enque(Message *p)
{
	ScopeLock lock(m_requestLock);
	trimQueue(m_requestQueue);
	m_requestQueue.push(p);
}
int32_t ProcessThread::mainLoop()
{
	std::string name = getName() + "::";
	try
	{
		while (shouldIquit())
		{
			bool executedonce = false;
			PROCESSTIME(pt,
			{
				someFunction(executedonce)
				;
			})
			;
			if (m_frequSec > 0)
			{
				int64_t td = m_frequSec - pt;
				if (td > 0)
				{
					usleep(m_frequSec - pt);
				}
			}
		}
	} catch (std::exception &ex)
	{
		cout << name << ex.what() << endl;
		cout << name << "exiting thread" << endl;
	} catch (const char *msg)
	{
		cout << name << msg << endl;
		cout << name << "exiting thread" << endl;
	} catch (...)
	{
		cout << name << "Unknown exception! exiting thread" << endl;
	}
	printf("ENDING *********************************************%32s::%lu \n",
			getName().c_str(), m_threadID);
	return 0;
}

void ProcessThread::enqueResponse(Message *message)
{
	ScopeLock lock(m_responseLock);
	trimQueue(m_responseQueue);
	m_responseQueue.push(message);
}

void ProcessThread::someFunction(bool &executedOnce)
{
	switch (m_typeofThread)
	{
	case FREERUNNING:
		process(NULL);
		break;
	case ONLY_MESSAGE:
		executedOnce = true;
	case MESSAGE_AND_FREERUNNING:
	{
		Message *item = NULL;
		{
			ScopeLock lock(m_requestLock);
			if (m_requestQueue.size() > 0)
			{
				item = m_requestQueue.front();
				m_requestQueue.pop();
			}
		}
		if (item)
		{
			process(item);
			if (item->getType() != REQUEST_RESPONSE)
			{
				delete item;
			}
			executedOnce = true;
		}
		if (!executedOnce)
		{
			process(NULL);
		}
	}
		break;
	}
}

void ProcessThread::trimQueue(std::queue<Message*> &queue)
{
	if (queue.size() > m_queueSizeLimit)
	{
		Message *item = queue.front();
		queue.pop();
		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
	}
}

Message* ProcessThread::dequeue()
{
	ScopeLock lock(m_responseLock);
	Message *item = NULL;
	if (!m_responseQueue.empty())
	{
		item = m_responseQueue.front();
		m_responseQueue.pop();
	}
	return item;
}
}

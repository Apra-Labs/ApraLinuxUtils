#ifndef INCLUDES_APRA_SYNCHRONIZATION_H_
#define INCLUDES_APRA_SYNCHRONIZATION_H_

#include <pthread.h>
#include <vector>
#include <deque>
#include "utils/Mutex.h"

namespace apra
{
class ScopeLock
{
public:
	ScopeLock(pthread_mutex_t &mutex);
	ScopeLock(Mutex &mutex);
	~ScopeLock();
protected:
	pthread_mutex_t &m_lock;
};
}

#endif /* INCLUDES_APRA_SYNCHRONIZATION_H_ */

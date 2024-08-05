/*
 * Mutex.cpp
 *
 *  Created on: Jun 17, 2024
 *      Author: developer
 */

#include "utils/Mutex.h"

namespace apra
{

Mutex::Mutex()
{
	pthread_mutex_init(&m_mutex, NULL);
}
Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_mutex);
}
void Mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}
void Mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}
pthread_mutex_t& Mutex::get()
{
	return m_mutex;
}
} /* namespace apra */


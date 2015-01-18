/*
 * MyQueue.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef MY_QUEUE_CPP
#define MY_QUEUE_CPP

#include <pthread.h>
#include "MyQueue.h"

template<class T>
MyQueue<T>::MyQueue()
{
    sem_init(&m_sem, 0, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&m_mutex, &attr);
}

template<class T>
T MyQueue<T>::pop()
{
	sem_wait(&m_sem);
    pthread_mutex_lock(&m_mutex);
    T result = m_queue.front();
    m_queue.pop();
    pthread_mutex_unlock(&m_mutex);
    return result;
}

template<class T>
void MyQueue<T>::push(T i)
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push(i);
    sem_post(&m_sem);
    pthread_mutex_unlock(&m_mutex);
}

template<class T>
MyQueue<T>::~MyQueue()
{
	sem_destroy(&m_sem);
}
template<class T>
bool MyQueue<T>::empty() const
{
	bool result = false;
	result = m_queue.empty();
}

#endif //MY_QUEUE_CPP

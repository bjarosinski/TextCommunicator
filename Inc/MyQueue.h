/*
 * MyQueue.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <queue>
#include <semaphore.h>

template<class T>
class MyQueue
{
public:
    MyQueue();
    ~MyQueue();
    T pop();
    void push(T);
    bool empty() const;
private:
    std::queue<T> m_queue;
    sem_t m_sem;
    pthread_mutex_t m_mutex;
};

#endif //MYQUEUE_H

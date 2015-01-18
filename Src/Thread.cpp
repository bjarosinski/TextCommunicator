#include "Thread.h"

Thread::Thread()
{
	m_thread = 0;
}

void Thread::Run()
{
    pthread_create(&m_thread,NULL,Thread::run_thread,this);
}

void* Thread::run_thread(void* thread)
{
    Thread* pthread = (Thread*) thread;
    pthread -> func();
    return 0;
}

void Thread::Wait()
{
    pthread_join(m_thread,NULL);
}

void Thread::Stop()
{
	pthread_cancel(m_thread);
}

Thread::~Thread()
{

}

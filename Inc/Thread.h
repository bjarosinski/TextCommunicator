#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <stdio.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();
    virtual void func() = 0;
    void Run();
    void Wait();
    void Stop();
protected:
    static void *run_thread(void *thread);
private:
    pthread_t m_thread;
};

#endif //THREAD_H

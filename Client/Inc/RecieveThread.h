/*
 * RecieveThread.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef RECIEVETHREAD_H_
#define RECIEVETHREAD_H_

#include "Thread.h"
#include "Session.h"
#include "Socket.h"
#include "MessagesQueues.h"

class RecieveThread: public Thread {
public:
	RecieveThread(Session& session_, MessagesQueues* queues_);
	~RecieveThread();
	void func();
private:
	Session& m_session;
	MessagesQueues* m_queues;
};

#endif /* RECIEVETHREAD_H_ */

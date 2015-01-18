/*
 * SendThread.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef SENDTHREAD_H_
#define SENDTHREAD_H_

#include "Thread.h"
#include "MessagesQueues.h"
#include "Sender.h"
#include "RecieveThread.h"

class SendThread: public Thread {
public:
	SendThread(Session& session_,MessagesQueues* queues_);
	~SendThread();
	void func();
private:
	void startReceiver();
	void endReceiver();
	RecieveThread* m_receiver;
	Session& m_session;
	MessagesQueues* m_queues;
	Interface * m_interface;
	Sender m_sender;
};

#endif /* SENDTHREAD_H_ */

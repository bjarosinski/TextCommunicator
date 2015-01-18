/*
 * ReceiverRegisterThread.h
 *
 *  Created on: 09-08-2013
 *      Author: b.jarosinski
 */

#ifndef RECEIVERREGISTERTHREAD_H_
#define RECEIVERREGISTERTHREAD_H_

#include "ServerSession.h"
#include <stdio.h>
#include "Thread.h"
#include "Validator.h"

class ReceiverRegisterThread:public Thread
{
public:
	ReceiverRegisterThread(ServerSession&, int port_ = 6060);
	~ReceiverRegisterThread();
	void func();
private:
	Validator m_validator;
	ServerSession& m_session;
	int m_port;
};

#endif /* RECEIVERREGISTERTHREAD_H_ */

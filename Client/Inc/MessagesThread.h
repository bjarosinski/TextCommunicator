/*
 * MessagesThread.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef MESSAGESTHREAD_H_
#define MESSAGESTHREAD_H_

#include "Thread.h"
#include "MyQueue.h"
#include "Session.h"
#include <string>
#include "TextUserInterface.h"

class MessagesThread: public Thread
{
public:
	MessagesThread(MyQueue<std::string>* queue_,Session* session_, TextUserInterface* userInterface);
	~MessagesThread();
	void func();
	bool isRunning();
private:
	Session* m_session;
	MyQueue<std::string>* m_messageQueue;
	bool m_running;
	TextUserInterface* m_userInterface;
};

#endif /* MESSAGESTHREAD_H_ */

/*
 * ChannelThread.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef CHANNELTHREAD_H_
#define CHANNELTHREAD_H_

#include "Thread.h"
#include "MessagesQueues.h"
#include <utility>
#include "Session.h"
#include "TextUserInterface.h"

class ChannelThread :public Thread
{
public:
	ChannelThread(MyQueue<std::pair<std::string, std::string> >* queue_,
			MyQueue<std::pair<std::string, std::string> >* emoteQueue, Session* session_, TextUserInterface* userInterface_);
	~ChannelThread();
	void func();
	bool isRunning();
private:
	MyQueue<std::pair<std::string, std::string> >* m_channelQueue;
	MyQueue<std::pair<std::string, std::string> >* m_emoteQueue;
	Session* m_session;
	TextUserInterface* m_userInterface;
	bool m_running;
};

#endif /* CHANNELTHREAD_H_ */

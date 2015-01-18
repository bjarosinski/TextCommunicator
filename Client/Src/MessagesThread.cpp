/*
 * MessagesThread.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "MessagesThread.h"
#include "MyQueue.cpp"

MessagesThread::MessagesThread(MyQueue<std::string>* queue_,Session* session_,TextUserInterface* userInterface_):
									m_messageQueue(queue_),m_session(session_),m_userInterface(userInterface_)
{
	m_running = false;

}

MessagesThread::~MessagesThread()
{
	m_messageQueue = NULL;
	m_userInterface = NULL;
	m_session = NULL;
}

void MessagesThread::func()
{
	m_running = true;
	while(!m_session->isEnd())
	{
		if(m_session->isEnd())
		{
				m_running = false;
				break;
		}
		if(	!m_messageQueue -> empty())
		{
			std::string message = m_messageQueue -> pop();
			m_userInterface -> addToWindow(messageWindow_E,message);
		}
	}
}

bool MessagesThread::isRunning()
{
	return m_running;
}


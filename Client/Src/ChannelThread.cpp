/*
 * ChannelThread.cpp
 *
 *  Created on: 06-08-2013
 *      Author: SW4
 */

#include "ChannelThread.h"

ChannelThread::ChannelThread(MyQueue<std::pair<std::string, std::string> >* queue_,
		MyQueue<std::pair<std::string, std::string> >* emoteQueue_, Session* session_,TextUserInterface* userInterface_):
					m_channelQueue(queue_), m_emoteQueue(emoteQueue_),m_session(session_), m_userInterface(userInterface_)
{
	m_running = false;
}

ChannelThread::~ChannelThread()
{
	m_emoteQueue = NULL;
	m_userInterface = NULL;
	m_channelQueue = NULL;
	m_session = NULL;
}

void ChannelThread::func()
{
	m_running = true;
	while(!m_session->isEnd())
	{

		if(m_session->isEnd())
		{
			m_running = false;
			break;
		}
		if(	!m_channelQueue -> empty() )
		{
			std::pair<std::string,std::string> message_pair =  m_channelQueue -> pop();
			std::string message = message_pair.first + ": " + message_pair.second;
			m_userInterface -> addToWindow(channelWindow_E,message);
		}
		else if( !m_emoteQueue -> empty() )
		{
			std::string message = "[emote] ";
			std::pair<std::string,std::string> message_pair =  m_emoteQueue -> pop();
			message += message_pair.first + ": " + message_pair.second;
			m_userInterface -> addToWindow(channelWindow_E,message);
		}
	}
}

bool ChannelThread::isRunning()
{
	return m_running;
}

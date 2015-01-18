/*
 * RecieveThread.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "RecieveThread.h"
#include "protocol.h"
#include <sstream>
#include <cstring>

RecieveThread::RecieveThread(Session& session_,MessagesQueues* queues_):m_session(session_), m_queues(queues_)
{
}

RecieveThread::~RecieveThread()
{
	m_queues = NULL;
}

void RecieveThread::func()
{
	while( (!m_session.isConnected()) || (!m_session.isUserLoggedIn()) )
	{
		if(m_session.isEnd())
		{
			return;
		}
	}

	struct sockaddr_in result;
	PROTOCOL::RegisterReceiver request;
	PROTOCOL::ResponseInfo response;
	PROTOCOL::Message message;
	bool udpConnected = false;
	while(!m_session.isEnd())
	{
		if(m_session.isUserLoggedIn())
		{
			if(!udpConnected)
			{
				m_session.getReceivSocket().Aton(m_session.getHost());
				strcpy(request.name ,m_session.getUserName().c_str());
				strcpy(request.token,m_session.getUserToken().c_str());
				connectInfo receiver;
				receiver.ip = m_session.getHost();
				receiver.port = m_session.getReceivSocket().getPort();
				m_session.getReceivSocket().sendRequestTo(request,receiver);
				m_session.getReceivSocket().getResponseInfoFrom(response);
				udpConnected = true;
			}
			else if(m_session.getReceivSocket().tryGetMessage(message)>0)
			{
				std::string messageInfo = "";
				switch(message.type)
				{
					case PROTOCOL::Message::NORMAL:
						m_queues -> getChannelQueue() -> push(std::pair<std::string,std::string>
																		(message.from,message.content));
						break;
					case PROTOCOL::Message::EMOTE:
						m_queues -> getEmoteMessages() -> push(std::pair<std::string,std::string>
																		(message.from,message.content));
						break;
					case PROTOCOL::Message::PRIVATE:
						messageInfo += message.from;
						messageInfo += " : ";
						messageInfo += message.content;
						m_queues -> getMessages() -> push(messageInfo);
						break;
					case PROTOCOL::Message::MAIL:
						if(message.delivered)
						{
							m_queues -> getDeliveredMessages() -> push(message);
						}
						else
						{
							m_queues -> getNewMessages() -> push(message);
							messageInfo += "New mail from ";
							messageInfo += message.from;
							m_queues -> getMessages() -> push(messageInfo);
						}
						break;
				}
			}
		}
		else
		{
			udpConnected = false;
		}
	}
	m_session.getReceivSocket().Close();
}


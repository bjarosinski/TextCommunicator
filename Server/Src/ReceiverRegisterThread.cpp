/*
 * ReceiverRegisterThread.cpp
 *
 *  Created on: 09-08-2013
 *      Author: b.jarosinski
 */

#include "ReceiverRegisterThread.h"
#include <cstring>

ReceiverRegisterThread::ReceiverRegisterThread(ServerSession& session_,int port_):
				m_session(session_),m_port(port_),m_validator(session_)
{
}

ReceiverRegisterThread::~ReceiverRegisterThread()
{
}

void ReceiverRegisterThread::func()
{
	Socket udpSocket(UDP,m_port);
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( udpSocket.GetSockfd() == -1 )
	{
		perror("accept error");
		return;
	}

	if( udpSocket.Bind() == -1 )
	{
		perror("bind error");
		return;
	}
	PROTOCOL::RegisterReceiver request;
	connectInfo info;
	while(true)
	{
		int i =0;
		info = udpSocket.getRequestFrom(request,sizeof(request));
		PROTOCOL::ResponseInfo response;
		m_session.getLogger().clientUDPRegistered(request.name,info.ip,info.port);
		m_session.releaseLogger();
		response.code = PROTOCOL::OKS;
		strcpy(response.token,request.token);
		if(!(m_session.getUser(request.name) -> checkToken(request.token)))
		{
			response.code = PROTOCOL::UNAUTHORIZE;
		}
		else
		{
			response.code = PROTOCOL::OKS;
			m_session.getUser(request.name) -> setConnectInfo(info);
			m_session.getUser(request.name) -> setLoggedIn(true);
			m_session.getUser(request.name) -> setReceivSocket(udpSocket);
		}
		m_session.getLogger().clientResponse(request.name,response.code);
		m_session.releaseLogger();
		int size = udpSocket.sendResponseInfoTo(response,info);
		PROTOCOL::Message message;
		message.type = PROTOCOL::Message::EMOTE;
		std::string a = "Welcome to R2-D2 Communicator Server";
		strcpy(message.content,a.c_str());
		strcpy(message.from,"Server");
		udpSocket.sendMessage(message,info);
	}
}

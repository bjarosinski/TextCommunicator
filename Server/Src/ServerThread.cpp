/*
 * ServerThread.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "ServerThread.h"
#include <cstring>

ServerThread::ServerThread(Socket socket_,ServerSession& session_):
					m_socket(socket_),m_parser(socket_,session_)
{
}

ServerThread::~ServerThread()
{
}

void ServerThread::func()
{
	if( m_socket.GetSockfd() == -1 )
	{
		perror("accept error");
		return;
	}
	PROTOCOL::Request request;
	PROTOCOL::RequestCommand command = PROTOCOL::ACK;
	while(command!=PROTOCOL::ENDS)
	{
		if(m_socket.getRequest(request,sizeof(request))> 0)
		{
			command = request.command;
			m_parser.parseCommand(request);
		}
		else
		{
			command = PROTOCOL::ENDS;
		}
	}
	m_socket.Close();
}

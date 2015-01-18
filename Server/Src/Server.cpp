/*
 * Server.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "Server.h"

Server::Server(int port_):m_socket(TCP,port_)
{
	m_port = port_;
	m_clients = 0;
	m_receiverRegisterThread = new ReceiverRegisterThread(m_session,port_);
	m_serverStop = false;
}

Server::~Server() {
	std::vector<ServerThread*>::iterator threadsIterator = m_threads.begin();
	for( ; threadsIterator != m_threads.end(); threadsIterator++)
	{
		(*threadsIterator) -> Wait();
		delete *threadsIterator;
		*threadsIterator = NULL;
	}
	if(m_receiverRegisterThread!=NULL)
	{
		m_receiverRegisterThread -> Stop();
		delete m_receiverRegisterThread;
		m_receiverRegisterThread = NULL;
	}
}

void Server::stop()
{
	m_socket.Close();
	m_serverStop = true;
}

void Server::run()
{
	if( m_socket.GetSockfd() == -1 )
	{
		perror("socket error");
		return;
	}

	if( m_socket.Bind() == -1 )
	{
		perror("bind error");
		return;
	}

	if( m_socket.Listen(MAX_USERS_LIST_SIZE) == -1)
	{
		perror("listen error");
		return;
	}
	m_receiverRegisterThread -> Run();
	socklen_t socklen;
	int threadsCount = 0;
	std::vector<Socket> sockets;
	while(!m_serverStop)
	{
		int in_sockfd = m_socket.Accept();
		if(in_sockfd == -1)
		{
			continue;
		}
		m_session.getLogger().clientConnectLog(m_socket.getIP(),m_socket.getInPort());
		m_session.releaseLogger();
		sockets.push_back(Socket(in_sockfd,m_port));
		m_threads.push_back(new ServerThread(sockets[threadsCount],m_session));
		m_threads[threadsCount++] -> Run();
	}
}


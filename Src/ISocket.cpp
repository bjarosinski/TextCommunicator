/*
 * ISocket.cpp
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#include "ISocket.h"

#include "Socket.h"

ISocket::ISocket()
{
	t_sockfd = -1;
	t_port = -1;
	t_socklen = 0;
}

ISocket::ISocket(socket_kind kind,int port_)
{
	switch(kind)
	{
		case TCP:
			t_sockfd = socket(AF_INET,SOCK_STREAM,0);
			break;
		case UDP:
			t_sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
			break;
	}
	t_port = port_;
	t_socklen = sizeof(t_addr);
	t_addr.sin_family = AF_INET;
	t_addr.sin_port = htons(port_);
}

ISocket::ISocket(int sockfd_, int port_)
{
	t_sockfd = sockfd_;
	t_port = port_;
	t_socklen = sizeof(t_addr);
}
ISocket::~ISocket()
{
}

int ISocket::Aton(std::string host)
{
	int result = inet_aton(host.c_str(),(struct in_addr*)&t_addr.sin_addr.s_addr);
	return result;
}

int ISocket::Connect()
{
	return connect(t_sockfd, (sockaddr*)&t_addr, sizeof(t_addr));
}

int ISocket::Write(std::string message)
{
	return write(t_sockfd,message.c_str(),message.length());
}

int ISocket::Read(std::string& message,int length)
{
	char toReadBuffer[length];
	int readed = read(t_sockfd,toReadBuffer,length);
	readed = readed < length ? readed : length - 1;
	toReadBuffer[readed] = '\0';
	message = toReadBuffer;
	return readed;
}

int ISocket::Bind()
{
	t_addr.sin_addr.s_addr = INADDR_ANY;
	return bind(t_sockfd, (sockaddr*)&t_addr, t_socklen);
}

int ISocket::Listen(int max)
{
	return listen(t_sockfd, max);
}

int ISocket::Accept()
{
	t_socklen = sizeof(t_in_addr);
	int sockfd = accept(t_sockfd, (sockaddr*)&t_in_addr, &t_socklen);
	return sockfd;
}

int ISocket::GetSockfd()
{
	return t_sockfd;
}

void ISocket::Close()
{
	close(t_sockfd);
}

int ISocket::getPort()
{
	return t_port;
}

std::string ISocket::getIP()
{
	return inet_ntoa(t_in_addr.sin_addr);
}

int ISocket::getInPort()
{
	return t_in_addr.sin_port;
}

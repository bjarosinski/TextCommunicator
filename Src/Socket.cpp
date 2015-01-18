/*
 * Socket.cpp
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#include "Socket.h"

Socket::Socket()
{
}

Socket::Socket(socket_kind kind,int port_):ISocket(kind,port_)
{
}

Socket::Socket(int sockfd_, int port_):ISocket(sockfd_,port_)
{
}

Socket::~Socket()
{
}

int Socket::sendRequest(PROTOCOL::RequestInterface& request,int size_)
{
	int result;
	result = write(t_sockfd,&request,size_);
	return result;
}

int Socket::getResponseInfo(PROTOCOL::ResponseInfo& response)
{
	int result = 0;
	result = read(t_sockfd,&response,sizeof(response));
	return result;
}
int Socket::sendResponse(PROTOCOL::ResponseInterface& response,int size_)
{
	int result = 0;
	result = write(t_sockfd,&response,size_);
	return result;
}

int Socket::getResponse(PROTOCOL::ResponseInterface &response, int size_)
{
	int result = 0;
	result = read(t_sockfd, &response, size_);
	return result;
}

int Socket::getRequest(PROTOCOL::RequestInterface& request, int size_)
{
	int result = 0;
	result = read(t_sockfd, &request, size_);
	return result;
}

int Socket::sendResponseInfo(PROTOCOL::ResponseInfo& response)
{
	int result = 0;
	result = write(t_sockfd,&response,sizeof(response));
	return result;
}

connectInfo Socket::getRequestFrom(PROTOCOL::RegisterReceiver& request, int size)
{
	sockaddr_in siOther;
	connectInfo result;
	result.resultCode = 1;
	socklen_t slen = sizeof(siOther);
	int l;
	if( (l=recvfrom(t_sockfd,&request,sizeof(request),0,(sockaddr*)&siOther,&slen)) == -1 )
	{
		result.resultCode = -1;
	}
	result.ip = inet_ntoa(siOther.sin_addr);
	result.port = siOther.sin_port;
	return result;
}

int Socket::sendResponseInfoTo(PROTOCOL::ResponseInfo& responseInfo,connectInfo receiver)
{
	int result = 0;
	struct sockaddr_in siOther;
	siOther.sin_family = AF_INET;
	siOther.sin_port = receiver.port;
	if(inet_aton(receiver.ip.c_str(), &siOther.sin_addr) == -1)
	{
		result = -1;
	}
	else
	{
		socklen_t slen = sizeof(siOther);
		result = sendto(t_sockfd,&responseInfo,sizeof(responseInfo),0, (sockaddr*)&siOther,slen);
	}

	return result;
}

int Socket::sendRequestTo(PROTOCOL::RegisterReceiver& request,connectInfo receiver)
{
	int result = 0;
	struct sockaddr_in siOther;
	siOther.sin_family = AF_INET;
	siOther.sin_port = htons(receiver.port);
	if(inet_aton(receiver.ip.c_str(), &siOther.sin_addr) == -1)
	{
		perror("aton error");
		result = -1;
	}
	else
	{
		socklen_t slen = sizeof(siOther);
		sendto(t_sockfd,&request,sizeof(request),0, (sockaddr*)&siOther,slen);
	}
	return result;
}

connectInfo Socket::getResponseInfoFrom(PROTOCOL::ResponseInfo &responseInfo)
{
	connectInfo result;
	struct sockaddr_in siOther;
	result.resultCode = 1;
	siOther.sin_addr.s_addr = INADDR_ANY;
	socklen_t slen = sizeof(siOther);
	if( recvfrom(t_sockfd,&responseInfo,sizeof(responseInfo),0,(sockaddr*)&siOther,&slen) == -1 )
	{
		result.resultCode = -1;
		return result;
	}
	else
	{
		result.ip = inet_ntoa(siOther.sin_addr);
		result.port = (siOther.sin_port);
	}
	return result;
}

int Socket::BindToPort(int port_)
{
	t_addr.sin_addr.s_addr = INADDR_ANY;
	t_addr.sin_port = port_;
	return bind(t_sockfd, (sockaddr*)&t_addr, sizeof(t_addr));
}

int Socket::tryGetMessage(PROTOCOL::Message& message)
{
	struct timeval tv;

	tv.tv_sec = 1;  /* 1 Secs Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors

	setsockopt(t_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	int result = 0;
	struct sockaddr_in siOther;
	siOther.sin_addr.s_addr = INADDR_ANY;
	socklen_t slen = sizeof(siOther);
	result = recvfrom(t_sockfd,&message,sizeof(message),0,(sockaddr*)&siOther,&slen);
	return result;
}

int Socket::sendMessage(PROTOCOL::Message& message,connectInfo receiver)
{
	int result = 0;
	struct sockaddr_in siOther;
	siOther.sin_family = AF_INET;
	siOther.sin_port = receiver.port;
	if(inet_aton(receiver.ip.c_str(), &siOther.sin_addr) == -1)
	{
		perror("Aton error");
		result = -1;
	}
	else
	{
		socklen_t slen = sizeof(siOther);
		result = sendto(t_sockfd,&message,sizeof(message),0, (sockaddr*)&siOther,slen);
	}
	return result;
}

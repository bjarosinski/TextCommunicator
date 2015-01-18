/*
 * Socket.h
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "ISocket.h"

struct connectInfo
{
	int resultCode;
	std::string ip;
	int port;
};

class Socket:public ISocket
{
public:
	Socket();
	Socket(socket_kind kind ,int port = 6060);
	Socket(int sockfd_, int port = 6060);
	~Socket();
	int sendRequest(PROTOCOL::RequestInterface& request,int size_);
	int BindToPort(int port_);
	int getResponseInfo(PROTOCOL::ResponseInfo& responseInfo);
	int sendResponseInfo(PROTOCOL::ResponseInfo& responseInfo);
	int sendResponse(PROTOCOL::ResponseInterface& response,int size_);
	int getResponse(PROTOCOL::ResponseInterface& response,int size_);
	int getRequest(PROTOCOL::RequestInterface& request,int size_);
	connectInfo getRequestFrom(PROTOCOL::RegisterReceiver& request, int size);
	int sendResponseInfoTo(PROTOCOL::ResponseInfo& responseInfo,connectInfo receiver);
	connectInfo getResponseInfoFrom(PROTOCOL::ResponseInfo& responseInfo);
	int sendRequestTo(PROTOCOL::RegisterReceiver& request,connectInfo receiver);
	int tryGetMessage(PROTOCOL::Message& message);
	int sendMessage(PROTOCOL::Message& message,connectInfo receiver);
};

#endif /* SOCKET_H_ */

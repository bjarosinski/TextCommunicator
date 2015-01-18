/*
 * ISocket.h
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#ifndef ISOCKET_H_
#define ISOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "protocol.h"
#include <stdio.h>

enum socket_kind
{
	TCP,
	UDP
};

class ISocket
{
public:
	ISocket();
	ISocket(socket_kind kind ,int port = 6060);
	ISocket(int sockfd_, int port = 6060);
	int getPort();
	virtual ~ISocket();
	virtual int Aton(std::string host);
	virtual int Connect();
	virtual int Write(std::string);
	virtual int Read(std::string&,int lenght);
	virtual int Bind();
	virtual int Listen(int max);
	virtual int Accept();
	virtual int GetSockfd();
	virtual void Close();
	virtual std::string getIP();
	virtual int getInPort();
protected:
	int t_sockfd;
	sockaddr_in t_addr;
	sockaddr_in t_in_addr;
	socklen_t t_socklen;
	int t_port;
};

#endif /* ISOCKET_H_ */

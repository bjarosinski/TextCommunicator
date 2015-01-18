/*
 * Client.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "Client.h"
#include "SendThread.h"
#include "MyQueue.cpp"
#include "Interface.h"

Client::Client(std::string host_ , int port_):IClient(host_,port_)
{
}

Client::~Client()
{
}

void Client::run()
{
	MessagesQueues queues;
	bool end = false;
	bool connected = false;
	Session session(port);
	session.setHost(host);
	SendThread senderThread(session,&queues);
	senderThread.Run();
	senderThread.Wait();
}

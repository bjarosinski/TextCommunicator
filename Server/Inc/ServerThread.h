/*
 * ServerThread.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef SERVERTHREAD_H_
#define SERVERTHREAD_H_

#include "Thread.h"
#include "Socket.h"
#include "CommandParser.h"
#include "ServerSession.h"

class ServerThread: public Thread
{
public:
	ServerThread(Socket socket,ServerSession& session);
	virtual ~ServerThread();
	void func();
private:
	Socket m_socket;
	CommandParser m_parser;
};

#endif /* SERVERTHREAD_H_ */

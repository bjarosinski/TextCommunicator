/*
 * Server.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "ServerThread.h"
#include <vector>
#include "IServer.h"
#include "ServerSession.h"
#include "ReceiverRegisterThread.h"

class Server:public IServer
{
public:
	Server(int port_=6060);
	~Server();
	void run();
	void stop();
private:
	ServerSession m_session;
	std::vector<ServerThread*> m_threads;
	int m_clients;
	int m_port;
	Socket m_socket;
	ReceiverRegisterThread* m_receiverRegisterThread;
	bool m_serverStop;
};

#endif /* SERVER_H_ */

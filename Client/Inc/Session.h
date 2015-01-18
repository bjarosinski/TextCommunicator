/*
 * Session.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "protocol.h"
#include "CurrentUser.h"
#include <pthread.h>
#include "MyQueue.h"
#include "Socket.h"


class Session
{
public:
	Session(int port = 6060);
	~Session();
	bool isConnected();
	void setConnected(bool connected_);
	bool isEnd();
	void setEnd(bool end_);
	//void addRequest(request);
	//request getRequest(request)const;
	bool isEmptyRequests()const;
	CurrentUser& getUser();
	void setUserPassword(std::string password_);
	void setUserName(std::string name_);
	void setUserToken(std::string token_);
	std::string getUserName();
	std::string getUserPassword();
	std::string getUserToken();
	bool isUserLoggedIn()const;
	void setUserLoggedIn(bool loggedIn_);
	Socket& getSocket();
	void setSocket(Socket socket);
	Socket& getReceivSocket();
	void setReceivSocket(Socket socket);
	void setHost(std::string host);
	std::string getHost();
private:
	void initializeMutex();
	bool connected;
	bool end;
	std::string m_host;
	CurrentUser m_user;
	pthread_mutex_t m_connectedMutex;
	pthread_mutex_t m_endMutex;
	pthread_mutex_t m_userMutex;
	pthread_mutex_t m_socketMutex;
	pthread_mutex_t m_receivSocketMutex;
	pthread_mutex_t m_hostMutex;
	Socket m_socket;
	Socket m_receivSocket;
};

#endif /* SESSION_H_ */

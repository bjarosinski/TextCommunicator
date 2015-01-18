/*
 * Session.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "Session.h"
#include "MyQueue.cpp"

Session::Session(int port):m_socket(TCP,port),m_receivSocket(UDP,port)
{
	connected = false;
	end = false;
	initializeMutex();
}

Session::~Session()
{
}

bool Session::isConnected()
{
	bool result = false;
	pthread_mutex_lock(&m_connectedMutex);
	result =  connected;
	pthread_mutex_unlock(&m_connectedMutex);
	return result;
}

void Session::setConnected(bool connected_)
{
	pthread_mutex_lock(&m_connectedMutex);
	connected =  connected_;
	pthread_mutex_unlock(&m_connectedMutex);
}

CurrentUser& Session::getUser()
{

	pthread_mutex_lock(&m_userMutex);
	CurrentUser& result = m_user;
	pthread_mutex_unlock(&m_userMutex);
	return result;
}

void Session::setUserPassword(std::string password_)
{
	pthread_mutex_lock(&m_userMutex);
	m_user.setPassword(password_);
	pthread_mutex_unlock(&m_userMutex);
}
void Session::setUserName(std::string name_)
{
	pthread_mutex_lock(&m_userMutex);
	m_user.setName(name_);
	pthread_mutex_unlock(&m_userMutex);
}
void Session::setUserToken(std::string token_)
{
	pthread_mutex_lock(&m_userMutex);
	m_user.setToken(token_);
	pthread_mutex_unlock(&m_userMutex);
}

bool Session::isEnd()
{
	bool result;
	pthread_mutex_lock(&m_endMutex);
	result = end;
	pthread_mutex_unlock(&m_endMutex);
	return result;
}

void Session::setEnd(bool end_)
{
	pthread_mutex_lock(&m_endMutex);
	end = end_;
	pthread_mutex_unlock(&m_endMutex);
}

Socket& Session::getSocket()
{
	pthread_mutex_lock(&m_socketMutex);
	Socket& result = m_socket;
	pthread_mutex_unlock(&m_socketMutex);
	return result;
}
void Session::setSocket(Socket socket_)
{
	pthread_mutex_lock(&m_socketMutex);
	m_socket = socket_;
	pthread_mutex_unlock(&m_socketMutex);
}

Socket& Session::getReceivSocket()
{
	pthread_mutex_lock(&m_receivSocketMutex);
	Socket& result = m_receivSocket;
	pthread_mutex_unlock(&m_receivSocketMutex);
	return result;
}
void Session::setReceivSocket(Socket socket_)
{
	pthread_mutex_lock(&m_receivSocketMutex);
	m_receivSocket = socket_;
	pthread_mutex_unlock(&m_receivSocketMutex);
}

std::string Session::getHost()
{
	std::string result;
	pthread_mutex_lock(&m_hostMutex);
	result = m_host;
	pthread_mutex_unlock(&m_hostMutex);
	return result;
}


void Session::setHost(std::string host_)
{
	pthread_mutex_lock(&m_hostMutex);
	m_host = host_;
	pthread_mutex_unlock(&m_hostMutex);
}

void Session::setUserLoggedIn(bool loggedIn_)
{
	m_user.setLoggedIn(loggedIn_);
}

bool Session::isUserLoggedIn()const
{
	return m_user.isLoggedIn();
}

void Session::initializeMutex()
{
	pthread_mutexattr_t attrEnd;
	pthread_mutexattr_init(&attrEnd);
	pthread_mutex_init(&m_endMutex, &attrEnd);
	pthread_mutexattr_t attrConnected;
	pthread_mutexattr_init(&attrConnected);
	pthread_mutex_init(&m_connectedMutex, &attrConnected);
	pthread_mutexattr_t attrSocket;
	pthread_mutexattr_init(&attrSocket);
	pthread_mutex_init(&m_socketMutex, &attrSocket);
	pthread_mutexattr_t attrReceivSocket;
	pthread_mutexattr_init(&attrReceivSocket);
	pthread_mutex_init(&m_receivSocketMutex, &attrReceivSocket);
	pthread_mutexattr_t attrUser;
	pthread_mutexattr_init(&attrUser);
	pthread_mutex_init(&m_userMutex, &attrUser);
	pthread_mutexattr_t attrHost;
	pthread_mutexattr_init(&attrHost);
	pthread_mutex_init(&m_hostMutex, &attrHost);

}

std::string Session::getUserName()
{
	std::string result;
	pthread_mutex_lock(&m_userMutex);
	result = m_user.getName();
	pthread_mutex_unlock(&m_userMutex);
	return result;
}

std::string Session::getUserPassword()
{
	std::string result;
	pthread_mutex_lock(&m_userMutex);
	result = m_user.getPassword();
	pthread_mutex_unlock(&m_userMutex);
	return result;
}

std::string Session::getUserToken()
{
	std::string result;
	pthread_mutex_lock(&m_userMutex);
	result = m_user.getToken();
	pthread_mutex_unlock(&m_userMutex);
	return result;
}

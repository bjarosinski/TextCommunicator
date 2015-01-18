/*
 * CurrentUser.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "CurrentUser.h"

CurrentUser::CurrentUser()
{
	m_loggedIn = false;
	pthread_mutexattr_t nameAttr;
	pthread_mutexattr_init(&nameAttr);
	pthread_mutex_init(&m_nameMutex,&nameAttr);
	pthread_mutexattr_t passwordAttr;
	pthread_mutexattr_init(&passwordAttr);
	pthread_mutex_init(&m_passwordMutex,&passwordAttr);
	pthread_mutexattr_t loggedInAttr;
	pthread_mutexattr_init(&loggedInAttr);
	pthread_mutex_init(&m_loggedInMutex,&loggedInAttr);
	pthread_mutexattr_t tokenAttr;
	pthread_mutexattr_init(&tokenAttr);
	pthread_mutex_init(&m_tokenMutex,&tokenAttr);
	m_mode = NORMAL_MODE;

}

CurrentUser::~CurrentUser()
{
}

std::string CurrentUser::getName()const
{
	return m_name;
}

std::string CurrentUser::getPassword()const
{
	return m_password;
}

std::string CurrentUser::getToken()const
{
	return m_token;
}

void CurrentUser::setPassword(std::string password_)
{
	pthread_mutex_lock(&m_passwordMutex);
	m_password = password_;
	pthread_mutex_unlock(&m_passwordMutex);
}

void CurrentUser::setName(std::string name_)
{
	pthread_mutex_lock(&m_nameMutex);
	m_name = name_;
	pthread_mutex_unlock(&m_nameMutex);
}

void CurrentUser::setToken(std::string token_)
{
	pthread_mutex_lock(&m_tokenMutex);
	m_token = token_;
	pthread_mutex_unlock(&m_tokenMutex);
}

bool CurrentUser::isLoggedIn()const
{
	return m_loggedIn;
}

void CurrentUser::setLoggedIn(bool loggedIn_)
{
	pthread_mutex_lock(&m_loggedInMutex);
	m_loggedIn = loggedIn_;
	pthread_mutex_unlock(&m_loggedInMutex);
}

CurrentUser::channelMode CurrentUser::getChannelMode()
{
	return m_mode;
}

void CurrentUser::setChannelMode(CurrentUser::channelMode mode_)
{
	m_mode = mode_;
}

/*
 * ServerSession.cpp
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#include "ServerSession.h"
#include <cstring>

ServerSession::ServerSession()
{
	m_usersCount = 0;
	m_channelsCount = 0;
	initializeMutexes();
	m_mainChannel = addChannel("Sandbox Channel");
	m_admin = addUser("server","server");
	m_mainChannel -> setAdmin(m_admin);
}

ServerSession::~ServerSession()
{
	pthread_mutex_lock(&m_channelsMutex);
	for(std::vector<IChannel*>::iterator channelIterator = m_channels.begin();
			channelIterator!=m_channels.end();channelIterator++)
	{
		delete *channelIterator;
	}
	pthread_mutex_unlock(&m_channelsMutex);
	pthread_mutex_lock(&m_usersMutex);
	for(std::vector<IUser*>::iterator userIterator = m_users.begin();
			userIterator!=m_users.end();userIterator++)
	{
		delete *userIterator;
	}
	pthread_mutex_unlock(&m_usersMutex);
}

IChannel* ServerSession::getChannel(std::string name)
{
	IChannel* result = NULL;
	pthread_mutex_lock(&m_channelsMutex);
	for(std::vector<IChannel*>::iterator channelIterator = m_channels.begin();
				channelIterator!=m_channels.end();channelIterator++)
	{
		if((*channelIterator) -> getName() == name)
		{
			result = *channelIterator;
			break;
		}
	}
	pthread_mutex_unlock(&m_channelsMutex);
	return result;
}

IChannel* ServerSession::addChannel(std::string name)
{
	pthread_mutex_lock(&m_channelsMutex);
	IChannel* newChannel = new CChannel(name);
	m_channels.push_back(newChannel);
	m_channelsCount ++;
	pthread_mutex_unlock(&m_channelsMutex);
	return newChannel;
}

int ServerSession::removeChannel(std::string name)
{
	int result = -1;
	pthread_mutex_lock(&m_channelsMutex);
	for(std::vector<IChannel*>::iterator channelIterator = m_channels.begin();
				channelIterator!=m_channels.end();channelIterator++)
	{
		if((*channelIterator) -> getName() == name)
		{
			delete *channelIterator;
			m_channelsCount --;
			m_channels.erase(channelIterator);
			result = 1;
			break;
		}
	}
	pthread_mutex_unlock(&m_channelsMutex);
	return result;
}

IUser* ServerSession::getUser(std::string name)
{
	IUser* result = NULL;
	pthread_mutex_lock(&m_usersMutex);
	for(std::vector<IUser*>::iterator userIterator = m_users.begin();
			userIterator!=m_users.end();userIterator++)
	{
		if((*userIterator) -> getName() == name)
		{
			result = *userIterator;
			break;
		}
	}
	pthread_mutex_unlock(&m_usersMutex);
	return result;
}

int ServerSession::addUser(IUser* user)
{
	int result = -1;
	pthread_mutex_lock(&m_usersMutex);
	m_users.push_back(user);
	m_usersCount++;
	pthread_mutex_unlock(&m_usersMutex);
	return result;
}

int ServerSession::removeUser(std::string name)
{
	int result = -1;
	pthread_mutex_lock(&m_usersMutex);
	for(std::vector<IUser*>::iterator usersIterator = m_users.begin();
			usersIterator!=m_users.end();usersIterator++)
	{
		if((*usersIterator) -> getName() == name)
		{
			delete *usersIterator;
			m_usersCount--;
			m_users.erase(usersIterator);
			result = 1;
			break;
		}
	}
	pthread_mutex_unlock(&m_usersMutex);
	return result;
}

bool ServerSession::isUser(std::string name)
{
	if(m_users.size() == 0)
	{
		return false;
	}

	bool result = false;
	for(std::vector<IUser*>::iterator usersIterator = m_users.begin();
				usersIterator!=m_users.end();usersIterator++)
	{
		if((*usersIterator) -> getName() == name)
		{
			result = true;
			break;
		}
	}
	return result;
}

bool ServerSession::isChannel(std::string name)
{
	bool result = false;
	for(std::vector<IChannel*>::iterator channelsIterator = m_channels.begin();
			channelsIterator!=m_channels.end();channelsIterator++)
	{
		if((*channelsIterator) -> getName() == name)
		{
			result = true;
			break;
		}
	}
	return result;
}

void ServerSession::initializeMutexes()
{
	pthread_mutexattr_t usersAttr;
	pthread_mutexattr_init(&usersAttr);
	pthread_mutex_init(&m_usersMutex, &usersAttr);
	pthread_mutexattr_t channelsAttr;
	pthread_mutexattr_init(&channelsAttr);
	pthread_mutex_init(&m_channelsMutex, &channelsAttr);
	pthread_mutexattr_t loggerAttr;
	pthread_mutexattr_init(&loggerAttr);
	pthread_mutex_init(&m_loggerMutex, &loggerAttr);
}

IUser* ServerSession::addUser(std::string name_, std::string password_)
{
	IUser* newUser = new User(name_,password_);
	pthread_mutex_lock(&m_usersMutex);
	m_users.push_back(newUser);
	m_usersCount++;
	pthread_mutex_unlock(&m_usersMutex);
	return newUser;
}

int ServerSession::getUsersCount()
{
	return m_usersCount;
}

int ServerSession::getChannelsCount()
{
	return m_channelsCount;
}

void ServerSession::fillChannelList(PROTOCOL::ChannelList& channelList)
{
	int channelCount = 0;
	for(std::vector<IChannel*>::iterator channelsIterator = m_channels.begin();
				channelsIterator!=m_channels.end();channelsIterator++)
	{
		strcpy(channelList.channels[channelCount].name,(*channelsIterator) -> getName().c_str());
		channelList.channels[channelCount].usersCount = (*channelsIterator) -> getUsersCount();
		channelCount++;
	}
	channelList.currentChannelNumber = m_channelsCount;
}

bool ServerSession::isAdmin(std::string name)
{
	return name == m_admin -> getName();
}

IChannel* ServerSession::getMainChannel()
{
	return m_mainChannel;
}

Logger ServerSession::getLogger()
{
	pthread_mutex_lock(&m_loggerMutex);
	return m_logger;
}

void ServerSession::releaseLogger()
{
	pthread_mutex_unlock(&m_loggerMutex);
}

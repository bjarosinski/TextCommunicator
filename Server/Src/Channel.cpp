/*
 * Channel.cpp
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#include "Channel.h"

CChannel::CChannel(std::string name_):IChannel(name_)
{

}

CChannel::~CChannel()
{
}

void CChannel::addUser(IUser* user)
{
	m_users.push_back(user);
	usersCount++;
}

void CChannel::removeUser(IUser* user)
{
	int count = 0;
	for(std::vector<IUser* >::iterator usersIterator = m_users.begin();
				usersIterator!=m_users.end(); usersIterator ++)
	{
		if((*usersIterator) -> getName() == user -> getName())
		{
			m_users.erase(usersIterator);
			usersCount--;
			break;
		}
		count ++;
	}
}

bool CChannel::hasUser(IUser* user)
{
	for(std::vector<IUser* >::iterator usersIterator = m_users.begin();
					usersIterator!=m_users.end(); usersIterator ++)
		{
			if((*usersIterator) -> getName() == user -> getName())
			{
				return true;
			}
		}
	return false;
}

std::vector<std::string> CChannel::getUsersNames()
{
	std::vector<std::string> usersNames;
	for(std::vector<IUser*>::iterator usersIterator = m_users.begin();
				usersIterator != m_users.end(); usersIterator ++)
	{
		usersNames.push_back((*usersIterator) -> getName());
	}
	return usersNames;
}

void CChannel::sendChannelMessage(PROTOCOL::Message& message)
{
	for(std::vector<IUser*>::iterator usersIterator = m_users.begin();
				usersIterator != m_users.end(); usersIterator ++)
	{
		(*usersIterator) -> getReceivSocket().sendMessage(message,(*usersIterator) -> getConnectInfo());
	}
}

void CChannel::setAdmin(IUser* admin_)
{
	channelAdmin = admin_;
}

bool CChannel::isAdmin(std::string name)
{
	return channelAdmin -> getName() == name;
}

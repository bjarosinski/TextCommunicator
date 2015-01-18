/*
 * User.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "User.h"

User::User(std::string name_,std::string password_):IUser(name_,password_)
{
}

User::~User()
{
	/*std::vector<IUser*>::iterator friendsIterator = friends.begin();
	for( ; friendsIterator != friends.end(); friendsIterator++)
	{
		delete *friendsIterator;
	}*/
}

void User::addFriend(IUser* newFriend)
{
	m_friends.push_back(newFriend);
}

void User::removeFriend(IUser* oldFriend)
{
	for(std::vector<IUser*>::iterator friendsIterator = m_friends.begin();
			friendsIterator!=m_friends.end(); friendsIterator ++)
	{
		if((*friendsIterator) -> getName() == oldFriend -> getName())
		{
			m_friends.erase(friendsIterator);
			break;
		}
	}
}

bool User::isFriend(IUser* checkedFriend)
{
	for(std::vector<IUser*>::iterator friendsIterator = m_friends.begin();
			friendsIterator!=m_friends.end(); friendsIterator ++)
	{
		if((*friendsIterator) -> getName() == checkedFriend -> getName())
		{
			return true;
		}
	}
	return false;
}

std::vector<std::string> User::getFriendsList(bool online)
{
	std::vector<std::string> friendsNames;
	for(std::vector<IUser*>::iterator usersIterator = m_friends.begin();
				usersIterator!=m_friends.end(); usersIterator++)
	{
		if((!online) || ((*usersIterator)->isLoggedIn()))
		{
			friendsNames.push_back((*usersIterator) -> getName());
		}
	}
	return friendsNames;
}

void User::addMail(PROTOCOL::Message message)
{
	message.messageId = t_mails.size();
	t_mails.push_back(message);
	if(loggedIn)
	{
		t_receivSocket.sendMessage(message,t_info);
	}
}

/*
 * IUser.cpp
 *
 *  Created on: 12-08-2013
 *      Author: b.jarosinski
 */

#include "IUser.h"
#include "IChannel.h"

IUser::IUser()
{
	t_banned = false;
	loggedIn = false;
	t_channel = NULL;
}

IUser::IUser(std::string name_, std::string password_):
t_name(name_), t_password(password_)
{
	t_banned = false;
	loggedIn = false;
	t_channel = NULL;
}

IUser::~IUser()
{
}

void IUser::changeChannel(IChannel *newChannel)
{
	if(t_channel != NULL)
	{
		t_channel -> removeUser(this);
	}
	t_channel = newChannel;
	if(t_channel != NULL)
	{
		t_channel -> addUser(this);
	}
}

std::string IUser::getName()
{
	return t_name;
}

std::string IUser::getPassword()
{
	return t_password;
}

bool IUser::isLoggedIn()
{
	return loggedIn;
}

void IUser::setLoggedIn(bool loggedIn_)
{
	loggedIn = loggedIn_;
}

std::string IUser::getToken()
{
	return t_token;
}

void IUser::setToken(std::string token_)
{
	t_token = token_;
}

bool IUser::checkToken(std::string token_)
{
	if (t_token.compare(token_)==0) {
		return true;
	}
	return false;
}

connectInfo IUser::getConnectInfo()
{
	return t_info;
}

void IUser::setConnectInfo(connectInfo info_)
{
	t_info = info_;
}

void IUser::setReceivSocket(Socket receivSocket_)
{
	t_receivSocket = receivSocket_;
}

Socket IUser::getReceivSocket()
{
	return t_receivSocket;
}

IChannel* IUser::getChannel()
{
	return t_channel;
}

void IUser::setPassword(std::string password_)
{
	t_password = password_;
}

void IUser::setName(std::string name_)
{
	t_name = name_;
}

void IUser::addMail(PROTOCOL::Message message)
{
	message.messageId = t_mails.size();
	t_mails.push_back(message);
}

int IUser::getMails(bool delivered)
{
	int mailsCount = 0;
	for(std::vector<PROTOCOL::Message>::iterator mailsIterator = t_mails.begin();
				mailsIterator!=t_mails.end(); mailsIterator++)
	{
		if(delivered && mailsIterator -> delivered)
		{
			t_receivSocket.sendMessage(*mailsIterator,t_info);
			mailsCount++;
		}
		else if(!delivered && !(mailsIterator -> delivered))
		{
			mailsIterator -> delivered = true;
			t_receivSocket.sendMessage(*mailsIterator,t_info);
						mailsCount++;
		}
	}
	return mailsCount;
}

int IUser::removeMail(int mailId)
{
	int mailCount = 0;
	for(std::vector<PROTOCOL::Message>::iterator mailsIterator = t_mails.begin();
				mailsIterator!=t_mails.end(); mailsIterator++)
	{
		if(mailsIterator -> messageId == mailId)
		{
			t_mails.erase(mailsIterator);
			return 1;
		}
	}
	return -1;
}

bool IUser::isBanned()
{
	return t_banned;
}

void IUser::setBanned(bool banned_)
{
	t_banned = banned_;
}

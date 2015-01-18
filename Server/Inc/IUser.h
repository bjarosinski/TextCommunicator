/*
 * IUser.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef IUSER_H_
#define IUSER_H_

#include <vector>
#include "Socket.h"

class IChannel;

enum rank
{
	USER,
	ADMIN
};

class IUser
{
public:
	IUser();
	IUser(std::string name_, std::string password_);
	virtual ~IUser();
	virtual void changeChannel(IChannel *newChannel);
	virtual std::string getName();
	virtual std::string getPassword();
	virtual void setName(std::string);
	virtual void setPassword(std::string);
	virtual bool isLoggedIn();
	virtual void setLoggedIn(bool loggedIn_);
	virtual std::string getToken();
	virtual void setToken(std::string token_);
	virtual bool  checkToken(std::string token_);
	virtual connectInfo getConnectInfo();
	virtual void setConnectInfo(connectInfo info_);
	virtual void setReceivSocket(Socket receivSocket_);
	virtual Socket getReceivSocket();
	virtual void addFriend(IUser*) = 0;
	virtual void removeFriend(IUser*) = 0;
	virtual bool isFriend(IUser* checkedFriend) = 0;
	virtual std::vector<std::string> getFriendsList(bool online = false) = 0;
	virtual IChannel* getChannel();
	virtual void addMail(PROTOCOL::Message message);
	virtual int removeMail(int idMail);
	virtual int getMails(bool delivered = false);
	virtual bool isBanned();
	virtual void setBanned(bool banned_);
protected:
	std::string t_name;
	std::string t_password;
	std::string t_token;
	bool loggedIn;
	bool t_banned;
	connectInfo t_info;
	Socket t_receivSocket;
	IChannel* t_channel;
	std::vector<PROTOCOL::Message> t_mails;
};

#endif /* IUSER_H_ */

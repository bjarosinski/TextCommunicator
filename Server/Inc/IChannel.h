/*
 * IChannel.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef ICHANNEL_H_
#define ICHANNEL_H_

#include <string>
#include "IUser.h"
#include "protocol.h"

class IChannel
{
public:
	IChannel(std::string name_):name(name_){ usersCount = 0; channelAdmin = NULL;};
	virtual ~IChannel(){};
	virtual void addUser(IUser* user) = 0;
	virtual void removeUser(IUser* user) = 0;
	bool hasUser(IUser* user);
	virtual std::string getName() {return name;}
	virtual unsigned getUsersCount(){ return usersCount; }
	virtual std::vector<std::string> getUsersNames() = 0;
	virtual void sendChannelMessage(PROTOCOL::Message& message) = 0;
	virtual void setAdmin(IUser* ) = 0;
	virtual bool isAdmin(std::string) = 0;
protected:
	std::string name;
	unsigned usersCount;
	IUser* channelAdmin;
};

#endif /* ICHANNEL_H_ */

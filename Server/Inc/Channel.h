/*
 * Channel.h
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "IChannel.h"
#include "IUser.h"
#include "protocol.h"

class CChannel: public IChannel
{
public:
	CChannel(std::string name_);
	virtual ~CChannel();
	void addUser(IUser* user);
	void removeUser(IUser* user);
	bool hasUser(IUser* user);
	virtual std::vector<std::string> getUsersNames();
	void sendChannelMessage(PROTOCOL::Message& message);
	void setAdmin(IUser*);
	bool isAdmin(std::string);
private:
	std::vector<IUser*>m_users;
};

#endif /* CHANNEL_H_ */

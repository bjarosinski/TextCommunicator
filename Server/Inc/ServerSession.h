/*
 * ServerSession.h
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#ifndef SERVERSESSION_H_
#define SERVERSESSION_H_

#include <vector>
#include "Channel.h"
#include "User.h"
#include "protocol.h"
#include <pthread.h>
#include "Logger.h"

class ServerSession
{
public:
	ServerSession();
	virtual ~ServerSession();
	IChannel* getChannel(std::string name);
	IChannel* addChannel(std::string name);
	int removeChannel(std::string name);
	IUser* getUser(std::string name);
	int addUser(IUser* user);
	IUser* addUser(std::string name, std::string password);
	int removeUser(std::string name);
	int getUsersCount();
	int getChannelsCount();
	bool isUser(std::string name);
	bool isChannel(std::string name);
	void fillChannelList(PROTOCOL::ChannelList& channelList);
	bool isAdmin(std::string name);
	void kickUserFromChannel();
	IChannel* getMainChannel();
	Logger getLogger();
	void releaseLogger();
private:
	void initializeMutexes();
	Logger m_logger;
	IUser* m_admin;
	IChannel* m_mainChannel;
	std::vector<IChannel*> m_channels;
	std::vector<IUser*> m_users;
	int m_channelsCount;
	int m_usersCount;
	pthread_mutex_t m_channelsMutex;
	pthread_mutex_t m_usersMutex;
	pthread_mutex_t m_loggerMutex;
};

#endif /* SERVERSESSION_H_ */

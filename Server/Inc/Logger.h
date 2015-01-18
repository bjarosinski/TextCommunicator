/*
 * Logger.h
 *
 *  Created on: 14-08-2013
 *      Author: b.jarosinski
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include "protocol.h"

class Logger
{
public:
	Logger();
	~Logger();
	void clientConnectLog(std::string userIP, int userPort);
	void clientRequest(std::string userName, PROTOCOL::RequestCommand command);
	void clientSendMessage(PROTOCOL::Message message);
	void clientRegistered(std::string userName);
	void clientUDPRegistered(std::string userName,std::string userIP, int userPort);
	void clientAddFriend(std::string userName,std::string friendName);
	void clientRemoveMail(std::string userName);
	void clientRemoveFriend(std::string userName,std::string friendName);
	void clientChangeChannel(std::string userName, std::string channelName);
	void clientCreateChannel(std::string userName, std::string channelName);
	void clientResponse(std::string userName, PROTOCOL::ResponseCode code);
	void clientBanOther(std::string userName, std::string otherName);
	void clientUnbanOther(std::string userName, std::string otherName);
	void clientKickOther(std::string userName, std::string otherName);
	void clientTokenGenerated(std::string userName, std::string token);
private:
	std::string generateLabel();
};

#endif /* LOGGER_H_ */

/*
 * CommandParser.h
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "protocol.h"
#include "Socket.h"
#include <stdio.h>
#include "ServerSession.h"
#include "Validator.h"
#include "Logger.h"

class CommandParser
{
public:
	CommandParser(Socket socket,ServerSession& session_);
	~CommandParser();
	void parseCommand(PROTOCOL::Request& request);
	std::string generateToken();
private:
	std::string m_userName;
	Socket m_socket;
	ServerSession& m_session;
	Validator m_validator;
	int end();
	int regist();
	int authenticate();
	int login();
	int logout(PROTOCOL::Request&);
	int listChannel(PROTOCOL::Request&);
	int changeChannel(PROTOCOL::Request&);
	int changePassword(PROTOCOL::Request&);
	int getUsersChannelList(PROTOCOL::Request&);
	int getFriendsList(PROTOCOL::Request&,bool online = false);
	int addFriend(PROTOCOL::Request&);
	int removeFriend(PROTOCOL::Request&);
	int sendMessage(PROTOCOL::Request&);
	int getAllMail(PROTOCOL::Request&);
	int removeMail(PROTOCOL::Request&);
	int takeUserAcces(PROTOCOL::Request&);
	int banUser();
	int unbanUser();
	int kickUser();
	void createCommandLog(PROTOCOL::RequestCommand command);
	void createResponseLog(PROTOCOL::ResponseCode code);
	int checkUser(std::string token);
};

#endif /* COMMANDPARSER_H_ */

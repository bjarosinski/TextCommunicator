/*
 * CommandParser.cpp
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#include "CommandParser.h"
#include <cstring>
#include <time.h>
#include <stdlib.h>

CommandParser::CommandParser(Socket socket_,ServerSession& session_):
			m_socket(socket_),m_session(session_),m_validator(m_session)
{
	m_userName = "";
}

CommandParser::~CommandParser()
{
}

void CommandParser::parseCommand(PROTOCOL::Request& request)
{
	m_session.getLogger().clientRequest(m_userName,request.command);
	m_session.releaseLogger();
	switch(request.command)
	{
		case PROTOCOL::REGISTER:
			regist();
			break;
		case PROTOCOL::LOGIN:
			login();
			break;
		case PROTOCOL::ENDS:
			end();
			break;
		case PROTOCOL::LOGOUT:
			logout(request);
			break;
		case PROTOCOL::CHANNEL_LIST:
			listChannel(request);
			break;
		case PROTOCOL::CHANGE_CHANNEL:
			changeChannel(request);
			break;
		case PROTOCOL::CHANGE_PASS:
			changePassword(request);
			break;
		case PROTOCOL::ALL_USERS_FROM_CHANNEL:
			getUsersChannelList(request);
			break;
		case PROTOCOL::FRIENDS:
			getFriendsList(request);
			break;
		case PROTOCOL::ADD_FRIEND:
			addFriend(request);
			break;
		case PROTOCOL::REMOVE_FRIEND:
			removeFriend(request);
			break;
		case PROTOCOL::CHECK_ONLINE:
			getFriendsList(request,true);
			break;
		case PROTOCOL::MESSAGE:
			sendMessage(request);
			break;
		case PROTOCOL::GET_ALL_MAIL:
			getAllMail(request);
			break;
		case PROTOCOL::REMOVE_MAIL:
			removeMail(request);
			break;
		case PROTOCOL::BAN_USER: case PROTOCOL::UNBAN_USER: case PROTOCOL::KICK_USER:
			takeUserAcces(request);
			break;
	}
}

int CommandParser::login()
{
	int result = 0;
	PROTOCOL::ResponseInfo response;
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::Authorize auth;
	m_socket.getRequest(auth,sizeof(auth));
	m_userName = auth.name;
	validCode valid = m_validator.checkUserLoggData(auth.name,auth.password);
	std::string token;
	switch(valid)
	{
		case OKI:
			response.code = PROTOCOL::OKS;
			token = generateToken();
			m_session.getLogger().clientTokenGenerated(m_userName, token);
			m_session.releaseLogger();
			m_session.getUser(auth.name) -> setToken(token);
			strcpy(response.token,token.c_str());
			m_session.getUser(auth.name) -> changeChannel(m_session.getMainChannel());
			break;
		case USER_NOT_EXIST:
			response.code = PROTOCOL::WRONG_USER;
			break;
		case PASSWORD_WRONG:
			response.code = PROTOCOL::WRONG_PASSWORD;
			break;
		case USER_ALREADY_LOGGED_IN:
			response.code = PROTOCOL::ALREADY_LOGGED_IN;
			break;
		case USER_BANNED:
			response.code = PROTOCOL::BANNED_USER;
			break;
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}

int CommandParser::regist()
{
	int result = 1;
	PROTOCOL::ResponseInfo response;
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::Authorize auth;
	m_socket.getRequest(auth,sizeof(auth));
	if(m_session.isUser(auth.name))
	{
		response.code = PROTOCOL::WRONG_USER;
	}
	else
	{
		std::string name = auth.name;
		std::string password = auth.password;
		validCode validName = m_validator.validateName(name);
		validCode validPassword = m_validator.validatePassword(password);
		if(validName != OKI)
		{
			response.code = PROTOCOL::WRONG_USER_NAME;
		}
		else if(validPassword != OKI)
		{
			response.code = PROTOCOL::WRONG_PASSWORD;
			result = -1;
		}
		else
		{
			m_session.addUser(name,password);
			m_session.getLogger().clientRegistered(name);
			m_session.releaseLogger();
		}
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}

std::string CommandParser::generateToken()
{
	std::string token;
	token.resize(TOKEN_SIZE - 1);
	srand(time(NULL));
	for(int tokenSign = 0; tokenSign < (TOKEN_SIZE - 1); tokenSign ++)
	{
		switch(rand() % 3 )
		{
			case 0: //number
				token[tokenSign] = rand() % 10 + 48;
				break;
			case 1: //big letter
				token[tokenSign] = rand() % 26 + 65;
				break;
			case 2: //small letter
				token[tokenSign] = rand() % 26 + 97;
				break;
		}
	}
	return token;
}

int CommandParser::logout(PROTOCOL::Request& request)
{
	int result = 0;
	PROTOCOL::ResponseInfo response;
	if(!m_session.isUser(m_userName))
	{
		response.code = PROTOCOL::UNAUTHORIZE;
		return -1;
	}
	else if(!(m_session.getUser(m_userName) -> checkToken(request.token)))
	{
		response.code = PROTOCOL::UNAUTHORIZE;
		result = -1;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		m_session.getUser(m_userName) -> setLoggedIn(false);
		m_session.getUser(m_userName) -> changeChannel(NULL);
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}

int CommandParser::listChannel(PROTOCOL::Request& request)
{
	PROTOCOL::ChannelList response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	else
	{
		response.response = PROTOCOL::OKS;
		m_session.fillChannelList(response);
		m_socket.sendResponse(response,sizeof(response));
	}
	createResponseLog(response.response);
	return 1;
}

int CommandParser::changeChannel(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}

	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponse(response,sizeof(response));
	PROTOCOL::ChangeChannel changeChannelRequest;
	m_socket.getRequest(changeChannelRequest,sizeof(changeChannelRequest));
	if(m_session.isChannel(changeChannelRequest.name))
	{
		if(m_session.getChannel(changeChannelRequest.name)->getUsersCount() < MAX_USERS_ON_CHANNEL)
		{
			m_session.getLogger().clientChangeChannel(m_userName,changeChannelRequest.name);
			m_session.releaseLogger();
			m_session.getUser(m_userName) -> changeChannel(m_session.getChannel(changeChannelRequest.name));
			PROTOCOL::Message message;
			message.type = PROTOCOL::Message::NORMAL;
			std::string messageContent = "Your are now in Channel: ";
			messageContent += changeChannelRequest.name;
			strcpy(message.content,messageContent.c_str());
			strcpy(message.from,"server");
			m_session.getUser(m_userName) -> getReceivSocket().sendMessage(message,m_session.getUser(m_userName) -> getConnectInfo());
		}
		else
		{
			response.code = PROTOCOL::USERS_OVERFLOW;
		}
	}
	else
	{
		if(m_session.getChannelsCount() < MAX_CHANNELS_NUMBER)
		{
			m_session.getLogger().clientCreateChannel(m_userName,changeChannelRequest.name);
			m_session.releaseLogger();
			IChannel* newChannel = m_session.addChannel(changeChannelRequest.name);
			newChannel -> setAdmin(m_session.getUser(m_userName));
			m_session.getUser(m_userName) -> changeChannel(newChannel);
			PROTOCOL::Message message;
			message.type = PROTOCOL::Message::NORMAL;
			std::string messageContent = "Your are now in Channel: ";
			messageContent += changeChannelRequest.name;
			strcpy(message.content,messageContent.c_str());
			strcpy(message.from,"server");
			m_session.getUser(m_userName) -> getReceivSocket().sendMessage(message,m_session.getUser(m_userName) -> getConnectInfo());
		}
		else
		{
			response.code = PROTOCOL::CHANNELS_OVERFLOW;
		}
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::changePassword(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponse(response,sizeof(response));
	PROTOCOL::ChangePassword changePasswordRequest;
	m_socket.getRequest(changePasswordRequest,sizeof(changePasswordRequest));
	if(m_session.getUser(m_userName) -> getPassword() == changePasswordRequest.oldPassword)
	{
		m_session.getUser(m_userName) -> setPassword(changePasswordRequest.newPassword);
	}
	else
	{
		response.code = PROTOCOL::WRONG_PASSWORD;
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::getUsersChannelList(PROTOCOL::Request& request)
{
	if(checkUser(request.token)==-1)
	{
		return -1;
	}
	PROTOCOL::UsersLists response;
	int userCount = 0;
	response.code = PROTOCOL::OKS;
	std::vector<std::string> names = m_session.getUser(m_userName) -> getChannel() -> getUsersNames();
	for(std::vector<std::string>::iterator namesIterator = names.begin();
				namesIterator!=names.end(); namesIterator++)
	{
		strcpy(response.usersList[userCount].name,namesIterator -> c_str());
		userCount++;
	}
	response.usersCount = m_session.getUser(m_userName) -> getChannel() -> getUsersCount();
	createResponseLog(response.code);
	m_socket.sendResponse(response,sizeof(response));
	return 1;
}

int CommandParser::end()
{
	PROTOCOL::ResponseInfo response;
	if((m_session.isUser(m_userName)) && (m_session.getUser(m_userName) -> isLoggedIn()))
	{
		m_session.getUser(m_userName) -> setLoggedIn(false);
		m_session.getUser(m_userName) -> changeChannel(NULL);
	}
	response.code = PROTOCOL::OKS;
	m_socket.sendResponseInfo(response);
	createResponseLog(response.code);
	return 1;
}

int CommandParser::getFriendsList(PROTOCOL::Request& request,bool online)
{
	PROTOCOL::UsersLists response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	int userCount = 0;
	response.code = PROTOCOL::OKS;
	std::vector<std::string> names = m_session.getUser(m_userName) -> getFriendsList(online);
	for(std::vector<std::string>::iterator namesIterator = names.begin();
				namesIterator!=names.end(); namesIterator++)
	{
		strcpy(response.usersList[userCount].name,namesIterator -> c_str());
		response.usersList[userCount].available = m_session.getUser(*namesIterator) -> isLoggedIn();
		userCount++;

	}
	response.usersCount = names.size();
	createResponseLog(response.code);
	m_socket.sendResponse(response,sizeof(response));
	return 1;
}

int CommandParser::addFriend(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::Friend friendRequest;
	m_socket.getRequest(friendRequest,sizeof(friendRequest));
	if((!m_session.isUser(friendRequest.name)) || (m_userName == friendRequest.name))
	{
		response.code = PROTOCOL::WRONG_USER;
	}
	else if(m_session.getUser(m_userName) -> isFriend(m_session.getUser(friendRequest.name)))
	{
		response.code = PROTOCOL::ALREADY_FRIEND;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		m_session.getLogger().clientAddFriend(m_userName,friendRequest.name);
		m_session.releaseLogger();
		m_session.getUser(m_userName) -> addFriend(m_session.getUser(friendRequest.name));
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::removeFriend(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::Friend friendRequest;
	m_socket.getRequest(friendRequest,sizeof(friendRequest));
	if((!m_session.isUser(friendRequest.name)) || (m_userName == friendRequest.name))
	{
		response.code = PROTOCOL::WRONG_USER;
	}
	else if(!(m_session.getUser(m_userName) -> isFriend(m_session.getUser(friendRequest.name))))
	{
		response.code = PROTOCOL::NOT_FRIEND;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		m_session.getLogger().clientRemoveFriend(m_userName,friendRequest.name);
		m_session.releaseLogger();
		m_session.getUser(m_userName) -> removeFriend(m_session.getUser(friendRequest.name));
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::sendMessage(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::Message message;
	m_socket.getRequest(message,sizeof(message));
	strcpy(message.from,m_userName.c_str());
	time(&message.sendDate);
	tm *ziom = gmtime(&message.sendDate);
	switch (message.type)
	{
		case PROTOCOL::Message::NORMAL: case PROTOCOL::Message::EMOTE:
			m_session.getUser(m_userName)->getChannel() -> sendChannelMessage(message);
			m_session.getLogger().clientSendMessage(message);
			m_session.releaseLogger();
			break;
		case PROTOCOL::Message::PRIVATE:
			if(!(m_session.isUser(message.to)))
			{
				response.code = PROTOCOL::WRONG_USER;
			}
			else if(!(m_session.getUser(m_userName)->isFriend(m_session.getUser(message.to))))
			{
				response.code = PROTOCOL::NOT_FRIEND;
			}
			else if(!(m_session.getUser(message.to) -> isLoggedIn()))
			{
				response.code = PROTOCOL::FRIEND_OFFLINE;
			}
			else
			{
				m_session.getLogger().clientSendMessage(message);
				m_session.releaseLogger();
				m_session.getUser(message.to)->getReceivSocket().
						sendMessage(message,m_session.getUser(message.to)->getConnectInfo());
				response.code = PROTOCOL::OKS;
			}
			break;
		case PROTOCOL::Message::MAIL:
			if(!(m_session.isUser(message.to)))
			{
				response.code = PROTOCOL::WRONG_USER;
			}
			else
			{
				m_session.getLogger().clientSendMessage(message);
				m_session.releaseLogger();
				message.delivered = false;
				m_session.getUser(message.to)->addMail(message);
				response.code = PROTOCOL::OKS;
			}
			break;
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::getAllMail(PROTOCOL::Request& request)
{
	PROTOCOL::GetAllMailsInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	response.mails = m_session.getUser(m_userName) -> getMails(true);
	response.mails += m_session.getUser(m_userName) -> getMails();
	createResponseLog(response.code);
	m_socket.sendResponse(response,sizeof(response));
	return 1;
}

int CommandParser::removeMail(PROTOCOL::Request& request)
{
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	response.code = PROTOCOL::OKS;
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	PROTOCOL::RemoveMail removeMessageRequest;
	m_socket.getRequest(removeMessageRequest,sizeof(removeMessageRequest));
	if(!(m_session.getUser(m_userName) -> checkToken(request.token)))
	{
		response.code = PROTOCOL::UNAUTHORIZE;
		createResponseLog(response.code);
		m_socket.sendResponseInfo(response);
		return -1;
	}
	if(m_session.getUser(m_userName) -> removeMail(removeMessageRequest.messageId) == -1)
	{
		response.code = PROTOCOL::WRONG_ID;
		createResponseLog(response.code);
		m_socket.sendResponseInfo(response);
		return -1;
	}
	response.code = PROTOCOL::OKS;
	m_session.getLogger().clientRemoveMail(m_userName);
	m_session.releaseLogger();
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return 1;
}

int CommandParser::takeUserAcces(PROTOCOL::Request& request)
{
	int result = 1;
	PROTOCOL::ResponseInfo response;
	if(checkUser(request.token) == -1)
	{
		return -1;
	}
	if(request.command == PROTOCOL::KICK_USER)
	{
		if(m_session.getUser(m_userName) -> getChannel() -> isAdmin(m_userName))
		{
			response.code = PROTOCOL::OKS;
			createResponseLog(response.code);
			m_socket.sendResponseInfo(response);
			kickUser();
		}
		else
		{
			response.code = PROTOCOL::UNAUTHORIZE;
			createResponseLog(response.code);
			m_socket.sendResponseInfo(response);
			result = -1;
		}
	}
	else
	{
		if(m_session.isAdmin(m_userName))
		{
			response.code = PROTOCOL::OKS;
			createResponseLog(response.code);
			m_socket.sendResponseInfo(response);
			request.command == PROTOCOL::BAN_USER ? banUser() : unbanUser();
		}
		else
		{
			response.code = PROTOCOL::UNAUTHORIZE;
			createResponseLog(response.code);
			m_socket.sendResponseInfo(response);
			result = -1;
		}
	}
	return result;
}

int CommandParser::banUser()
{
	int result = 1;
	PROTOCOL::BanUser request;
	PROTOCOL::ResponseInfo response;
	m_socket.getRequest(request,sizeof(request));
	if(!m_session.isUser(request.name))
	{
		response.code = PROTOCOL::WRONG_USER;
		result = -1;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		if(m_session.getUser(request.name) -> isLoggedIn())
		{
			PROTOCOL::Message message;
			message.type = PROTOCOL::Message::NORMAL;
			strcpy(message.content,"You are banned");
			strcpy(message.from,"Server");
			m_session.getUser(request.name) -> getReceivSocket().sendMessage(message,m_session.getUser(request.name)->getConnectInfo());
			m_session.getUser(request.name) -> setLoggedIn(false);
			m_session.getUser(request.name) -> changeChannel(m_session.getMainChannel());

		}
		m_session.getLogger().clientBanOther(m_userName,request.name);
		m_session.releaseLogger();
		m_session.getUser(request.name) -> setBanned(true);
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}

int CommandParser::unbanUser()
{
	int result = 1;
	PROTOCOL::Unban request;
	PROTOCOL::ResponseInfo response;
	m_socket.getRequest(request,sizeof(request));
	if(!m_session.isUser(request.name))
	{
		response.code = PROTOCOL::WRONG_USER;
		result = -1;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		m_session.getUser(request.name) -> setBanned(false);
		m_session.getLogger().clientUnbanOther(m_userName,request.name);
		m_session.releaseLogger();
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}
int CommandParser::kickUser()
{
	int result = 1;
	PROTOCOL::KickUser request;
	PROTOCOL::ResponseInfo response;
	m_socket.getRequest(request,sizeof(request));
	if(!m_session.isUser(request.name))
	{
		response.code = PROTOCOL::WRONG_USER;
		result = -1;
	}
	else
	{
		response.code = PROTOCOL::OKS;
		if(m_session.getUser(request.name) -> isLoggedIn())
		{
			PROTOCOL::Message message;
			message.type = PROTOCOL::Message::NORMAL;
			strcpy(message.content,"You are kicked");
			strcpy(message.from,"Server");
			m_session.getUser(request.name) -> getReceivSocket().sendMessage(message,m_session.getUser(request.name)->getConnectInfo());
			m_session.getUser(request.name) -> changeChannel(m_session.getMainChannel());
			std::string messageContent = "Your are now in Channel: ";
			messageContent += m_session.getMainChannel() -> getName();
			strcpy(message.content,messageContent.c_str());
			strcpy(message.from,"server");
			m_session.getUser(request.name) -> getReceivSocket().sendMessage(message,m_session.getUser(request.name)->getConnectInfo());
			m_session.getLogger().clientKickOther(m_userName,request.name);
			m_session.releaseLogger();
		}
	}
	createResponseLog(response.code);
	m_socket.sendResponseInfo(response);
	return result;
}

void CommandParser::createCommandLog(PROTOCOL::RequestCommand command)
{
	m_session.getLogger().clientRequest(m_userName,command);
	m_session.releaseLogger();
}

void CommandParser::createResponseLog(PROTOCOL::ResponseCode code)
{
	m_session.getLogger().clientResponse(m_userName,code);
	m_session.releaseLogger();
}

int CommandParser::checkUser(std::string token)
{
	PROTOCOL::ResponseInfo response;
	validCode valCode = m_validator.checkUser(m_userName,token);
	if(valCode == USER_UNAUTHORIZED)
	{
		response.code = PROTOCOL::UNAUTHORIZE;
		m_socket.sendResponse(response,sizeof(response));
		return -1;
	}
	else if(valCode == USER_BANNED)
	{
		response.code = PROTOCOL::BANNED_USER;
		m_socket.sendResponse(response,sizeof(response));
		return -1;
	}
	return 1;
}

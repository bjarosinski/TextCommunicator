/*
 * Logger.cpp
 *
 *  Created on: 14-08-2013
 *      Author: b.jarosinski
 */

#include "Logger.h"
#include  <sstream>
#include <iostream>

Logger::Logger()
{

}

Logger::~Logger()
{
}

void Logger::clientConnectLog(std::string userIP, int userPort)
{
	std::string result = generateLabel();
	result += "TCP client connected. IP: ";
	result += userIP;
	std::ostringstream stringMaker;
	stringMaker << userPort;
	result += " PORT: ";
	result += stringMaker.str();
	std::cout << result << std::endl;
}

void Logger::clientTokenGenerated(std::string userName, std::string token)
{
	std::cout << generateLabel() << " Client: " << userName << " generate Token: " << token << std::endl;
}
void Logger::clientRequest(std::string userName,PROTOCOL::RequestCommand command)
{
	std::string result = generateLabel();
	result += "Client : ";
	result += userName;
	result += " requests ";
	switch(command)
	{
		case PROTOCOL::REGISTER:
			result += "registration";
			break;
		case PROTOCOL::LOGOUT:
			result += "logout";
			break;
		case PROTOCOL::LOGIN:
			result += "login";
			break;
		case PROTOCOL::CHANNEL_LIST:
			result += "channel lists";
			break;
		case PROTOCOL::CHANGE_CHANNEL:
			result += "change channel";
			break;
		case PROTOCOL::ENDS:
			result += "end of session";
			break;
		case PROTOCOL::ALL_USERS_FROM_CHANNEL:
			result += "user on channel lists";
			break;
		case PROTOCOL::FRIENDS:
			result += "friend list";
			break;
		case PROTOCOL::MESSAGE:
			result += "message send";
			break;
		case PROTOCOL::CHANGE_PASS:
			result += "password change";
			break;
		case PROTOCOL::REMOVE_MAIL:
			result += "mail remove";
			break;
		case PROTOCOL::ADD_FRIEND:
			result += "friend add";
			break;
		case PROTOCOL::REMOVE_FRIEND:
			result += "friend remove";
			break;
		case PROTOCOL::GET_ALL_MAIL:
			result += "get all mail";
			break;
		case PROTOCOL::BAN_USER:
			result += "ban user";
			break;
		case PROTOCOL::UNBAN_USER:
			result += "unban user";
			break;
		case PROTOCOL::KICK_USER:
			result += "kick other user from channel";
			break;
		case PROTOCOL::CHECK_ONLINE:
			result += "online friends lists";
			break;
		default:
			result += "uknown command";
			break;
	}
	std::cout << result << std::endl;
}

void Logger::clientSendMessage(PROTOCOL::Message message)
{
	std::string result = generateLabel();
	result += "Client: ";
	result += message.from;
	switch(message.type)
	{
		case PROTOCOL::Message::PRIVATE:
			result += " sent private message to: ";
			result += message.to;
			break;
		case PROTOCOL::Message::NORMAL:
			result += " sent channel message.";
			break;
		case PROTOCOL::Message::EMOTE:
			result += " sent emote channel message";
			break;
		case PROTOCOL::Message::MAIL:
			result += " sent mail to: ";
			result += message.to;
			break;
	}
	std::cout << result << std::endl;
}

void Logger::clientRegistered(std::string userName)
{
	std::cout << generateLabel() << "Client: " << userName << " is registered." << std::endl;
}

void Logger::clientUDPRegistered(std::string userName,std::string userIP, int userPort)
{
	std::string result = generateLabel();
	result += "UDP client connected. IP: ";
	result += userIP;
	std::ostringstream stringMaker;
	stringMaker << userPort;
	result += " PORT: ";
	result += stringMaker.str();
	result += " receiver registered";
	std::cout << result << std::endl;
}

void Logger::clientAddFriend(std::string userName,std::string friendName)
{
	std::cout<<generateLabel() << "Client: " << userName << " add Friend " << friendName << std::endl;
}

void Logger::clientRemoveMail(std::string userName)
{
	std::cout << generateLabel() << "Client: " << userName << " remove mail "<<std::endl;
}

void Logger::clientRemoveFriend(std::string userName,std::string friendName)
{
	std::cout<<generateLabel() << "Client: " << userName << " remove Friend " << friendName << std::endl;
}

void Logger::clientChangeChannel(std::string userName, std::string channelName)
{
	std::cout<<generateLabel() << "Client: " << userName << " change channel to " << channelName << std::endl;
}

void Logger::clientCreateChannel(std::string userName, std::string channelName)
{
	std::cout<<generateLabel() << "Client: " << userName << " create channel " << channelName << std::endl;
}

void Logger::clientResponse(std::string userName, PROTOCOL::ResponseCode code)
{
	std::string result = generateLabel();
	result += "Client: ";
	result += userName;
	result += " operation result: ";
	switch(code)
	{
		case PROTOCOL::OKS:
			result += "OK";
			break;
		case PROTOCOL::WRONG_COMMAND:
			result += "WRONG COMMAND";
			break;
		case PROTOCOL::WRONG_USER:
			result += "WRONG USER";
			break;
		case PROTOCOL::WRONG_PASSWORD:
			result += "WRONG PASSWORD";
			break;
		case PROTOCOL::WRONG_USER_NAME:
			result += "WRONG USER NAME";
			break;
		case PROTOCOL::WRONG_PASS_STRUCTURE:
			result += "WRONG PASS STRUCTURE";
			break;
		case PROTOCOL::UNAUTHORIZE:
			result += "UNAUTHORIZE";
			break;
		case PROTOCOL::USERS_OVERFLOW:
			result += "USERS OVERFLOW";
			break;
		case PROTOCOL::CHANNELS_OVERFLOW:
			result += "CHANNELS OVERFLOW";
			break;
		case PROTOCOL::WRONG_ID:
			result += "WRONG ID";
			break;
		case PROTOCOL::BANNED_USER:
			result += "BANNED USER";
			break;
		case PROTOCOL::ALREADY_LOGGED_IN:
			result += "ALREADY LOGGED IN";
			break;
		case PROTOCOL::ALREADY_FRIEND:
			result += "ALREADY FRIEND";
			break;
		case PROTOCOL::FRIEND_OFFLINE:
			result += "FRIEND OFFLINE";
			break;
		case PROTOCOL::NOT_FRIEND:
			result += "NOT FRIEND";
			break;
		case PROTOCOL::ERROR:
			result += "ERROR";
			break;
	}
	std::cout << result << std::endl;
}

void Logger::clientBanOther(std::string userName, std::string otherName)
{
	std::cout << generateLabel() << "Client: " << userName << " ban user: " << otherName << std::endl;
}

void Logger::clientUnbanOther(std::string userName, std::string otherName)
{
	std::cout << generateLabel() << "Client: " << userName << " unban user: " << otherName << std::endl;
}

void Logger::clientKickOther(std::string userName, std::string otherName)
{
	std::cout << generateLabel() << "Client: " << userName << " kick user: " << otherName << std::endl;
}

std::string Logger::generateLabel()
{
	std::string result;
	std::ostringstream stringMaker;
	time_t currentTime;
	time(&currentTime);
	tm* date = gmtime(&currentTime);
	result = "[ LOG ] [ ";
	stringMaker << date -> tm_year + 1900;
	result += stringMaker.str();
	stringMaker.str("");
	stringMaker << date -> tm_mon + 1;
	result += "-";
	result += stringMaker.str();
	stringMaker.str("");
	stringMaker << date -> tm_mday;
	result +="-";
	result += stringMaker.str();
	stringMaker.str("");
	stringMaker << date -> tm_hour + 2;
	result += " ";
	result += stringMaker.str();
	stringMaker.str("");
	stringMaker << date -> tm_min;
	result += ":";
	result += stringMaker.str();
	stringMaker.str("");
	stringMaker << date -> tm_sec;
	result += ":";
	result += stringMaker.str();
	result += " ] ";
	return result;
}

/*
 * Sender.cpp
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#include "Sender.h"
#include <cstring>

Sender::Sender(Session& session_,Interface* interface_,MyQueue<PROTOCOL::Message>* messageQueue_):
				m_session(session_),m_interface(interface_), m_messageQueue(m_messageQueue)
{
	m_session.setUserLoggedIn(false);
}

Sender::~Sender()
{
}

void Sender::setInterface(Interface* interface_)
{
	m_interface = interface_;
}

void Sender::setMessageQueue(MyQueue<PROTOCOL::Message>* messageQueue_)
{
	m_messageQueue = messageQueue_;
}

PROTOCOL::ResponseCode Sender::sendCommand(option command)
{
	PROTOCOL::ResponseCode result = PROTOCOL::OKS;
	PROTOCOL::Message::messageMode mode;
	switch(command)
	{
		case CONNECT_C:
			result = connectToServer();
			break;
		case LOGIN_C:
			result = authorize();
			break;
		case REGISTER_C:
			result = regist();
			break;
		case END_C:
			result = end();
			break;
		case LOGOUT_C:
			result = logout();
			break;
		case CHANNEL_LIST_C:
			result = channelList();
			break;
		case CHANGE_CHANNEL_C:
			result = changeChannel();
			break;
		case CHANGE_PASS_C:
			result = changePassword();
			break;
		case CHANNEL_USERS_LIST_C:
			result = getUsersChannelList();
			break;
		case FRIENDS_C:
			result = getFriendsList();
			break;
		case ADD_FRIEND_C:
			result = addFriend();
			break;
		case REMOVE_FRIEND_C:
			result = removeFriend();
			break;
		case CHECK_ONLINE_C:
			result = getFriendsList(true);
			break;
		case SEND_PRIVATE_MESSAGE_C:
			result = sendMessage(PROTOCOL::Message::PRIVATE);
			break;
		case SEND_CHANNEL_MESSAGE_C:
			mode = m_session.getUser().getChannelMode() == CurrentUser::NORMAL_MODE ?
					PROTOCOL::Message::NORMAL : PROTOCOL::Message::EMOTE;
			result = sendMessage(mode);
			break;
		case CHANGE_CHANNEL_MODE_C:
			changeChannelMode();
			break;
		case SEND_MAIL_C:
			result = sendMessage(PROTOCOL::Message::MAIL);
			break;
		case GET_ALL_MAIL_C:
			result = mails();
			break;
		case REMOVE_MAIL_C:
			result = mails(true);
			break;
		case BAN_USER_C:
			result = banUser();
			break;
		case KICK_USER_C:
			result = kickUser();
			break;
		case UNBAN_USER_C:
			result = unbanUser();
			break;
	}
	return result;
}

PROTOCOL::ResponseCode Sender::authorize()
{
	PROTOCOL::ResponseInfo response;
	if(!m_session.isConnected())
	{
		response.code = PROTOCOL::ERROR;
		m_interface -> printOnDialog("You are not connected to the server");
		return response.code;
	}

	if(m_session.isUserLoggedIn())
	{
		response.code = PROTOCOL::ERROR;
		m_interface -> printOnDialog("You are already logged in");
		return response.code;
	}
	PROTOCOL::Authorize auth;
	std::string name;
	std::string password;
	name = m_interface -> getInput("Type your name",MAX_NAME_SIZE);
	if(name.size() < MIN_NAME_SIZE || name.size() > MAX_NAME_SIZE)
	{
		m_interface -> printOnDialog("Invalid user name size");
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	password = m_interface -> getInput("Type your password",MAX_PASS_SIZE);
	if(password.size() < MIN_PASS_SIZE || password.size() > MAX_PASS_SIZE )
	{
		m_interface -> printOnDialog("Invalid user name size");
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::LOGIN;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{
		strcpy(auth.name,name.c_str());
		strcpy(auth.password,password.c_str());
		if(m_session.getSocket().sendRequest(auth,sizeof(auth)) == -1)
		{
			m_interface -> printOnDialog("Server Error");
			response.code = PROTOCOL::ERROR;
			return response.code;
		}
		if(m_session.getSocket().getResponseInfo(response) == -1)
		{
			m_interface -> printOnDialog("Server Error");
			response.code = PROTOCOL::ERROR;
			return response.code;
		}
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("You are logged in now!");
			m_session.setUserName(name);
			m_session.setUserPassword(password);
			m_session.setUserToken(response.token);
			m_session.setUserLoggedIn(true);
			m_interface -> runThreads();
		}
		else
		{
			printErrorCode(response.code);
		}
	}
	return PROTOCOL::OKS;
}

PROTOCOL::ResponseCode Sender::connectToServer()
{
	if( m_session.getSocket().Aton(m_session.getHost()) == 0 )
	{
		m_interface -> printOnDialog("You can not connect to the server");
		return PROTOCOL::ERROR;
	}
	if( m_session.getSocket().Connect() == -1)
	{
		m_interface -> printOnDialog("You can not connect to the server");
		return PROTOCOL::ERROR;
	}
	m_interface -> printOnDialog("Connection is established");
	m_session.setConnected(true);
	return PROTOCOL::OKS;
}

PROTOCOL::ResponseCode Sender::regist()
{
	PROTOCOL::ResponseInfo response;
	if(!m_session.isConnected())
	{
		response.code = PROTOCOL::ERROR;
		m_interface -> printOnDialog("You are not connected to the server");
		return response.code;
	}
	PROTOCOL::Authorize auth;
	std::string name;
	std::string password;
	name = m_interface -> getInput("Type your name",MAX_NAME_SIZE);
	if(name.size() < MIN_NAME_SIZE || name.size() > MAX_NAME_SIZE)
	{
		m_interface -> printOnDialog("Invalid user name size");
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	password = m_interface -> getInput("Type your password",MAX_PASS_SIZE);
	if(password.size() < MIN_PASS_SIZE || password.size() > MAX_PASS_SIZE )
	{
		m_interface -> printOnDialog("Invalid passwords size");
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::REGISTER;
	m_session.getSocket().sendRequest(request,sizeof(request));

	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{

		strcpy(auth.name,name.c_str());
		strcpy(auth.password,password.c_str());
		if(m_session.getSocket().sendRequest(auth,sizeof(auth)) == -1)
		{
			m_interface -> printOnDialog("Server Error");
			response.code = PROTOCOL::ERROR;
			return response.code;
		}
		if(m_session.getSocket().getResponseInfo(response) == -1)
		{
			m_interface -> printOnDialog("Server Error");
			response.code = PROTOCOL::ERROR;
			return response.code;
		}
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("You are registered now!");
		}
		else
		{
			if(response.code == PROTOCOL::WRONG_USER)
			{
				m_interface -> printOnDialog("This user is already registered!");
			}
			else
			{
				printErrorCode(response.code);
			}
		}
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::end()
{

	if(m_session.isConnected())
	{
		m_session.setEnd(true);
		PROTOCOL::Request request;
		request.command = PROTOCOL::ENDS;
		m_session.getSocket().sendRequest(request,sizeof(request));
		PROTOCOL::ResponseInfo response;
		m_session.getSocket().getResponseInfo(response);
		m_session.getSocket().Close();
		return response.code;
	}

	return PROTOCOL::OKS;
}

PROTOCOL::ResponseCode Sender::logout()
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::LOGOUT;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{
		m_session.setUserLoggedIn(false);
		m_interface -> printOnDialog("You are logout");
	}
	else
	{
		m_interface -> printOnDialog("Server problem");
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::channelList()
{
	PROTOCOL::ChannelList response;
	if(checkOnline() == -1)
	{
		response.response = PROTOCOL::ERROR;
		return response.response;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::CHANNEL_LIST;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.response == PROTOCOL::OKS)
	{
		std::vector<std::string> channels;
		channels.push_back("Channels:");
		for(int channel = 0; channel < response.currentChannelNumber; channel++)
		{
			channels.push_back(response.channels[channel].name);
		}
		m_interface -> printOnDialog(channels);
	}
	else
	{
		printErrorCode(response.response);
	}
	return response.response;
}

PROTOCOL::ResponseCode Sender::changeChannel()
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::CHANGE_CHANNEL;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::ChangeChannel changeChannelRequest;
		PROTOCOL::ResponseInfo changeChannelResponse;
		std::string channelName = m_interface -> getInput("Enter channel name",MAX_CHANNEL_NAME_SIZE);
		strcpy(changeChannelRequest.name,channelName.c_str());
		m_session.getSocket().sendRequest(changeChannelRequest,sizeof(changeChannelRequest));
		m_session.getSocket().getResponseInfo(changeChannelResponse);
		if(changeChannelResponse.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("Channel Changed");
		}
		else
		{
			printErrorCode(changeChannelResponse.code);
		}
		return changeChannelResponse.code;
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::changePassword()
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::CHANGE_PASS;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::ChangePassword changePasswordRequest;
		std::string oldPassword = m_interface -> getInput("Enter your old password",MAX_PASS_SIZE);
		std::string newPassword = m_interface -> getInput("Enter your new password",MAX_PASS_SIZE);
		strcpy(changePasswordRequest.oldPassword,oldPassword.c_str());
		strcpy(changePasswordRequest.newPassword,newPassword.c_str());
		m_session.getSocket().sendRequest(changePasswordRequest,sizeof(changePasswordRequest));
		m_session.getSocket().getResponseInfo(response);
		if(response.code != PROTOCOL::OKS)
		{
			printErrorCode(response.code);
		}
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::getUsersChannelList()
{
	PROTOCOL::UsersLists response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::ALL_USERS_FROM_CHANNEL;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		std::vector<std::string> usersNames;
		usersNames.push_back("Users on channel:");
		for(int user = 0; user < response.usersCount; user++)
		{
			usersNames.push_back(response.usersList[user].name);
		}
		m_interface -> printOnDialog(usersNames);
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

int Sender::checkOnline()
{
	if(!(m_session.isConnected()))
	{
		m_interface -> printOnDialog("You are not connected to the server");
		return -1;
	}
	if(!(m_session.isUserLoggedIn()))
	{
		m_interface -> printOnDialog("You are not logged in");
		return -1;
	}
	return 1;
}

PROTOCOL::ResponseCode Sender::getFriendsList(bool online)
{
	PROTOCOL::UsersLists response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = online ? PROTOCOL::CHECK_ONLINE : PROTOCOL::FRIENDS;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		std::vector<std::string> usersNames;
		if(online)
		{
			usersNames.push_back("Your friends online");
		}
		else
		{
			usersNames.push_back("Your friends");
		}
		for(int user = 0; user < response.usersCount; user++)
		{
			std::string friendInfos = response.usersList[user].name;
			friendInfos += response.usersList[user].available ? " Online" : " Offline";
			usersNames.push_back(friendInfos);
		}
		m_interface -> printOnDialog(usersNames);
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::addFriend()
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::ADD_FRIEND;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::Friend friendRequest;
		std::string friendName = m_interface -> getInput("Enter your friend's name",MAX_NAME_SIZE);
		strcpy(friendRequest.name, friendName.c_str());
		m_session.getSocket().sendRequest(friendRequest,sizeof(friendRequest));
		m_session.getSocket().getResponseInfo(response);
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("Your friend is on your friends list now");
		}
		else
		{
			printErrorCode(response.code);
		}
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::removeFriend()
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	request.command = PROTOCOL::REMOVE_FRIEND;
	strcpy(request.token,m_session.getUserToken().c_str());
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::Friend friendRequest;
		std::string friendName = m_interface -> getInput("Enter your friend's name",MAX_NAME_SIZE);

		strcpy(friendRequest.name, friendName.c_str());
		m_session.getSocket().sendRequest(friendRequest,sizeof(friendRequest));
		m_session.getSocket().getResponseInfo(response);
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("Your friend is on your friends list now");
		}
		else
		{
			printErrorCode(response.code);
		}

	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::sendMessage(PROTOCOL::Message::messageMode mode)
{
	PROTOCOL::ResponseInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	strcpy(request.token,m_session.getUserToken().c_str());
	request.command = PROTOCOL::MESSAGE;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponseInfo(response);
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::Message message;
		message.type = mode;
		std::string receiver;
		std::string content;
		if( (mode == PROTOCOL::Message::PRIVATE) || (mode == PROTOCOL::Message::MAIL) )
		{
			receiver = m_interface -> getInput("Enter receiver name",MAX_NAME_SIZE);
			strcpy(message.to,receiver.c_str());
		}
		do
		{
			content = m_interface -> getInput("Enter your message",MAX_MESSAGE_LENGTH);
			if(content.length() > MAX_MESSAGE_LENGTH || content.length() == 0)
			{
				m_interface -> printOnDialog("Wrong message length");
			}
		}while(content.length() > MAX_MESSAGE_LENGTH || content.length() == 0);
		strcpy(message.content,content.c_str());
		m_session.getSocket().sendRequest(message,sizeof(message));
		m_session.getSocket().getResponseInfo(response);
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("Message was sent");
		}

		else
		{
			printErrorCode(response.code);
		}
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::mails(bool toDelete)
{
	PROTOCOL::GetAllMailsInfo response;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	PROTOCOL::Request request;
	strcpy(request.token,m_session.getUserToken().c_str());
	request.command = PROTOCOL::GET_ALL_MAIL;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		std::vector<PROTOCOL::Message> messages;
		int messageCounter = 0;
		while(messageCounter < response.mails)
		{
			if(!m_messageQueue->empty())
			{
				messages.push_back(m_messageQueue->pop());
				messageCounter++;
			}
		}
		if(response.mails > 0)
		{
			if(toDelete)
			{
				removeMail(messages);
			}
			else
			{
				m_interface -> printMails(messages);
			}
		}
		else
		{
			m_interface -> printOnDialog("You don't have any mails in your inbox");
		}
	}
	else
	{
		printErrorCode(response.code);
	}

	return response.code;
}

PROTOCOL::ResponseCode Sender::removeMail(std::vector<PROTOCOL::Message> messages)
{
	PROTOCOL::ResponseInfo response;
	PROTOCOL::Request request;
	int deletedMail = m_interface -> deleteMail(messages);
	if(deletedMail != -1)
	{
		strcpy(request.token, m_session.getUserToken().c_str());
		request.command = PROTOCOL::REMOVE_MAIL;
		m_session.getSocket().sendRequest(request,sizeof(request));
		m_session.getSocket().getResponseInfo(response);
		if(response.code == PROTOCOL::OKS)
		{
			PROTOCOL::RemoveMail removeMailRequest;
			removeMailRequest.messageId = messages[deletedMail].messageId;
			m_session.getSocket().sendRequest(removeMailRequest,sizeof(removeMailRequest));
			m_session.getSocket().getResponseInfo(response);
		}
		if(response.code == PROTOCOL::OKS)
		{
			m_interface -> printOnDialog("Message deleted");
		}
		else
		{
			printErrorCode(response.code);
		}
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::banUser()
{
	PROTOCOL::ResponseInfo response;
	PROTOCOL::Request request;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	strcpy(request.token,m_session.getUserToken().c_str());
	request.command = PROTOCOL::BAN_USER;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::BanUser banUserRequest;
		std::string name;
		do
		{
			name = m_interface -> getInput("Enter name of user you want ban",MAX_NAME_SIZE);
			if(name.length() < MIN_NAME_SIZE)
			{
				m_interface -> printOnDialog("Too short name");
			}
		}while(name.length() < MIN_NAME_SIZE);
		strcpy(banUserRequest.name,name.c_str());
		m_session.getSocket().sendRequest(banUserRequest,sizeof(banUserRequest));
		m_session.getSocket().getResponseInfo(response);
	}
	if(response.code == PROTOCOL::OKS)
	{
		m_interface -> printOnDialog("The user is banned now");
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::unbanUser()
{
	PROTOCOL::ResponseInfo response;
	PROTOCOL::Request request;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	strcpy(request.token,m_session.getUserToken().c_str());
	request.command = PROTOCOL::UNBAN_USER;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::Unban UnbanUserRequest;
		std::string name;
		do
		{
			name = m_interface -> getInput("Enter name of user you want unban",MAX_NAME_SIZE);
			if(name.length() < MIN_NAME_SIZE)
			{
				m_interface -> printOnDialog("Too short name");
			}
		}while(name.length() < MIN_NAME_SIZE);
		strcpy(UnbanUserRequest.name,name.c_str());
		m_session.getSocket().sendRequest(UnbanUserRequest,sizeof(UnbanUserRequest));
		m_session.getSocket().getResponseInfo(response);
	}
	if(response.code == PROTOCOL::OKS)
	{
		m_interface -> printOnDialog("The user is unbanned now");
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

PROTOCOL::ResponseCode Sender::kickUser()
{
	PROTOCOL::ResponseInfo response;
	PROTOCOL::Request request;
	if(checkOnline() == -1)
	{
		response.code = PROTOCOL::ERROR;
		return response.code;
	}
	strcpy(request.token,m_session.getUserToken().c_str());
	request.command = PROTOCOL::KICK_USER;
	m_session.getSocket().sendRequest(request,sizeof(request));
	m_session.getSocket().getResponse(response,sizeof(response));
	if(response.code == PROTOCOL::OKS)
	{
		PROTOCOL::KickUser kickUserRequest;
		std::string name;
		do
		{
			name = m_interface -> getInput("Enter name of user you want kick",MAX_NAME_SIZE);
			if(name.length() < MIN_NAME_SIZE)
			{
				m_interface -> printOnDialog("Too short name");
			}
		}while(name.length() < MIN_NAME_SIZE);
		strcpy(kickUserRequest.name,name.c_str());
		m_session.getSocket().sendRequest(kickUserRequest,sizeof(kickUserRequest));
		m_session.getSocket().getResponseInfo(response);
	}
	if(response.code == PROTOCOL::OKS)
	{
		m_interface -> printOnDialog("The user was kicked");
	}
	else
	{
		printErrorCode(response.code);
	}
	return response.code;
}

void Sender::changeChannelMode()
{
	if(m_session.getUser().getChannelMode() == CurrentUser::NORMAL_MODE)
	{
		m_session.getUser().setChannelMode(CurrentUser::EMOTE_MODE);
		m_interface -> printOnDialog("Channel mode changed to emote");
	}
	else
	{
		m_session.getUser().setChannelMode(CurrentUser::NORMAL_MODE);
		m_interface -> printOnDialog("Channel mode changed to normal");
	}
}

void Sender::printErrorCode(PROTOCOL::ResponseCode errorCode)
{
	switch(errorCode)
	{
		case PROTOCOL::UNAUTHORIZE:
			m_interface -> printOnDialog("You are unauthorized");
			break;
		case PROTOCOL::WRONG_USER:
			m_interface -> printOnDialog("There is no such user");
			break;
		case PROTOCOL::BANNED_USER:
			m_interface -> printOnDialog("You were banned");
			break;
		case PROTOCOL::WRONG_ID:
			m_interface -> printOnDialog("Wrong message id");
			break;
		case PROTOCOL::NOT_FRIEND:
			m_interface -> printOnDialog("This user is not your friend yet");
			break;
		case PROTOCOL::FRIEND_OFFLINE:
			m_interface -> printOnDialog("Your friend is now offline");
			break;
		case PROTOCOL::ALREADY_FRIEND:
			m_interface -> printOnDialog("This user is already your friend");
			break;
		case PROTOCOL::WRONG_PASSWORD:
			m_interface -> printOnDialog("Wrong password");
			break;
		case PROTOCOL::ALREADY_LOGGED_IN:
			m_interface -> printOnDialog("This user is already logged in");
			break;
		case PROTOCOL::CHANNELS_OVERFLOW:
			m_interface -> printOnDialog("Server reach his channels limit!");
			break;
		case PROTOCOL::USERS_OVERFLOW:
			m_interface -> printOnDialog("Channel reach his users limit!");
			break;
		default:
			m_interface -> printOnDialog("Server error");
			break;
	}
}

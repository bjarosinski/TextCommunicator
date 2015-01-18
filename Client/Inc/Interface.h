/*
 * Interface.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <string>
#include <map>
#include <ncurses.h>
#include "MyQueue.cpp"
#include <utility>
#include "MessagesQueues.h"
#include "ChannelThread.h"
#include "MessagesThread.h"
#include "Session.h"
#include "TextUserInterface.h"

enum option
{
	CONNECT_C = 0,
	REGISTER_C,
	LOGIN_C,
	LOGOUT_C,
	CHANNEL_LIST_C,
	CHANGE_CHANNEL_C,
	CHANNEL_USERS_LIST_C,
	FRIENDS_C,
	SEND_PRIVATE_MESSAGE_C,
	SEND_CHANNEL_MESSAGE_C,
	CHANGE_CHANNEL_MODE_C,
	SEND_MAIL_C,
	CHANGE_PASS_C,
	REMOVE_MAIL_C,
	ADD_FRIEND_C,
	REMOVE_FRIEND_C,
	GET_ALL_MAIL_C,

	BAN_USER_C,
	UNBAN_USER_C,
	KICK_USER_C,
	CHECK_ONLINE_C,
	END_C
};

class Interface
{
public:
	Interface(Session* session, MessagesQueues* queues_);
	~Interface();
	void registerOption(option op,std::string);
	option chooseOption();
	void printOptions(int currentOption);
	void setChannel(std::string);
	Thread* getChannelThread();
	void runThreads();
	void endInterface();
	TextUserInterface& getUserInterface();
	std::string getInput(std::string question, int length);
	void printOnDialog(std::string message);
	void printOnDialog(std::vector<std::string>& messages);
	void printMails(std::vector<PROTOCOL::Message>& messages);
	int deleteMail(std::vector<PROTOCOL::Message>& messages);
private:
	void startInterface();
	int printMailsWithAction(std::vector<PROTOCOL::Message>& messages,
			char actionSign = '\0',bool action = false, std::string actionMessage = "");
	void registerOptions();
	std::string m_actualChannel;
	std::map<option,std::string> m_options;
	MyQueue<std::pair<std::string, std::string> > m_channelMessages;
	MyQueue<std::string> m_messages;
	TextUserInterface* m_userInterface;
	MessagesQueues* m_queues;
	ChannelThread* m_channelThread;
	MessagesThread* m_messageThread;
	Session *m_session;
	int m_optionsCount;
};

#endif /* INTERFACE_H_ */

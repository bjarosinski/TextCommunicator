/*
 * Interface.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "Interface.h"

Interface::Interface(Session *session_,MessagesQueues* queues_):m_queues(queues_), m_session(session_)
{
	m_actualChannel = "channel";
	m_optionsCount = 0;
	startInterface();
	m_channelThread = new ChannelThread(m_queues -> getChannelQueue(), m_queues -> getEmoteMessages(), m_session, m_userInterface);
	m_messageThread = new MessagesThread(m_queues -> getMessages(), m_session, m_userInterface);
}

Interface::~Interface()
{
	m_userInterface -> stopInterface();
	if(m_userInterface!=NULL)
	{
		delete m_userInterface;
		m_userInterface = NULL;
	}

}

void Interface::startInterface()
{
	m_userInterface = new TextUserInterface();
    registerOptions();
}

option Interface::chooseOption()
{
	int choose = 0;
	int character;
	do
	{
		printOptions(choose);
		character = getch();
		switch(character)
		{
			case KEY_UP:
			{
				choose = choose == 0 ? m_optionsCount-1 : choose -1 ;
				break;
			}
			case KEY_DOWN:
			{
				choose = choose == m_optionsCount-1 ?  0: choose +1 ;
				break;
			}
		}
	}while(character != 10);
	return static_cast<option>(choose);
}
void Interface::printOptions(int currentOption)
{
	m_userInterface -> clearWindow(optionWindow_E);
	int line = 0;
	std::string currentOptionStr;
	std::map<option,std::string>::iterator optionIterator = m_options.begin();
	for(; optionIterator != m_options.end(); optionIterator++)
	{
		currentOptionStr = optionIterator -> second;
		m_userInterface -> printOption(currentOptionStr,line,currentOption);
		line++;
	}

}

Thread* Interface::getChannelThread()
{
	return m_channelThread;
}

void Interface::registerOption(option op,std::string option_)
{
	m_options[op]=option_;
	m_optionsCount ++;
}

void Interface::registerOptions()
{
	registerOption(CONNECT_C,"Connect");
	registerOption(REGISTER_C,"Register");
	registerOption(LOGIN_C,"Login");
	registerOption(LOGOUT_C,"Log Out");
	registerOption(CHANNEL_LIST_C,"List of Channels");
	registerOption(CHANGE_CHANNEL_C,"Change Channel");
	registerOption(CHANNEL_USERS_LIST_C, "List of Channel Users");
	registerOption(FRIENDS_C,"List of Friends");
	registerOption(SEND_PRIVATE_MESSAGE_C,"Send Private Message");
	registerOption(SEND_CHANNEL_MESSAGE_C,"Send Channel Message");
	registerOption(CHANGE_CHANNEL_MODE_C,"Change Channel Mode");
	registerOption(SEND_MAIL_C,"Send Mail");
	registerOption(CHANGE_PASS_C,"Change Password");
	registerOption(REMOVE_MAIL_C,"Remove Mail");
	registerOption(ADD_FRIEND_C, "Add Friend");
	registerOption(REMOVE_FRIEND_C,"Remove Friend");
	registerOption(GET_ALL_MAIL_C,"Get Mails");
	registerOption(BAN_USER_C,"Ban Other User");
	registerOption(UNBAN_USER_C,"UnBan Other User");
	registerOption(KICK_USER_C,"Kick Other User From Channel");
	registerOption(CHECK_ONLINE_C,"Check Who is Online");
	registerOption(END_C,"Exit");
}

void Interface::runThreads()
{
	m_channelThread -> Run();
	m_messageThread -> Run();
}

void Interface::endInterface()
{
	if(m_channelThread -> isRunning())
	{
		m_channelThread -> Wait();
	}
	if(m_messageThread -> isRunning())
	{
		m_messageThread -> Wait();
	}
	delete m_messageThread;
	delete m_channelThread;
	m_channelThread = NULL;
	m_messageThread = NULL;
}

TextUserInterface&  Interface::getUserInterface()
{
	return *m_userInterface;
}

std::string Interface::getInput(std::string question, int length)
{
	m_userInterface -> clearWindow(optionWindow_E);
	m_userInterface -> addToWindow(optionWindow_E,question);
	m_userInterface -> printDialog();
	return m_userInterface -> getFromDialog(length);
}

void Interface::printOnDialog(std::string message)
{
	m_userInterface -> clearWindow(optionWindow_E);
	m_userInterface -> addToWindow(optionWindow_E,message);
	m_userInterface -> printDialog();
	m_userInterface -> waitOnChar();
}

void Interface::printOnDialog(std::vector<std::string>& messages)
{
	m_userInterface -> clearWindow(optionWindow_E);
	for(std::vector<std::string>::iterator messagesIterator = messages.begin();
				messagesIterator!=messages.end();messagesIterator++)
	{
		m_userInterface -> addToWindow(optionWindow_E,*messagesIterator);
	}
	m_userInterface -> printDialog();
	m_userInterface -> waitOnChar();
}

std::string getDate(time_t date_t)
{
	std::string result;
	std::ostringstream stream;
	tm* date = gmtime(&(date_t));
	stream << date -> tm_hour + 2;
	result += stream.str();
	result += ":";
	stream.str("");
	stream << date -> tm_min;
	result += stream.str();
	result += " ";
	stream.str("");
	stream << date -> tm_mday;
	result += stream.str();
	result += "-";
	stream.str("");
	stream << date -> tm_mon + 1;
	result += stream.str();
	result += "-";
	stream.str("");
	stream << date -> tm_year + 1900;
	result += stream.str();
	return result;
}

int Interface::printMailsWithAction(std::vector<PROTOCOL::Message>& messages,char actionSign,bool action,std::string actionMessage)
{
	int messagesCount = messages.size() - 1;
	std::vector<std::string> lines;
	std::string lineOfMessage = "";
	char pressedSign = '\0';
	for(std::vector<PROTOCOL::Message>::iterator messagesIterator = messages.end();
				messagesIterator != messages.begin(); messagesIterator--)
	{
		m_userInterface -> clearWindow(optionWindow_E);
		lineOfMessage = "Message from: ";
		lineOfMessage += (messagesIterator -1) -> from;
		m_userInterface -> addToWindow(optionWindow_E,lineOfMessage);
		lineOfMessage = "Message date: ";
		lineOfMessage += getDate((messagesIterator -1) -> sendDate);
		m_userInterface -> addToWindow(optionWindow_E,lineOfMessage);
		lineOfMessage = (messagesIterator -1) -> content;
		m_userInterface -> addToWindow(optionWindow_E,lineOfMessage);
		if(action)
		{
			std::ostringstream ss;
			ss << actionSign;
			std::string userInfo = std::string("Press ")+ ss.str() + std::string(" to ") + actionMessage;
			m_userInterface -> addToWindow(optionWindow_E,userInfo);
		}
		if((messagesIterator -1) != messages.begin())
		{
			m_userInterface -> addToWindow(optionWindow_E,"\n\nPress enter to move to next message ");
		}
		m_userInterface -> printDialog();
		pressedSign = m_userInterface -> waitOnChar();
		if(action && pressedSign == actionSign)
		{
			return messagesCount;
		}
		messagesCount--;
	}
	return -1;
}

void  Interface::printMails(std::vector<PROTOCOL::Message>& messages)
{
	printMailsWithAction(messages);
}

int Interface::deleteMail(std::vector<PROTOCOL::Message>& messages)
{
	return printMailsWithAction(messages,'r',true,"remove Mail");
}

/*
 * Sender.h
 *
 *  Created on: 08-08-2013
 *      Author: b.jarosinski
 */

#ifndef SENDER_H_
#define SENDER_H_

#include "protocol.h"
#include "Interface.h"

class Sender
{
public:
	Sender(Session& session_, Interface* interface_ = NULL,MyQueue<PROTOCOL::Message>*messageQueue = NULL);
	~Sender();
	PROTOCOL::ResponseCode sendCommand(option command);
	void setInterface(Interface* interface_);
	void setMessageQueue(MyQueue<PROTOCOL::Message>*);
private:
	int checkOnline();
	void changeChannelMode();
	PROTOCOL::ResponseCode connectToServer();
	PROTOCOL::ResponseCode authorize();
	PROTOCOL::ResponseCode login();
	PROTOCOL::ResponseCode regist();
	PROTOCOL::ResponseCode end();
	PROTOCOL::ResponseCode logout();
	PROTOCOL::ResponseCode channelList();
	PROTOCOL::ResponseCode changeChannel();
	PROTOCOL::ResponseCode changePassword();
	PROTOCOL::ResponseCode getUsersChannelList();
	PROTOCOL::ResponseCode getFriendsList(bool online = false);
	PROTOCOL::ResponseCode addFriend();
	PROTOCOL::ResponseCode removeFriend();
	PROTOCOL::ResponseCode sendMessage(PROTOCOL::Message::messageMode mode = PROTOCOL::Message::NORMAL);
	PROTOCOL::ResponseCode removeMail(std::vector<PROTOCOL::Message> messages);
	PROTOCOL::ResponseCode mails(bool toDelete = false);
	PROTOCOL::ResponseCode banUser();
	PROTOCOL::ResponseCode kickUser();
	PROTOCOL::ResponseCode unbanUser();
	void printErrorCode(PROTOCOL::ResponseCode errorCode);
	Session& m_session;
	Interface* m_interface;
	MyQueue<PROTOCOL::Message>* m_messageQueue;
};

#endif /* SENDER_H_ */

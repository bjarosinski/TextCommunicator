/*
 * MessagesQueues.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jasrosinski
 */

#ifndef MESSAGESQUEUES_H_
#define MESSAGESQUEUES_H_

#include "MyQueue.cpp"
#include <utility>
#include "protocol.h"
#include <string>

class MessagesQueues
{
private:
	MyQueue<std::pair<std::string, std::string> >* channelQueue;
	MyQueue<std::string>* messages;
	MyQueue<PROTOCOL::Message>* deliveredMessages;
	MyQueue<PROTOCOL::Message>* newMessages;
	MyQueue<std::pair<std::string, std::string> >* emoteMessages;
public:
	MessagesQueues();
	~MessagesQueues();
	MyQueue<std::pair<std::string, std::string> >* getChannelQueue();
	MyQueue<std::string>* getMessages();
	MyQueue<PROTOCOL::Message>* getDeliveredMessages();
	MyQueue<PROTOCOL::Message>* getNewMessages();
	MyQueue<std::pair<std::string, std::string> >* getEmoteMessages();
	void resetQueues();
};


#endif /* MESSAGESQUEUES_H_ */

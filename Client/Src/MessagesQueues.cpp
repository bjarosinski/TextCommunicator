#include "MessagesQueues.h"

MessagesQueues::MessagesQueues()
{
	channelQueue = new MyQueue<std::pair<std::string, std::string> >();
	messages =  new MyQueue<std::string>();
	deliveredMessages = new MyQueue<PROTOCOL::Message>();
	newMessages = new MyQueue<PROTOCOL::Message>();
	emoteMessages = new MyQueue<std::pair<std::string, std::string> >();
}

MessagesQueues::~MessagesQueues()
{
	delete messages;
	delete channelQueue;
	delete deliveredMessages;
	delete newMessages;
	delete emoteMessages;
	emoteMessages = NULL;
	messages = NULL;
	channelQueue = NULL;
	deliveredMessages = NULL;
	newMessages = NULL;
}

MyQueue<std::pair<std::string, std::string> >* MessagesQueues::getChannelQueue()
{
	return channelQueue;
}

MyQueue<std::string>* MessagesQueues::getMessages()
{
	return messages;
}

MyQueue<PROTOCOL::Message>* MessagesQueues::getDeliveredMessages()
{
	return deliveredMessages;
}

MyQueue<PROTOCOL::Message>* MessagesQueues::getNewMessages()
{
	return newMessages;
}

MyQueue<std::pair<std::string, std::string> >* MessagesQueues::getEmoteMessages()
{
	return emoteMessages;
}

void MessagesQueues::resetQueues()
{
	delete messages;
	delete channelQueue;
	channelQueue = new MyQueue<std::pair<std::string, std::string> >();
	messages =  new MyQueue<std::string>();
}

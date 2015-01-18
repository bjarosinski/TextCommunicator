/*
 * Client.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "IClient.h"
#include <string>
#include <utility>
#include "MyQueue.h"
#include "MessagesQueues.h"

class Client: public IClient {
public:
	Client(std::string host_ = "127.0.0.1", int port_=6060);
	~Client();
	void run();
};

#endif /* CLIENT_H_ */

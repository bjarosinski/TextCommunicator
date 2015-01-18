/*
 * IClient.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef ICLIENT_H_
#define ICLIENT_H_

#include <string>

class IClient
{
public:
	IClient(std::string host_ = "127.0.0.1", int port_ = 6060):host(host_), port(port_){};
	virtual ~IClient(){};
	virtual void run()=0;
protected:
	int port;
	std::string host;

};

#endif /* ICLIENT_H_ */

/*
 * IServer.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef ISERVER_H_
#define ISERVER_H_

#include <vector>
#include "IUser.h"
#include "IChannel.h"

class IServer
{
public:
	virtual void run() = 0;
	virtual void stop() = 0;
	virtual ~IServer(){};
protected:
	std::vector<IUser*> t_users;
	std::vector<IChannel*> t_channels;
};

#endif /* ISERVER_H_ */

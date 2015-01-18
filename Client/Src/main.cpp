/*
 * main.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "Client.h"
#include <sstream>

int main(int argc, char* argv[])
{
	IClient* client;

	if(argc > 2)
	{
		std::istringstream portString(argv[2]);
		int port = -1;
		portString >> port;
		client = new Client(argv[1], port);
	}
	else
	{
		client = new Client();
	}
	client -> run();
	delete client;
	client = NULL;
}

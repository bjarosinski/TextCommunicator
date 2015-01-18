/*
 * main.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include <iostream>
#include <cstdio>
#include <sstream>
#include "Server.h"
#include <signal.h>

IServer* server = NULL;

void my_terminate(int param)
{
	std::cout << "Server terminate\n";
	if(server!=NULL)
	{
		server -> stop();
		delete server;
		server = NULL;
	}
}

int main(int argc, char* argv[])
{
	void (*prev_fn)(int);

	prev_fn = signal (SIGINT,my_terminate);
	if (prev_fn==SIG_IGN) signal (SIGTERM,SIG_IGN);

	if(argc<2)
	{
		std::cout << "Use server -portNumber" << std::endl;
		perror("Invalid arguments number");
		return -1;
	}


	std::istringstream portString(argv[1]);
	int port = -1;
	portString >> port;
	if( (port < 0) || (port > 65535))
	{
		std::cout << "Invalid port number, use one of <0-65535>" << std::endl;
		perror("Invalid arguments");
		return -1;
	}
	server = new Server(port);
	server -> run();
	if(server != NULL)
	{
		delete server;
		server = NULL;
	}
}

/*
 * CurrentUser.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef CURRENTUSER_H_
#define CURRENTUSER_H_

#include <string>
#include <pthread.h>
#include "MessagesQueues.h"

class CurrentUser
{
public:
	enum channelMode
	{
		EMOTE_MODE,
		NORMAL_MODE
	};
	CurrentUser();
	~CurrentUser();
	std::string getName()const;
	std::string getPassword()const;
	std::string getToken()const;
	void setPassword(std::string password_);
	void setName(std::string name_);
	void setToken(std::string token_);
	bool isLoggedIn()const;
	void setLoggedIn(bool loggedIn_);
	channelMode getChannelMode();
	void setChannelMode(CurrentUser::channelMode mode_);
private:
	channelMode m_mode;
	std::string m_name;
	std::string m_password;
	std::string m_token;
	bool m_loggedIn;
	pthread_mutex_t m_nameMutex;
	pthread_mutex_t m_passwordMutex;
	pthread_mutex_t m_tokenMutex;
	pthread_mutex_t m_loggedInMutex;
};

#endif /* CURRENTUSER_H_ */

/*
 * Validator.cpp
 *
 *  Created on: 09-08-2013
 *      Author: b.jarosinski
 */

#include "Validator.h"

Validator::Validator(ServerSession& session_):m_session(session_)
{

}

Validator::~Validator()
{
}

validCode Validator::validateName(std::string name)
{
	validCode result = OKI;
	if(name.length() < MIN_NAME_SIZE)
	{
		result = TOO_SHORT;
	}

	if(name.length() > MAX_NAME_SIZE)
	{
		result = TOO_LONG;
	}

	if(result == OKI && ( name[0] < 'A' || name[0] > 'z'))
	{
		result = INAPPROPRIATE;
	}
	return result;
}

validCode Validator::validatePassword(std::string password)
{
	validCode result = OKI;
	if(password.length() < MIN_PASS_SIZE)
	{
		result = TOO_SHORT;
	}

	if(password.length() > MAX_PASS_SIZE)
	{
		result = TOO_LONG;
	}
	return result;
}

validCode Validator::checkUserLoggData(std::string name, std::string password)
{
	validCode result = OKI;
	if(!m_session.isUser(name))
	{
		result = USER_NOT_EXIST;
	}
	else
	{
		if(!(m_session.getUser(name)-> getPassword() == password))
		{
			result = PASSWORD_WRONG;
		}
		else if(m_session.getUser(name) -> isLoggedIn())
		{
			result = USER_ALREADY_LOGGED_IN;
		}
		else if(m_session.getUser(name) -> isBanned())
		{
			return USER_BANNED;
		}
	}
	return result;
}

validCode Validator::checkUser(std::string userName,std::string token)
{
	if(!(m_session.getUser(userName) -> checkToken(token)))
	{
		m_session.getLogger().clientResponse(userName,PROTOCOL::UNAUTHORIZE);
		m_session.releaseLogger();
		return USER_UNAUTHORIZED;
	}
	else if(m_session.getUser(userName) -> isBanned())
	{
		m_session.getLogger().clientResponse(userName,PROTOCOL::BANNED_USER);
		m_session.releaseLogger();
		return USER_BANNED;
	}
	return OKI;
}

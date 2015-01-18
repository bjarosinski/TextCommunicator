/*
 * Validator.h
 *
 *  Created on: 09-08-2013
 *      Author: b.jarosinski
 */

#ifndef VALIDATOR_H_
#define VALIDATOR_H_

#include <string>
#include "protocol.h"
#include "ServerSession.h"

enum validCode
{
	OKI,
	TOO_SHORT,
	TOO_LONG,
	INAPPROPRIATE,
	USER_NOT_EXIST,
	PASSWORD_WRONG,
	USER_ALREADY_LOGGED_IN,
	USER_BANNED,
	USER_UNAUTHORIZED
};

class Validator
{
public:
	Validator(ServerSession& );
	~Validator();
	validCode validateName(std::string);
	validCode validatePassword(std::string);
	validCode checkUserLoggData(std::string, std::string);
	validCode checkUser(std::string name,std::string token);
private:
	ServerSession& m_session;
};

#endif /* VALIDATOR_H_ */

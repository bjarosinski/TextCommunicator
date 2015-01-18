/*
 * User.h
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#ifndef USER_H_
#define USER_H_

#include "IUser.h"
#include <vector>

class User: public IUser
{
public:
	User(std::string name_,std::string password_);
	virtual ~User();
	void addFriend(IUser* newFriend);
	void removeFriend(IUser* oldFriend);
	bool isFriend(IUser* checkedFriend);
	std::vector<std::string> getFriendsList(bool online=false);
	void addMail(PROTOCOL::Message message);
private:
	std::vector<IUser*> m_friends;
};

#endif /* USER_H_ */

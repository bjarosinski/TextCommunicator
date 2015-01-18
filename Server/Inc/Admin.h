/*
 * Admin.h
 *
 *  Created on: 06-08-2013
 *      Author: SW4
 */

#ifndef ADMIN_H_
#define ADMIN_H_

#include "IUser.h"

class Admin: public IUser {
public:
	Admin();
	virtual ~Admin();

};

#endif /* ADMIN_H_ */

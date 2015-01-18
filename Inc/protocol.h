#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/**
*	@file          cpp_projectcpp
* @author      Pawel Ostrowski
* @author      Bartlomiej Jarosinski
* @author      Michal Admek
* @author      Adam Szczepanski
* @author      Daniel Figat
*============================================================================

* Version     :  0.7
* Copyright   : Your copyright notice
* Description : Header file with define request/response structures
*============================================================================
*/

#include <time.h>

namespace PROTOCOL
{
	#define MAX_PASS_SIZE  10
	#define MIN_PASS_SIZE   5

	#define MIN_NAME_SIZE 4
	#define MAX_NAME_SIZE 10

	#define MAX_SUBJECT_SIZE 50

	#define MAX_CONNECTIONS 10
	#define MAX_CHANNELS_NUMBER 10
	#define MAX_USERS_ON_CHANNEL 20
	#define MAX_MESSAGE_LENGTH 200

	#define TOKEN_SIZE 20
	#define MAX_CHANNEL_NAME_SIZE 100
	#define MAX_USERS_LIST_SIZE 20//MAX_CHANNELS_NUMBER*MAX_USERS_ON_CHANNEL



	/**
	* Request command
	*  Commands send with first call
	*/
	enum RequestCommand
	{
		ACK = 0,
		REGISTER,
		LOGIN,
		LOGOUT,
		CHANNEL_LIST,
		CHANGE_CHANNEL,
		ENDS,
		ALL_AVAILABLE_USERS,
		ALL_USERS_FROM_CHANNEL,
		FRIENDS,
		MESSAGE,
		CHANGE_PASS,
		REMOVE_MAIL,
		ADD_FRIEND,
		REMOVE_FRIEND,
		GET_ALL_MAIL,

		BAN_USER,
		UNBAN_USER,
		KICK_USER,
		CHECK_ONLINE,
	};


	/**
	* Response Codes are used in response message.
	* In case of success, value OK is send, otherwise
	* Error code value is send
	*/
	enum ResponseCode
	{
		OKS = 0,
		WRONG_COMMAND,
		WRONG_USER,
		WRONG_PASSWORD,
		WRONG_USER_NAME,
		WRONG_PASS_STRUCTURE,
		UNAUTHORIZE,
		USERS_OVERFLOW,
		CHANNELS_OVERFLOW,
		WRONG_ID,
		BANNED_USER,
		ALREADY_LOGGED_IN,
		ALREADY_FRIEND,
		FRIEND_OFFLINE,
		NOT_FRIEND,
		ERROR
	};

	//------------------Request structures--------------------

	struct RequestInterface
	{

	};
	/**
	* Base request struct, contains token value and request command.
	* Send always during first call
	* During registration and loggin, token value is empty
	*/
	struct Request: public RequestInterface
	{
		char token[TOKEN_SIZE];
		RequestCommand command;
	};


	/**
	* struct used during
	* - register
	* -login
	*/
	struct  Authorize: public RequestInterface
	{
		char name[MAX_NAME_SIZE];
		char password[MAX_PASS_SIZE];
	};

	/**
	* struct used during change password procedure,
	* user input old password and new password
	* user identificator ( user name) is not required
	* because user is recognise by token value
	*/
	struct ChangePassword: public RequestInterface
	{
		char  oldPassword[MAX_PASS_SIZE];
		char newPassword[MAX_PASS_SIZE];
	};

	/**
	* struct used during remove message from serwer,
	* user give just message identifier
	*/
	struct RemoveMail: public RequestInterface
	{
		int messageId;
	};

	/**
	* Friend structure used during call add and remove friend method.
	*/
	struct Friend: public RequestInterface
	{
		char name[MAX_NAME_SIZE];
	};

	/**
	* struct used during ban user call method
	* @name : name of the user
	* @endTime : ammpunt of time (ban end time)
	*/
	struct BanUser: public RequestInterface
	{
		char name[MAX_NAME_SIZE];
		time_t endTime;
	};

	/**
	* struct used during kick user from channel
	* @userName : name of the user
	* @channelName : name of the channel
	*/
	struct KickUser: public RequestInterface
	{
		char name[MAX_NAME_SIZE];
		char channelName[MAX_CHANNEL_NAME_SIZE];
	};

	/**
	*	Method call during unban user
	*/
	struct Unban: public RequestInterface
	{
		char name[MAX_NAME_SIZE];
	};

	/**
	*	Method call during change channel,
	*  @name: channel name
	*/
	struct ChangeChannel: public RequestInterface
	{
		char name[MAX_CHANNEL_NAME_SIZE];
	};

	struct RegisterReceiver: public RequestInterface
	{
		char token[TOKEN_SIZE];
		char name[MAX_NAME_SIZE];
	};

	//------------------Response structures--------------------

	struct ResponseInterface
	{

	};
	/**
	*	Common response for methods.
	*  Contain response code ( ok, or error code)
	*  and current value for the token.
	*  If token value is different than current stored by user,
	* 	old value must be override.
	*/
	struct ResponseInfo: public ResponseInterface
	{
		char token[TOKEN_SIZE];
		ResponseCode code;
	};

	struct GetAllMailsInfo:public ResponseInfo
	{
		int mails;
	};
	/**
	* Struct used to return all channel lists
	*/
	struct ChannelList: public ResponseInterface
	{
		struct ChannelInfo
		{
			char name[MAX_CHANNEL_NAME_SIZE];
			int usersCount;
		};

		ResponseCode response;
		ChannelInfo channels[MAX_CHANNELS_NUMBER];
		int currentChannelNumber;
	};

	/**
	* Structure return during call this command:
	*	ALL_AVAILABLE_USERS,
	*	ALL_USERS_FROM_CHANNEL,
	*	FRIENDS
	*  Contains current user Count. For each item bigger than usersCount,
	*  list is empty
	*/
	struct UsersLists: public ResponseInterface
	{
		struct UserInfo
		{
			char name[MAX_NAME_SIZE];
			bool available;
			char channelName[MAX_CHANNEL_NAME_SIZE];
		};
		ResponseCode code;
		UserInfo usersList[MAX_USERS_LIST_SIZE ];
		int usersCount;
	};



	//------------------ Message structure ------------------------------------
	//------------------ The same for request and response --------------------

	/**
	* Message struct used to send any kind of message
	* @sendDate : leave empty during send message, field filled by server automatically
	* @messageId: leave empty during send message, field filled by server automatically
	* @from: leave empty during send message, field filled by server automatically
	*/
	struct Message: public RequestInterface
	{
		enum messageMode{ PRIVATE, MAIL, NORMAL, EMOTE };
		Message::messageMode type;
		time_t sendDate;

		int messageId;
		bool delivered;
		char subject[50];
		char from[MAX_NAME_SIZE];
		char to[MAX_NAME_SIZE];
		char content[MAX_MESSAGE_LENGTH];
	};
};
#endif /* PROTOCOL_H_ */

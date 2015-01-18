/*
 * TextUserInterface.h
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#ifndef TEXTUSERINTERFACE_H_
#define TEXTUSERINTERFACE_H_

#include <ncurses.h>
#include <pthread.h>
#include <string>
#include <deque>
#include <sstream>

#define MAX_MESSAGES 20

enum window
{
	optionWindow_E = 0,
	messageWindow_E,
	channelWindow_E
};
class TextUserInterface {
public:
	TextUserInterface();
	~TextUserInterface();
	void addToWindow(window win, std::string message);
	void printOption(std::string, int which, int current);
	void clearWindow(window win);
	void printDialog();
	std::string getFromDialog(int length);
	void stopInterface();
	char waitOnChar();
private:
	void initializeWindows();
	void reprintChannelMessages();
	void reprintMessages();
	std::string getInput(int length,int line = 0);
	WINDOW *createWindow(int height, int width, int starty, int startx);
	void destroyWindow(WINDOW *local_win);
	pthread_mutex_t m_mutex;
	WINDOW* m_optionWindow;
	WINDOW* m_messageWindow;
	WINDOW* m_channelWindow;
	std::deque<std::string> m_messages;
	std::deque<std::string> m_channelMessages;
	std::deque<std::string> m_optionMessages;
	int m_optionLine;
};


#endif /* TEXTUSERINTERFACE_H_ */

/*
 * TexUserInterface.cpp
 *
 *  Created on: 07-08-2013
 *      Author: b.jarosinski
 */

#include "TextUserInterface.h"
#include <cstring>
#include <string.h>

#define MAX_INPUT_LENGTH 200

TextUserInterface::TextUserInterface()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&m_mutex,&attr);
	initializeWindows();
	m_optionLine = 0;
}

TextUserInterface::~TextUserInterface()
{
	stopInterface();
}

void TextUserInterface::stopInterface()
{
	destroyWindow(m_messageWindow);
	destroyWindow(m_channelWindow);
	destroyWindow(m_optionWindow);
	m_messageWindow = NULL;
	m_channelWindow = NULL;
	m_optionWindow = NULL;
	clear();
	refresh();
	clear();
	refresh();
	endwin();
}

void TextUserInterface::initializeWindows()
{
	initscr();
	cbreak();
	refresh();
	m_messageWindow = createWindow((LINES / 2), (COLS /2),0,0);
	m_channelWindow = createWindow((LINES / 2), (COLS /2)-1,0, (COLS /2)+1);
	m_optionWindow = createWindow((LINES / 2)  , 0,(LINES / 2), 0);
	keypad(stdscr,TRUE);
	keypad(m_optionWindow, TRUE);
	noecho();
	wprintw(m_messageWindow,"Messages:");
	wprintw(m_channelWindow,"Channel:");
	wrefresh(m_messageWindow);
	wrefresh(m_channelWindow);
    wrefresh(m_optionWindow);

}
void TextUserInterface::addToWindow(window win, std::string message)
{

	switch (win)
	{
		case optionWindow_E:
			m_optionMessages.push_back(message);
			m_optionLine ++;
			break;
		case messageWindow_E:
			if(m_messages.size() == MAX_MESSAGES)
			{
				m_messages.pop_front();
			}
			m_messages.push_back(message);
			reprintMessages();
			break;
		case channelWindow_E:
			if(m_channelMessages.size() == MAX_MESSAGES)
			{
				m_channelMessages.pop_front();
			}
			m_channelMessages.push_back(message);
			reprintChannelMessages();
			break;
	}
}

void TextUserInterface::printOption(std::string currentOptionStr, int which, int current)
{
	pthread_mutex_lock(&m_mutex);
	if(which == current)
	{
		wattron(m_optionWindow,A_REVERSE);
	}
	int col = (COLS/2) - (currentOptionStr.length()/2);
	wmove(m_optionWindow,which + 1, col);
	wprintw(m_optionWindow,currentOptionStr.c_str());
	if(which == current)
	{
		wattroff(m_optionWindow,A_REVERSE);
	}
	wrefresh(m_optionWindow);
	pthread_mutex_unlock(&m_mutex);
}

WINDOW *TextUserInterface::createWindow(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win,0,0);
	wrefresh(local_win);
	wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(local_win);
	return local_win;
}

void TextUserInterface::destroyWindow(WINDOW *local_win)
{
	wrefresh(local_win);
	delwin(local_win);
}

void TextUserInterface::reprintMessages()
{
	pthread_mutex_lock(&m_mutex);
	wclear(m_messageWindow);
	box(m_messageWindow,0,0);
	wrefresh(m_messageWindow);
	wborder(m_messageWindow, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(m_messageWindow);
	wprintw(m_messageWindow,"Messages:");
	wrefresh(m_messageWindow);
	int line = 0;
	std::deque<std::string>::iterator messagesIterator = m_messages.begin();
	for( ; messagesIterator!=m_messages.end(); messagesIterator++)
	{
		if(*messagesIterator !="")
		{
			wmove(m_messageWindow,line + 1,1);
			wprintw(m_messageWindow,messagesIterator -> c_str());
			if(messagesIterator -> length() > COLS / 2 - 3)
			{
				line ++;
			}
			line++;
		}

	}
	wrefresh(m_messageWindow);
	pthread_mutex_unlock(&m_mutex);
}

void TextUserInterface::reprintChannelMessages()
{
	pthread_mutex_lock(&m_mutex);
	wclear(m_channelWindow);
	box(m_channelWindow,0,0);
	wrefresh(m_channelWindow);
	wborder(m_channelWindow, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(m_channelWindow);
	wprintw(m_channelWindow,"Channel:");
	wrefresh(m_channelWindow);
	int line = 0;
	std::deque<std::string>::iterator channelMessagesIterator = m_channelMessages.begin();
	for( ; channelMessagesIterator!=m_channelMessages.end(); channelMessagesIterator++)
	{
		if(*channelMessagesIterator != "")
		{
			wmove(m_channelWindow,line + 1,1);
			wprintw(m_channelWindow,channelMessagesIterator -> c_str());
			if(channelMessagesIterator -> length() > COLS / 2 - 3)
			{
				line ++;
			}
			line++;
		}

	}

	wrefresh(m_channelWindow);
	pthread_mutex_unlock(&m_mutex);
}

void TextUserInterface::clearWindow(window win)
{
	pthread_mutex_lock(&m_mutex);
	switch(win)
	{
		case optionWindow_E:
			wclear(m_optionWindow);
			wrefresh(m_optionWindow);
			wborder(m_optionWindow, '|', '|', '-', '-', '+', '+', '+', '+');
			wrefresh(m_optionWindow);
			break;
		case messageWindow_E:
			wclear(m_messageWindow);
			wrefresh(m_messageWindow);
			break;
		case channelWindow_E:
			wclear(m_channelWindow);
			wrefresh(m_channelWindow);
			break;
	}
	pthread_mutex_unlock(&m_mutex);
}

void TextUserInterface::printDialog()
{
	pthread_mutex_lock(&m_mutex);
	m_optionLine = 0;
	for(std::deque<std::string>::iterator messageIterator= m_optionMessages.begin();
			messageIterator != m_optionMessages.end(); messageIterator++)
	{
		wmove(m_optionWindow,m_optionLine+1,(COLS/2)-(messageIterator -> length()/2));
		wprintw(m_optionWindow,messageIterator -> c_str());
		m_optionLine++;
	}
	wrefresh(m_optionWindow);
	pthread_mutex_unlock(&m_mutex);
}

std::string TextUserInterface::getFromDialog(int length)
{
	std::string result;
	pthread_mutex_lock(&m_mutex);
	char buff[MAX_INPUT_LENGTH];
	wmove(m_optionWindow,++m_optionLine ,(COLS/2) - length/2);
	result = getInput(length,m_optionLine);// buff;
	m_optionMessages.pop_front();
	pthread_mutex_unlock(&m_mutex);
	return result;
}

char TextUserInterface::waitOnChar()
{
	char result;
	pthread_mutex_lock(&m_mutex);
	result = wgetch(m_optionWindow);
	m_optionMessages.clear();
	pthread_mutex_unlock(&m_mutex);
	return result;
}

std::string TextUserInterface::getInput(const int length, int line)
{
	char* buff = new char[length];//('\0');
	for(int charCount = 0; charCount < length; charCount++)
	{
		buff[charCount] = '\0';
	}
	char c;
	int currentSign = 0;
	keypad(m_optionWindow, FALSE);
	keypad(m_optionWindow, TRUE);
	int lineNumber = line;
	int signInLine = 0;
	int startCol = COLS < length ? COLS : COLS/2;
	do
	{
		std::string info = "Signs left: ";
		std::ostringstream ss;
		ss << length - currentSign -1;
		info += ss.str();
		mvwprintw(m_optionWindow,lineNumber+1,COLS/2 - info.length()/2 -1,"                       ");
		mvwprintw(m_optionWindow,lineNumber+1,COLS/2 - info.length()/2,info.c_str());
		wmove(m_optionWindow,lineNumber ,(startCol - length/2) + signInLine);
		c = wgetch(m_optionWindow);

		if(c == 7 && currentSign > 0)
		{
			signInLine --;
			currentSign--;
			buff[currentSign] = '\0';
			wprintw(m_optionWindow,"\b \b");
		}
		else if( (currentSign<length-1) && ((c >= ' ')&&(c <= 'z')) )
		{
			buff[currentSign] = c;
			currentSign++;
			signInLine++ ;
			wprintw(m_optionWindow,"%c",c);
		}
		//Line breaking
		if((startCol - length/2) + signInLine > COLS -1)
		{
			mvwprintw(m_optionWindow,lineNumber+1,COLS/2 - info.length()/2 -1,"                       ");
			signInLine = 0;
			lineNumber++;
		}
		//Line up when backspace
		if(signInLine < 0)
		{
			mvwprintw(m_optionWindow,lineNumber+1,COLS/2 - info.length()/2 -1,"                       ");
			signInLine = (COLS -1) - (startCol - length/2);
			lineNumber--;
		}
	}while(c!='\n');
	std::string result = buff;
	delete buff;
	buff = NULL;
	return result;
}

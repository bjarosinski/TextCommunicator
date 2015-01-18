/*
 * SendThread.cpp
 *
 *  Created on: 06-08-2013
 *      Author: b.jarosinski
 */

#include "SendThread.h"
#include "protocol.h"
#include "Interface.h"

SendThread::SendThread(Session& session_,MessagesQueues* queues_):
			m_session(session_), m_queues(queues_),m_sender(m_session)
{
		m_interface = new Interface(&m_session,m_queues);
		m_sender.setInterface(m_interface);
		m_sender.setMessageQueue(m_queues -> getDeliveredMessages());
		m_receiver = new RecieveThread(m_session,m_queues);
		m_receiver -> Run();
}

SendThread::~SendThread()
{
	if(m_receiver != NULL)
	{
		m_receiver -> Wait();
		delete m_receiver;
		m_receiver = NULL;
	}
	if(m_interface != NULL)
	{
		delete m_interface;
		m_interface = NULL;
	}
	m_queues = NULL;
}

void SendThread::func()
{

	if( m_session.getSocket().GetSockfd() == -1 )
	{
		perror("socket error");
		return;
	}

	option choose;
	PROTOCOL::ResponseCode code;
	do
	{
		choose = m_interface -> chooseOption();
		code = m_sender.sendCommand(choose);
	}while(choose != END_C);
	m_interface -> endInterface();
	delete m_interface;
	m_interface = NULL;
	m_session.setEnd(true);
}

void SendThread::startReceiver()
{
	m_receiver = new RecieveThread(m_session,m_queues);
	m_receiver -> Run();
}

void SendThread::endReceiver()
{
	m_receiver -> Wait();
	delete m_receiver;
	m_receiver = NULL;
}

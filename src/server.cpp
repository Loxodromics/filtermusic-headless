/**
 *  server.cpp
 *  %PROJECT_NAME%
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#include "server.h"
#include "serverthread.h"

#include <QRandomGenerator>

#include <stdlib.h>

namespace LFD {

namespace filtermusic {

Server::Server(QObject* parent)
	: QTcpServer(parent)
{
	messages << tr("Message 1") << tr("Message 2") << tr("Message 3") << tr("Message 4") << tr("Message 5")
			 << tr("Message 6") << tr("Message 7");
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	QString message = messages.at(QRandomGenerator::global()->bounded(messages.size()));
	ServerThread* thread = new ServerThread(socketDescriptor, message, this);
	connect(thread, &ServerThread::finished, thread, &ServerThread::deleteLater);
	thread->start();
}

}	/// namespace filtermusic

}	/// namespace LFD

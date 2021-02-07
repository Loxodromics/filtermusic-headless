/**
 *  serverthread.cpp
 *  %PROJECT_NAME%
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#include "serverthread.h"

#include <QtNetwork>

namespace LFD {

namespace filtermusic {

ServerThread::ServerThread(int socketDescriptor, const QString& fortune, QObject* parent)
	: QThread(parent)
	, socketDescriptor(socketDescriptor)
	, text(fortune)
{
}

void ServerThread::run()
{
	QTcpSocket tcpSocket;

	if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
		emit error(tcpSocket.error());
		return;
	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << text;

	tcpSocket.write(block);
	tcpSocket.disconnectFromHost();
	tcpSocket.waitForDisconnected();
}

}	/// namespace filtermusic

}	/// namespace LFD

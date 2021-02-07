/**
 *  serverthread.h
 *  %PROJECT_NAME%
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#pragma once

#include <QTcpSocket>
#include <QThread>

namespace LFD {

namespace filtermusic {

class ServerThread : public QThread {
	Q_OBJECT

public:
	ServerThread(int socketDescriptor, const QString& fortune, QObject* parent);

	void run() override;

signals:
	void error(QTcpSocket::SocketError socketError);

private:
	int socketDescriptor;
	QString text;
};

}	/// namespace filtermusic

}	/// namespace LFD

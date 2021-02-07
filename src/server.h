/**
 *  server.h
 *  %PROJECT_NAME%
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */

#pragma once

#include <QStringList>
#include <QTcpServer>

namespace LFD {

namespace filtermusic {

class Server : public QTcpServer {
	Q_OBJECT

public:
	Server(QObject* parent = nullptr);

protected:
	void incomingConnection(qintptr socketDescriptor) override;

private:
	QStringList messages;
};

}	/// namespace filtermusic

}	/// namespace LFD

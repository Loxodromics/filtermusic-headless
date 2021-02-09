/**
 *  server.h
 *  %PROJECT_NAME%
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */

#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>

class QWebSocketServer;
class QWebSocket;
class QString;

namespace LFD {

namespace filtermusic {

class Server : public QObject {
	Q_OBJECT

public:
	Server(quint16 port, QObject* parent = nullptr);
	~Server() override;

private slots:
	void onNewConnection();
	void processMessage(const QString& message);
	void socketDisconnected();

private:
	QWebSocketServer* m_pWebSocketServer;
	QList<QWebSocket*> m_clients;
};

}	/// namespace filtermusic

}	/// namespace LFD

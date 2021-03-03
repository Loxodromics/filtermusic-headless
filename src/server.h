/**
 *  server.h
 *  filtermusic-headless
 *
 *  Created by philipp on 06.02.2021.
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

signals:
	void playStation(const QString& stationUrl);
	void stop();
	void setVolume(const int volume);
	void updateState();

public slots:
	void stopped();
	void sendMessage(const QString& message);

private slots:
	void onNewConnection();
	void processMessage(const QString& message);
	void socketDisconnected();

protected:
	QWebSocketServer* m_webSocketServer;
	QList<QWebSocket*> m_clients;

};

}	/// namespace filtermusic

}	/// namespace LFD

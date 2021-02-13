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
	void volume(const int volume);

public slots:
	void playingTitle(const QString& title);
	void stopped();
	void setVolume(const int volume);

private slots:
	void onNewConnection();
	void processMessage(const QString& message);
	void socketDisconnected();

protected:
	void sendMessage(const QString& message);

	QWebSocketServer* m_pWebSocketServer;
	QList<QWebSocket*> m_clients;
};

}	/// namespace filtermusic

}	/// namespace LFD

/**
 *  server.cpp
 *  filtermusic-headless
 *
 *  Created by philipp2 on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#include "server.h"

#include <QtCore>
#include <QtWebSockets>

#include <cstdio>

namespace LFD {

namespace filtermusic {

static QString getIdentifier(QWebSocket* peer)
{
	return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(), QString::number(peer->peerPort()));
}

Server::Server(quint16 port, QObject* parent)
	: QObject(parent)
	, m_pWebSocketServer(new QWebSocketServer(QStringLiteral("FiltermusicServer"), QWebSocketServer::NonSecureMode, this))
{
	if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
		QTextStream(stdout) << "filtermusic Server listening on port " << port << '\n';
		connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
	}
}

Server::~Server() {
	m_pWebSocketServer->close();
}

void Server::playingTitle(const QString& title)
{

}

void Server::stopped()
{

}

void Server::setVolume(const int volume)
{

}

void Server::onNewConnection()
{
	auto pSocket = m_pWebSocketServer->nextPendingConnection();
	QTextStream(stdout) << getIdentifier(pSocket) << " connected!\n";
	pSocket->setParent(this);

	connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processMessage);
	connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

	m_clients << pSocket;
}

void Server::processMessage(const QString& message)
{
	QString answer = "no answer";
	if (message.startsWith("play:")) {
		QString stationUrl = message.mid(5);
		answer = QStringLiteral("Trying to play: ") + stationUrl + QStringLiteral("...");
		emit playStation(stationUrl);
	}
	else if (message.startsWith("stop")) {
		answer = QStringLiteral("Stopping...");
	}
	else if (message.startsWith("volume:")) {
		QString volume = message.right(7);
		answer = QStringLiteral("Volume set to: ") + volume;
	}
	else {
		answer = QStringLiteral("¯\\_(ツ)_/¯");
	}
	QWebSocket* pSender = qobject_cast<QWebSocket*>(sender());
	this->sendMessage(answer);
}

void Server::socketDisconnected()
{
	QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
	QTextStream(stdout) << getIdentifier(pClient) << " disconnected!\n";
	if (pClient) {
		m_clients.removeAll(pClient);
		pClient->deleteLater();
	}
}

void Server::sendMessage(const QString& message)
{
	for (QWebSocket* pClient : qAsConst(m_clients)) {
		pClient->sendTextMessage(message);
	}
}

}	/// namespace filtermusic

}	/// namespace LFD

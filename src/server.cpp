/**
 *  server.cpp
 *  %PROJECT_NAME%
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
	QWebSocket* pSender = qobject_cast<QWebSocket*>(sender());
	for (QWebSocket* pClient : qAsConst(m_clients)) {
		pClient->sendTextMessage("Blaaaa");
	}
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

}	/// namespace filtermusic

}	/// namespace LFD

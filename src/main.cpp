/**
 *  main.cpp
 *  filtermusic-headless
 *
 *  Created by philipp on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */

#include "server.h"
#include <QCoreApplication>
#include <QtNetwork>
#include <QDebug>

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	LFD::filtermusic::Server server;

	if (!server.listen()) {
		return -1;
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
			ipAddress = ipAddressesList.at(i).toString();
			break;
		}
	}

	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	qDebug() << "The server is running on\n\nIP:" << ipAddress << " Port:" << server.serverPort();

	return a.exec();
}

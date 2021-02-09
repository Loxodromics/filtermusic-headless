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

	LFD::filtermusic::Server server(54321);

	return a.exec();
}

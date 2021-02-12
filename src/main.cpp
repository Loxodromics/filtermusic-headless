/**
 *  main.cpp
 *  filtermusic-headless
 *
 *  Created by philipp on 06.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */

#include "server.h"
#include "audio/audioplayer.h"
#include <QCoreApplication>
#include <QtNetwork>
#include <QDebug>

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	LFD::filtermusic::Server server(54321);
	LFD::AudioPlayer audioPlayer;

	QObject::connect(&server, &LFD::filtermusic::Server::playStation,
			&audioPlayer, &LFD::AudioPlayer::setStationUrl);
	QObject::connect(&server, &LFD::filtermusic::Server::stop,
			&audioPlayer, &LFD::AudioPlayer::pause);
	QObject::connect(&server, &LFD::filtermusic::Server::volume,
			&audioPlayer, &LFD::AudioPlayer::setVolume);

	QObject::connect(&audioPlayer, &LFD::AudioPlayer::newTitle,
			&server, &LFD::filtermusic::Server::playingTitle);

	return a.exec();
}

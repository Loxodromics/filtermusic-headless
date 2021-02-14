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
	LFD::filtermusic::AudioPlayer audioPlayer;

	QObject::connect(&server, &LFD::filtermusic::Server::playStation,
			&audioPlayer, &LFD::filtermusic::AudioPlayer::setStationUrl);
	QObject::connect(&server, &LFD::filtermusic::Server::stop,
			&audioPlayer, &LFD::filtermusic::AudioPlayer::pause);
	QObject::connect(&server, &LFD::filtermusic::Server::setVolume,
			&audioPlayer, &LFD::filtermusic::AudioPlayer::setVolume);

	QObject::connect(&audioPlayer, &LFD::filtermusic::AudioPlayer::newTitle,
			&server, &LFD::filtermusic::Server::sendMessage);
	QObject::connect(&audioPlayer, &LFD::filtermusic::AudioPlayer::paused,
			&server, &LFD::filtermusic::Server::stopped);
//	QObject::connect(audioPlayer, &LFD::filtermusic::AudioPlayer::volume,
//			server, &Server::setVolume);

	return a.exec();
}

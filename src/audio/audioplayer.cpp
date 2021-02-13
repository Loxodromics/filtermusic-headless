/**
 *  audioplayer.cpp
 *  filtermusic-headless
 *
 *  Created by philipp on 10.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
*/
#include "audioplayer.h"

#include <QDebug>
#include <QMediaMetaData>

namespace LFD {

AudioPlayer::AudioPlayer(QObject* parent)
	: QObject(parent),
	  m_player(this),
	  m_playingState(PlayingState::NotConnected),
	  m_title(""),
	  m_stationUrl("")
{
	connect( &this->m_player, SIGNAL(metaDataChanged()),
			 this, SLOT(metaDataChanged()));

	connect( &this->m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
			 this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));

	connect( &this->m_player, SIGNAL(bufferStatusChanged(int)),
			 this, SLOT(bufferingProgress(int)));

	connect( &this->m_player, SIGNAL(error(QMediaPlayer::Error)),
			 this, SLOT(reportErrorMessage()));

	m_player.setVolume(100);
}

bool AudioPlayer::playing() const
{
	qDebug() << "this->m_playingState" << (int)this->m_playingState;
	return (this->m_playingState == PlayingState::Playing);
}

void AudioPlayer::setPlayingState(const PlayingState& playingState)
{
	if ( this->m_playingState != playingState )
	{
		this->m_playingState = playingState;
//		emit playingStateChanged( this->m_playingState );
//		emit playingChanged( this->playing() );
	}
}

QString AudioPlayer::title() const
{
	return this->m_title;
}

void AudioPlayer::setTitle(QString title)
{
	if (m_title == title)
		return;

	m_title = title;
	emit newTitle(m_title);
}

void AudioPlayer::setStationUrl(const QString stationUrl)
{
	this->m_stationUrl = stationUrl;
	this->play();
}

void AudioPlayer::setVolume(int volume)
{
	qDebug() << "volume:" << volume;
	this->m_player.setVolume(volume);
}

AudioPlayer::PlayingState AudioPlayer::playingState() const
{
	return this->m_playingState;
}

void AudioPlayer::play()
{
	qDebug() << "AudioPlayer::play()" << this->m_stationUrl;
	if ( !this->m_stationUrl.isEmpty() )
	{
		qDebug() << "AudioPlayer::play...";
		this->m_player.setMedia(QUrl(this->m_stationUrl));
		this->setPlayingState(PlayingState::Connecting);
		this->m_player.play();
	}
}

void AudioPlayer::pause()
{
	this->setPlayingState(PlayingState::NotConnected);
	this->m_player.pause();
}

void AudioPlayer::metaDataChanged()
{
	if (this->m_player.isMetaDataAvailable()) {
		qDebug() << this->m_player.metaData(QMediaMetaData::Title);
//        setTrackInfo(QString("%1 - %2")
//                .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
//                .arg(player->metaData(QMediaMetaData::Title).toString()));

//        if (coverLabel) {
//            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

//            coverLabel->setPixmap(!url.isEmpty()
//                    ? QPixmap(url.toString())
//                    : QPixmap());
//        }
	}
}

void AudioPlayer::statusChanged(QMediaPlayer::MediaStatus status)
{
	qDebug() << "AudioPlayer::statusChanged" << status;
	// handle status message
	switch (status) {
	case QMediaPlayer::UnknownMediaStatus:
	case QMediaPlayer::NoMedia:
	case QMediaPlayer::LoadedMedia:
	case QMediaPlayer::BufferingMedia:
	case QMediaPlayer::BufferedMedia:
		this->setPlayingState(PlayingState::Playing);
		break;
	case QMediaPlayer::LoadingMedia:
		this->setPlayingState(PlayingState::Connecting);
		break;
	case QMediaPlayer::StalledMedia:
		this->setPlayingState(PlayingState::Connecting);
		break;
	case QMediaPlayer::EndOfMedia:
		this->setPlayingState( PlayingState::NotConnected );
		break;
	case QMediaPlayer::InvalidMedia:
		reportErrorMessage();
		break;
	}
}

void AudioPlayer::stateChanged(QMediaPlayer::State state)
{
	if (state == QMediaPlayer::StoppedState) {
		this->setPlayingState( PlayingState::NotConnected );
	}
}

void AudioPlayer::bufferingProgress(int progress)
{
	qDebug() << tr("Buffering %4%").arg(progress);
}

void AudioPlayer::reportErrorMessage()
{
	qDebug() << "error:" << this->m_player.errorString();
	this->setPlayingState( PlayingState::NotConnected );
}

}  /// namespace LFD
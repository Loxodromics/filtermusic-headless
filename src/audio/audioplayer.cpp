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

namespace filtermusic {

AudioPlayer::AudioPlayer(QObject* parent)
	: QObject(parent),
	  m_player(this),
	  m_playingState(PlayingState::NotConnected),
	  m_trackinfo(""),
	  m_stationUrl(""),
	  m_media()
{
	connect( &this->m_player, SIGNAL(metaDataChanged()),
			 this, SLOT(metaDataChanged()));

	connect( &this->m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
			 this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));

	connect( &this->m_player, SIGNAL(bufferStatusChanged(int)),
			 this, SLOT(bufferingProgress(int)));

	connect( &this->m_player, SIGNAL(error(QMediaPlayer::Error)),
			 this, SLOT(reportErrorMessage()));

//	this->m_player.setVolume(oldVolume);
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

void AudioPlayer::setTrackInfo(const QString title)
{
	if (this->m_trackinfo == title)
		return;

	this->m_trackinfo = title;
	emit newTitle(QStringLiteral("trackinfo:") + m_trackinfo);
}

void AudioPlayer::setStationUrl(const QString stationUrl)
{
	this->m_stationUrl = stationUrl;
	this->play();
}

void AudioPlayer::setMedia(const QString jsonBase64)
{
	QByteArray byteArray = QByteArray::fromBase64(jsonBase64.toUtf8());
	this->m_media.fromJsonString(byteArray);
	this->setStationUrl(this->m_media.url());
}

void AudioPlayer::setVolume(int volume)
{
	qDebug() << "volume:" << volume;
	this->m_player.setVolume(volume);
	emit this->status(QStringLiteral("volume:") + QString::number(volume));
}

void AudioPlayer::updateState()
{
	this->statusChanged(this->m_player.mediaStatus());
	emit this->status(QStringLiteral("volume:") + QString::number(this->m_player.volume()));
	emit this->newTitle(m_trackinfo);
	QByteArray jsonBase64 = this->m_media.toJsonString().toUtf8().toBase64();
	emit this->media(jsonBase64);
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
	emit this->status(QStringLiteral("paused"));
}

void AudioPlayer::metaDataChanged()
{
	if (this->m_player.isMetaDataAvailable()) {
		qDebug() << this->m_player.metaData(QMediaMetaData::Title);
		this->setTrackInfo(QStringLiteral("trackinfo:%1 - %2")
							 .arg(this->m_player.metaData(QMediaMetaData::AlbumArtist).toString())
							 .arg(this->m_player.metaData(QMediaMetaData::Title).toString()));


		QUrl url = this->m_player.metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();
		qDebug() << "cover url: " << url;
		emit this->status(QStringLiteral("coverurl:") + url.toString());
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
		emit this->status(QStringLiteral("stopped"));
		break;
	case QMediaPlayer::BufferedMedia:
		this->setPlayingState(PlayingState::Playing);
		emit this->status(QStringLiteral("playing"));
		break;
	case QMediaPlayer::LoadingMedia:
		this->setPlayingState(PlayingState::Connecting);
		emit this->status(QStringLiteral("connecting"));
		break;
	case QMediaPlayer::StalledMedia:
		this->setPlayingState(PlayingState::Connecting);
		emit this->status(QStringLiteral("connecting"));
		break;
	case QMediaPlayer::EndOfMedia:
		this->setPlayingState( PlayingState::NotConnected );
		emit this->status(QStringLiteral("stopped"));
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
		emit this->status(QStringLiteral("stopped"));
	}
}

void AudioPlayer::bufferingProgress(int progress)
{
	qDebug() << tr("Buffering %4%").arg(progress);
	emit this->status(tr("buffering %4%").arg(progress));
}

void AudioPlayer::reportErrorMessage()
{
	qDebug() << "error:" << this->m_player.errorString();
	this->setPlayingState( PlayingState::NotConnected );
	emit this->status(QStringLiteral("error:") + this->m_player.errorString());
}

}	/// namespace filtermusic

}  /// namespace LFD

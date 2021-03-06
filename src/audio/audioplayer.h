/**
 *  audioplayer.h
 *  filtermusic-headless
 *
 *  Created by philipp on 10.02.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
*/
#pragma once

#include <QObject>
#include <QMediaPlayer>

#include "src/audio/lfdaudiomedia.h"

namespace LFD {

namespace filtermusic {

class AudioPlayer : public QObject
{
	Q_OBJECT

public:

	enum class PlayingState : int
	{
		NotConnected,
		FailedPlaying,
		Connecting,
		Reconnecting,
		Paused,
		Playing
	};

	explicit AudioPlayer(QObject* parent = nullptr);

	void play();
	void pause();
	bool playing() const;

	AudioPlayer::PlayingState playingState() const;

public slots:
	void setStationUrl(const QString stationUrl);
	void setMedia(const QString jsonBase64);
	void setTrackInfo(const QString trackinfo);
	void setVolume(int volume);
	void updateState();

protected slots:
	void metaDataChanged();
	void statusChanged(QMediaPlayer::MediaStatus status);
	void stateChanged(QMediaPlayer::State state);
	void bufferingProgress(int progress);
	void reportErrorMessage();

signals:
	void newTitle(QString title);
	void startedPlaying();
	void paused();
	void status(const QString& status);
	void media(const QString jsonBase64);

protected:
	void setPlayingState(const PlayingState& playingState);

	QMediaPlayer m_player;
	PlayingState m_playingState;
	QString m_trackinfo;
	QString m_stationUrl;
	LFD::AudioMedia m_media;
};

}	/// namespace filtermusic

}	/// namespace LFD


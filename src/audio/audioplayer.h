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

namespace LFD {

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

	QString title() const;

public slots:
	void setStationUrl(const QString stationUrl);
	void setTitle(QString title);
	void setVolume(int volume);

protected slots:
	void metaDataChanged();
	void statusChanged(QMediaPlayer::MediaStatus status);
	void stateChanged(QMediaPlayer::State state);
	void bufferingProgress(int progress);
	void reportErrorMessage();

signals:
	void newTitle( QString title );
	void playPressed();
	void pausePressed();

protected:
	void setPlayingState( const PlayingState& playingState );

	QMediaPlayer m_player;
	PlayingState m_playingState;
	QString m_title;
	QString m_stationUrl;
};

}	/// namespace LFD


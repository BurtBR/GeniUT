#ifndef WORKERSOUNDPLAYER_H
#define WORKERSOUNDPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>

#include "sounds.h"

class WorkerSoundPlayer : public QObject{
    Q_OBJECT
private:
    QMediaPlayer *_mediaPlayerContinuous = nullptr;
    QVector<Sounds::Sound> _soundQueue, _currentMusic;
    QTimer *_timer= nullptr;

public:
    WorkerSoundPlayer(QObject *parent = nullptr);
    ~WorkerSoundPlayer();

private slots:
    void PlayerMediaStatusChanged(QMediaPlayer::MediaStatus progress);
    void MediaStatusChanged(QMediaPlayer::MediaStatus progress);
    void TimerTimeout();

public slots:
    void PlayNow(Sounds::Sound s);
    void PlayNext(Sounds::Sound s);
    void StopPlaying();
    void PlayTonesFromString(QString str, int clock);

signals:
    void PlayerPlay();
    void PlayerStop();
    void PlayerSetSource(QUrl);
    void TimerStop();
    void InvalidMusicStr();
    void MusicFinished();
    void PressButton(uint8_t octave, uint8_t pos);
};

#endif // WORKERSOUNDPLAYER_H

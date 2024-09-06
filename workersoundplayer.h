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
    QTimer *_timer= nullptr, *_delay = nullptr;
    uint32_t _currentlimit = 0;
    int _currentclock = 1000;

public:
    WorkerSoundPlayer(QObject *parent = nullptr);
    ~WorkerSoundPlayer();

private slots:
    void PlayerMediaStatusChanged(QMediaPlayer::MediaStatus progress);
    void MediaStatusChanged(QMediaPlayer::MediaStatus progress);
    void TimerTimeout();
    void DelayEnd();

public slots:
    void PlayNow(Sounds::Sound s);
    void PlayNext(Sounds::Sound s);
    void StopPlaying();
    void PlayTonesFromString(QString str, int clock, uint32_t limit = 0xFFFFFFFF);
    void PlayTones(QVector<Sounds::Sound> music, int clock, uint32_t limit = 0xFFFFFFFF, int delay = 0);

signals:
    void PlayerPlay();
    void PlayerStop();
    void PlayerSetSource(QUrl);
    void DelayStart(int msec);
    void TimerStart(int msec);
    void TimerStop();
    void InvalidMusicStr();
    void MusicFinished();
    void PressButton(uint8_t octave, uint8_t pos);
};

#endif // WORKERSOUNDPLAYER_H

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
    QVector<Sounds::Sound> _soundQueue;

public:
    WorkerSoundPlayer(QObject *parent = nullptr);
    ~WorkerSoundPlayer();

private slots:
    void PlayerMediaStatusChanged(QMediaPlayer::MediaStatus progress);
    void MediaStatusChanged(QMediaPlayer::MediaStatus progress);

public slots:
    void PlayNext(Sounds::Sound s);

signals:
};

#endif // WORKERSOUNDPLAYER_H

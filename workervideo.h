#ifndef WORKERVIDEO_H
#define WORKERVIDEO_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QAudioOutput>

class WorkerVideo : public QObject{
    Q_OBJECT
private:
    static const QVector<QUrl> _videoSources;
    uint32_t _videocounter = 0;
    QMediaPlayer *_player = nullptr;
    QVideoSink *_videosink = nullptr;
    QAudioOutput *_audioout = nullptr;

    void DeletePointers();
    bool AllocateResources();

public:
    WorkerVideo(QObject *parent = nullptr);
    ~WorkerVideo();

private slots:
    void FrameChanged(const QVideoFrame &frame);
    void MediaStatusChanged(QMediaPlayer::MediaStatus status);

public slots:
    void Play();

signals:
    void FrameReady(QPixmap);
    void EndOfVideo();
};

#endif // WORKERVIDEO_H

#include "workervideo.h"

#include <QFileInfo>
#include <QPixmap>

const QVector<QUrl> WorkerVideo::_videoSources{
    QUrl("./Videos/SabiaGeradoSeno.mp4"),
    QUrl("./Videos/HardClipping.mp4")
};

WorkerVideo::WorkerVideo(QObject *parent) : QObject{parent}{

}

WorkerVideo::~WorkerVideo(){
    DeletePointers();
}

void WorkerVideo::DeletePointers(){
    if(_player){
        delete _player;
        _player = nullptr;
    }

    if(_videosink){
        delete _videosink;
        _videosink = nullptr;
    }

    if(_audioout){
        delete _audioout;
        _audioout = nullptr;
    }
}

bool WorkerVideo::AllocateResources(){
    if(_player || _videosink || _audioout)
        return false;

    try{
        _player = new QMediaPlayer;
    }catch(...){
        _player = nullptr;
        return false;
    }

    try{
        _videosink = new QVideoSink;
    }catch(...){
        DeletePointers();
        return false;
    }

    try{
        _audioout = new QAudioOutput;
    }catch(...){
        DeletePointers();
        return false;
    }

    _player->setVideoSink(_videosink);
    _player->setAudioOutput(_audioout);

    connect(_videosink, &QVideoSink::videoFrameChanged, this, &WorkerVideo::FrameChanged);
    connect(_player, &QMediaPlayer::mediaStatusChanged, this, &WorkerVideo::MediaStatusChanged);

    return true;
}

void WorkerVideo::FrameChanged(const QVideoFrame &frame){
    if(!frame.isValid())
        return;

    emit FrameReady(QPixmap::fromImage(frame.toImage()));
}

void WorkerVideo::MediaStatusChanged(QMediaPlayer::MediaStatus status){
    if(status == QMediaPlayer::EndOfMedia){
        emit EndOfVideo();
        DeletePointers();
    }
}

void WorkerVideo::Play(){
    if(!_videoSources.size()){
        emit EndOfVideo();
        return;
    }

    if(_videocounter >= _videoSources.size())
        _videocounter =0;

    if(!QFileInfo::exists(_videoSources[_videocounter].toString())){
        _videocounter++;
        emit EndOfVideo();
        return;
    }

    if(!AllocateResources()){
        emit EndOfVideo();
        return;
    }

    _player->setSource(_videoSources[_videocounter]);
    _player->play();

    _videocounter++;
}

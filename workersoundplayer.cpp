#include "workersoundplayer.h"

WorkerSoundPlayer::WorkerSoundPlayer(QObject *parent) : QObject{parent}{

    QAudioOutput *audioOutput = nullptr;

    try{
        _mediaPlayerContinuous = new QMediaPlayer();
    }catch(...){
        throw "Unable to allocate memory for Media Player";
    }

    try{
        audioOutput = new QAudioOutput(_mediaPlayerContinuous);
    }catch(...){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
        throw "Unable to allocate memory for Audio Output";
    }

    _mediaPlayerContinuous->setAudioOutput(audioOutput);

    connect(_mediaPlayerContinuous, &QMediaPlayer::mediaStatusChanged, this, &WorkerSoundPlayer::PlayerMediaStatusChanged);
}

WorkerSoundPlayer::~WorkerSoundPlayer(){
    if(_mediaPlayerContinuous){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
    }
}

void WorkerSoundPlayer::PlayNext(Sounds::Sound s){
    _soundQueue.append(s);
    if(_soundQueue.size() == 1){
        _mediaPlayerContinuous->setSource(Sounds::GetSoundPath(s));
        _mediaPlayerContinuous->play();
    }
}

void WorkerSoundPlayer::PlayerMediaStatusChanged(QMediaPlayer::MediaStatus progress){
    if(progress == QMediaPlayer::EndOfMedia){
        _soundQueue.removeFirst();
        if(!_soundQueue.isEmpty()){
            _mediaPlayerContinuous->setSource(Sounds::GetSoundPath(_soundQueue[0]));
            _mediaPlayerContinuous->play();
        }
    }
}

void WorkerSoundPlayer::MediaStatusChanged(QMediaPlayer::MediaStatus progress){
    if(progress == QMediaPlayer::EndOfMedia){
        sender()->deleteLater();
    }
}

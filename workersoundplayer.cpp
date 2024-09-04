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
    connect(this, &WorkerSoundPlayer::PlayerPlay, _mediaPlayerContinuous, &QMediaPlayer::play);
    connect(this, &WorkerSoundPlayer::PlayerStop, _mediaPlayerContinuous, &QMediaPlayer::stop);
    connect(this, &WorkerSoundPlayer::PlayerSetSource, _mediaPlayerContinuous, &QMediaPlayer::setSource);
}

WorkerSoundPlayer::~WorkerSoundPlayer(){
    if(_mediaPlayerContinuous){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
    }
}

void WorkerSoundPlayer::PlayNow(Sounds::Sound s){
    if(s == Sounds::Sound::silence)
        return;

    QAudioOutput *output;
    QMediaPlayer *player;

    try{
        player = new QMediaPlayer();
    }catch(...){
        return;
    }

    try{
        output = new QAudioOutput(player);
    }catch(...){
        delete player;
        return;
    }

    player->setAudioOutput(output);
    player->setSource(Sounds::GetSoundPath(s));
    output->setVolume(0.3);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &WorkerSoundPlayer::MediaStatusChanged);

    player->play();
}

void WorkerSoundPlayer::PlayNext(Sounds::Sound s){
    _soundQueue.append(s);
    if(_soundQueue.size() == 1){
        emit PlayerSetSource(Sounds::GetSoundPath(s));
        emit PlayerPlay();
    }
}

void WorkerSoundPlayer::StopPlaying(){
    emit PlayerStop();
    _soundQueue.clear();
}

void WorkerSoundPlayer::PlayerMediaStatusChanged(QMediaPlayer::MediaStatus progress){
    if(progress == QMediaPlayer::EndOfMedia){
        _soundQueue.removeFirst();
        if(!_soundQueue.isEmpty()){
            emit PlayerSetSource(Sounds::GetSoundPath(_soundQueue[0]));
            emit PlayerPlay();
        }
    }
}

void WorkerSoundPlayer::MediaStatusChanged(QMediaPlayer::MediaStatus progress){
    if(progress == QMediaPlayer::EndOfMedia){
        sender()->deleteLater();
    }
}

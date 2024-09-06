#include "workersoundplayer.h"

WorkerSoundPlayer::WorkerSoundPlayer(QObject *parent) : QObject{parent}{

    QAudioOutput *audioOutput = nullptr;

    try{
        _mediaPlayerContinuous = new QMediaPlayer();
    }catch(...){
        throw "Unable to allocate memory for Media Player";
        return;
    }

    try{
        audioOutput = new QAudioOutput(_mediaPlayerContinuous);
    }catch(...){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
        throw "Unable to allocate memory for Audio Output";
        return;
    }

    try{
        _timer = new QTimer(this);
    }catch(...){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
        delete audioOutput;
        throw "Unable to allocate memory for Timer";
        return;
    }

    try{
        _delay = new QTimer(this);
    }catch(...){
        delete _mediaPlayerContinuous;
        _mediaPlayerContinuous = nullptr;
        delete audioOutput;
        delete _timer;
        throw "Unable to allocate memory for Timer";
        return;
    }

    _timer->setTimerType(Qt::PreciseTimer);
    connect(this, &WorkerSoundPlayer::TimerStop, _timer, &QTimer::stop);
    connect(this, &WorkerSoundPlayer::TimerStart, _timer, qOverload<int>(&QTimer::start));
    connect(_timer, &QTimer::timeout, this, &WorkerSoundPlayer::TimerTimeout);

    connect(this, &WorkerSoundPlayer::TimerStop, _delay, &QTimer::stop);
    connect(this, &WorkerSoundPlayer::DelayStart, _delay, qOverload<int>(&QTimer::start));
    connect(_delay, &QTimer::timeout, this, &WorkerSoundPlayer::DelayEnd);
    _delay->setSingleShot(true);

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
    if(_timer){
        _timer->stop();
        delete _timer;
        _timer = nullptr;
    }
    if(_delay){
        _delay->stop();
        delete _delay;
        _delay = nullptr;
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
    emit TimerStop();
    emit PlayerStop();
    _soundQueue.clear();
}

void WorkerSoundPlayer::PlayTonesFromString(QString str, int clock, uint32_t limit){
    bool ok;

    _currentMusic = Sounds::GetMusicFromString(str, ok);

    if(!ok){
        emit InvalidMusicStr();
        _currentMusic.clear();
        emit MusicFinished();
        return;
    }

    _currentlimit = limit;
    emit TimerStart(clock);
}

void WorkerSoundPlayer::PlayTones(QVector<Sounds::Sound> music, int clock, uint32_t limit, int delay){
    _currentMusic = music;
    _currentlimit = limit;
    _currentclock = clock;
    if(delay > 0)
        emit DelayStart(delay);
    else
        emit TimerStart(clock);
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

void WorkerSoundPlayer::TimerTimeout(){

    uint8_t octave=0, pos=0;

    if(_currentlimit == 0){ // uint doesn't has negative values
        emit TimerStop();
        emit MusicFinished();
        return;
    }

    if(_currentMusic[0] != Sounds::Sound::silence){
        _currentlimit--;
        Sounds::GetOctavePosFromTone(_currentMusic[0], octave, pos);
        emit PressButton(octave, pos);
        PlayNow(_currentMusic[0]);
    }else{
        emit PressButton(0, 12);
    }

    _currentMusic.removeFirst();

    if(!_currentMusic.size() || _currentlimit == 0){
        emit TimerStop();
        emit MusicFinished();
    }
}

void WorkerSoundPlayer::DelayEnd(){
    emit TimerStart(_currentclock);
}

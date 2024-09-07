#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    connect(_ui->button1, &QPushButton::clicked, this, &MainWindow::On_button1_Clicked);
    connect(_ui->button2, &QPushButton::clicked, this, &MainWindow::On_button2_Clicked);
    connect(_ui->button3, &QPushButton::clicked, this, &MainWindow::On_button3_Clicked);
    connect(_ui->button4, &QPushButton::clicked, this, &MainWindow::On_button4_Clicked);
    connect(_ui->button5, &QPushButton::clicked, this, &MainWindow::On_button5_Clicked);
    connect(_ui->button6, &QPushButton::clicked, this, &MainWindow::On_button6_Clicked);
    connect(_ui->button7, &QPushButton::clicked, this, &MainWindow::On_button7_Clicked);
    connect(_ui->button8, &QPushButton::clicked, this, &MainWindow::On_button8_Clicked);
    connect(_ui->button9, &QPushButton::clicked, this, &MainWindow::On_button9_Clicked);
    connect(_ui->button10, &QPushButton::clicked, this, &MainWindow::On_button10_Clicked);
    connect(_ui->button11, &QPushButton::clicked, this, &MainWindow::On_button11_Clicked);
    connect(_ui->button12, &QPushButton::clicked, this, &MainWindow::On_button12_Clicked);
    connect(_ui->buttonSilence, &QPushButton::clicked, this, &MainWindow::On_buttonSilence_Clicked);
    connect(_ui->buttonRecord, &QPushButton::clicked, this, &MainWindow::On_buttonRecord_Clicked);
    connect(_ui->buttonPlay, &QPushButton::clicked, this, &MainWindow::On_buttonPlay_Clicked);
    connect(_ui->buttonSave, &QPushButton::clicked, this, &MainWindow::On_buttonSave_Clicked);
    connect(_ui->buttonOpen, &QPushButton::clicked, this, &MainWindow::On_buttonOpen_Clicked);
    connect(_ui->buttonBack, &QPushButton::clicked, this, &MainWindow::On_buttonBack_Clicked);
    connect(_ui->comboOctave, &QComboBox::currentIndexChanged, this, &MainWindow::OctaveChanged);

    _ui->comboOctave->addItem("2");
    _ui->comboOctave->addItem("3");
    _ui->comboOctave->addItem("4");
    _ui->comboOctave->addItem("5");
    _ui->comboOctave->addItem("6");

    _ui->comboOctave->setCurrentText("4");

    SetUIMode(UIMode::Initial);

    installEventFilter(this);
}

MainWindow::~MainWindow(){
    DeleteThread(&_threadSoundPlayer);
    DeleteThread(&_threadFileHandler);
    delete _ui;
}

bool MainWindow::Init(){

    if(!QDir("Musicas").exists()){
        QDir().mkdir("Musicas");
        if(!QDir("Musicas").exists())
            return false;
    }

    if(!StartThreadSoundPlayer())
        return false;

    if(!StartThreadFileHandler())
        return false;

    SetGamemode(Gamemode::Welcome);

    _scoreToday = {0,0,0,0,0};
    emit GetScoreFile();

    return true;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event){

    if(event->type() == QEvent::KeyPress){
        switch(((QKeyEvent*)event)->key()){
        case Qt::Key_2:
            _ui->comboOctave->setCurrentText("2");
            break;
        case Qt::Key_3:
            _ui->comboOctave->setCurrentText("3");
            break;
        case Qt::Key_4:
            _ui->comboOctave->setCurrentText("4");
            break;
        case Qt::Key_5:
            _ui->comboOctave->setCurrentText("5");
            break;
        case Qt::Key_6:
            _ui->comboOctave->setCurrentText("6");
            break;
        case Qt::Key_Equal:
            ButtonClicked(ButtonType::BtnUp);
            break;
        case Qt::Key_Plus:
            ButtonClicked(ButtonType::BtnUp);
            break;
        case Qt::Key_Minus:
            ButtonClicked(ButtonType::BtnDown);
            break;
        case Qt::Key_Backspace:
            ButtonClicked(ButtonType::BtnDeleteLast);
            break;
        default:
            break;
        }
    }

    return QMainWindow::eventFilter(target, event);
}

bool MainWindow::StartThreadSoundPlayer(){

    if(_threadSoundPlayer)
        return true;

    WorkerSoundPlayer *worker;

    try{
        _threadSoundPlayer = new QThread();
    }catch(...){
        return false;
    }

    try{
        worker = new WorkerSoundPlayer();
    }catch(...){
        delete _threadSoundPlayer;
        _threadSoundPlayer = nullptr;
        return false;
    }

    connect(_threadSoundPlayer, &QThread::finished, worker, &WorkerSoundPlayer::deleteLater);
    connect(this, &MainWindow::PlaySoundNext, worker, &WorkerSoundPlayer::PlayNext);
    connect(this, &MainWindow::PlayTone, worker, &WorkerSoundPlayer::PlayNow);
    connect(this, &MainWindow::StopPlaying, worker, &WorkerSoundPlayer::StopPlaying);
    connect(this, &MainWindow::PlayTonesFromString, worker, &WorkerSoundPlayer::PlayTonesFromString);
    connect(this, &MainWindow::PlayTones, worker, &WorkerSoundPlayer::PlayTones);
    connect(worker, &WorkerSoundPlayer::InvalidMusicStr, this, &MainWindow::InvalidMusicStr);
    connect(worker, &WorkerSoundPlayer::MusicFinished, this, &MainWindow::MusicFinished);
    connect(worker, &WorkerSoundPlayer::PressButton, this, &MainWindow::MusicPressButton);

    worker->moveToThread(_threadSoundPlayer);
    _threadSoundPlayer->start(QThread::HighestPriority);

    return true;
}

bool MainWindow::StartThreadFileHandler(){

    if(_threadFileHandler)
        return false;

    WorkerFileHandler *worker;

    try{
        _threadFileHandler = new QThread();
    }catch(...){
        return false;
    }

    try{
        worker = new WorkerFileHandler();
    }catch(...){
        delete _threadFileHandler;
        _threadFileHandler = nullptr;
        return false;
    }

    connect(_threadFileHandler, &QThread::finished, worker, &WorkerFileHandler::deleteLater);
    connect(this, &MainWindow::GetScoreFile, worker, &WorkerFileHandler::GetScoreFile);
    connect(this, &MainWindow::SaveMusicFile, worker, &WorkerFileHandler::SaveMusicFile);
    connect(this, &MainWindow::OpenMusicFile, worker, &WorkerFileHandler::OpenMusicFile);
    connect(this, &MainWindow::SetScoreFile, worker, &WorkerFileHandler::SetScoreFile);
    connect(worker, &WorkerFileHandler::FileHandlingFinished, this, &MainWindow::FileHandlingFinished);
    connect(worker, &WorkerFileHandler::FileHandlingError, this, &MainWindow::FileError);
    connect(worker, &WorkerFileHandler::ScoreFile, this, &MainWindow::ScoreFile);
    connect(worker, &WorkerFileHandler::InvalidMusicStr, this, &MainWindow::InvalidMusicStr);
    connect(worker, &WorkerFileHandler::FileMusic, this, &MainWindow::ReceivedFileMusic);

    worker->moveToThread(_threadFileHandler);
    _threadFileHandler->start();

    return true;
}

void MainWindow::SetGamemode(Gamemode mode){

    StopMusic();
    _isRecording = false;

    switch(mode){

    case Gamemode::Welcome:
        SetUIMode(UIMode::Initial);
        _currentgamemode = Gamemode::Initial;
        emit PlaySoundNext(Sounds::Sound::welcome);
        emit PlaySoundNext(Sounds::Sound::choosegamemode);
        break;

    case Gamemode::Initial:
        _currentgamemode = Gamemode::Initial;
        SetUIMode(UIMode::Initial);
        emit PlaySoundNext(Sounds::Sound::choosegamemode);
        break;

    case Gamemode::Practice:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicpractice);
            return;
        }
        SetCurrentRound(1);
        _currentMusic.clear();
        _currentgamemode = Gamemode::Practice;
        SetUIMode(UIMode::Practice);
        _musicsInFolder = QDir("Musicas").entryInfoList(QDir::Files);
        if(!StartThreadFileHandler()){
            emit PlaySoundNext(Sounds::Sound::filehandlingfail);
            SetGamemode(Gamemode::Initial);
            return;
        }
        this->setEnabled(false);
        _currentFileIndex = 0;
        emit OpenMusicFile(_musicsInFolder[_currentFileIndex].absoluteFilePath());
        break;

    case Gamemode::OneMusic:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicplay);
            return;
        }
        if(!StartThreadFileHandler()){
            emit PlaySoundNext(Sounds::Sound::filehandlingfail);
            SetGamemode(Gamemode::Initial);
            return;
        }
        SetCurrentRound(1);
        _currentMusic.clear();
        _currentgamemode = Gamemode::OneMusic;
        SetUIMode(UIMode::Playing);
        _musicsInFolder = QDir("Musicas").entryInfoList(QDir::Files);
        _currentFileIndex = QRandomGenerator::global()->bounded(_musicsInFolder.size());
        this->setEnabled(false);
        emit OpenMusicFile(_musicsInFolder[_currentFileIndex].absoluteFilePath());
        break;

    case Gamemode::OneRandom:
        SetCurrentRound(1);
        _currentgamemode = Gamemode::OneRandom;
        SetUIMode(UIMode::Playing);
        _currentMusic.clear();
        _currentMusic.append(Sounds::GetRandomTone());
        SetTonesWhite();
        _ui->spinClock->setValue(500);
        _ui->labelInfo->setText("Tocando...");
        emit PlayTones(_currentMusic, 500, _currentRound, 1000);
        break;

    case Gamemode::TwoMusic:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicplay);
            return;
        }
        SetCurrentRound(1);
        _currentgamemode = Gamemode::TwoMusic;
        SetUIMode(UIMode::Playing);
        break;

    case Gamemode::TwoRandom:
        SetCurrentRound(1);
        _currentgamemode = Gamemode::TwoRandom;
        SetUIMode(UIMode::Playing);
        break;

    case Gamemode::TwoMakeSong:
        SetCurrentRound(1);
        _currentgamemode = Gamemode::TwoMakeSong;
        SetUIMode(UIMode::PlayingCreate);
        break;

    case Gamemode::Creation:
        _currentgamemode = Gamemode::Creation;
        SetUIMode(UIMode::Creation);
        break;

    default:
        break;
    }
}

void MainWindow::SetUIMode(UIMode mode){

    _ui->button1->setText("C");
    _ui->button2->setText("C#");
    _ui->button3->setText("D");
    _ui->button4->setText("D#");
    _ui->button5->setText("E");
    _ui->button6->setText("F");
    _ui->button7->setText("F#");
    _ui->button8->setText("G");
    _ui->button9->setText("G#");
    _ui->button10->setText("A");
    _ui->button11->setText("A#");
    _ui->button12->setText("B");

    switch(mode){

    case UIMode::Initial:
        _ui->labelInfo->setText("Aguardando...");
        _ui->button1->setText("Praticar");
        _ui->button2->setText("1 Jogador\nMúsica");
        _ui->button3->setText("1 Jogador\nAleatório");
        _ui->button4->setText("");
        _ui->button5->setText("2 Jogadores\nMúsica");
        _ui->button6->setText("2 Jogadores\nAleatório");
        _ui->button7->setText("2 Jogadores\nCriar");
        _ui->button8->setText("");
        _ui->button9->setText("");
        _ui->button10->setText("Compor Música");
        _ui->button11->setText("");
        _ui->button12->setText("");
        _ui->textConsole->hide();
        _ui->textScores->hide();
        _ui->buttonSilence->hide();
        _ui->buttonBack->hide();
        _ui->buttonOpen->hide();
        _ui->buttonSave->hide();
        _ui->buttonRecord->hide();
        _ui->spinClock->hide();
        _ui->labelClock->hide();
        _ui->labelOctave->hide();
        _ui->comboOctave->hide();
        _ui->buttonPlay->hide();
        _ui->labelRoundLabel->hide();
        _ui->labelRound->hide();
        SetTonesGreen();
        break;

    case UIMode::Practice:
        _ui->textConsole->setProperty("readOnly", true);
        _ui->textConsole->show();
        _ui->textConsole->clear();
        _ui->textScores->hide();
        _ui->buttonSilence->hide();
        _ui->buttonBack->show();
        _ui->buttonOpen->show();
        _ui->buttonSave->hide();
        _ui->buttonRecord->hide();
        _ui->spinClock->show();
        _ui->labelClock->show();
        _ui->labelOctave->hide();
        _ui->comboOctave->hide();
        _ui->buttonPlay->hide();
        _ui->labelRoundLabel->show();
        _ui->labelRound->show();
        SetTonesGreen();
        break;

    case UIMode::Playing:
        _ui->textConsole->hide();
        _ui->textScores->show();
        _ui->buttonSilence->hide();
        _ui->buttonBack->show();
        _ui->buttonOpen->hide();
        _ui->buttonSave->hide();
        _ui->buttonRecord->hide();
        _ui->spinClock->hide();
        _ui->labelClock->hide();
        _ui->labelOctave->hide();
        _ui->comboOctave->hide();
        _ui->buttonPlay->hide();
        _ui->labelRoundLabel->show();
        _ui->labelRound->show();
        SetTonesGreen();
        break;

    case UIMode::PlayingCreate:
        _ui->textConsole->hide();
        _ui->textScores->show();
        _ui->buttonSilence->hide();
        _ui->buttonBack->show();
        _ui->buttonOpen->hide();
        _ui->buttonSave->hide();
        _ui->buttonRecord->hide();
        _ui->spinClock->show();
        _ui->labelClock->show();
        _ui->labelOctave->show();
        _ui->comboOctave->show();
        _ui->buttonPlay->hide();
        _ui->labelRoundLabel->show();
        _ui->labelRound->show();
        SetTonesGreen();
        break;

    case UIMode::Creation:
        _ui->buttonRecord->setEnabled(true);
        _ui->textConsole->setProperty("readOnly", false);
        _ui->textConsole->show();
        _ui->textScores->hide();
        _ui->buttonSilence->show();
        _ui->buttonBack->show();
        _ui->buttonOpen->show();
        _ui->buttonSave->show();
        _ui->buttonRecord->show();
        _ui->spinClock->show();
        _ui->labelClock->show();
        _ui->labelOctave->show();
        _ui->comboOctave->show();
        _ui->buttonPlay->show();
        _ui->labelRoundLabel->hide();
        _ui->labelRound->hide();
        _ui->buttonRecord->setText("Gravar");
        _ui->buttonPlay->setText("Tocar");
        _ui->textConsole->clear();
        SetTonesWhite();
        break;

    default:
        break;
    }
    ResetShortcuts();
}

void MainWindow::ButtonClicked(ButtonType btn){

    if(btn == ButtonType::BtnBack){
        SetGamemode(Gamemode::Initial);
        return;
    }

    Sounds::Sound toneaux;
    QString straux, filename;
    int intaux;

    switch(_currentgamemode){

    case Gamemode::Initial:
        switch(btn){
        case ButtonType::Btn1:
            SetGamemode(Gamemode::Practice);
            break;
        case ButtonType::Btn2:
            SetGamemode(Gamemode::OneMusic);
            break;
        case ButtonType::Btn3:
            SetGamemode(Gamemode::OneRandom);
            break;
        case ButtonType::Btn5:
            SetGamemode(Gamemode::TwoMusic);
            break;
        case ButtonType::Btn6:
            SetGamemode(Gamemode::TwoRandom);
            break;
        case ButtonType::Btn7:
            SetGamemode(Gamemode::TwoMakeSong);
            break;
        case ButtonType::Btn10:
            SetGamemode(Gamemode::Creation);
            break;
        default:
            break;
        }
        break;

    case Gamemode::Practice:
        switch(btn){
        case ButtonType::Btn1:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 0);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn2:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 1);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn3:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 2);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn4:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 3);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn5:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 4);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn6:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 5);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn7:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 6);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn8:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 7);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn9:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 8);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn10:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 9);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn11:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 10);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn12:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 11);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::BtnSilence:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            break;
        case ButtonType::BtnUp:
            if(_musicsInFolder.size() == 1)
                return;
            if(!StartThreadFileHandler()){
                emit PlaySoundNext(Sounds::Sound::filehandlingfail);
                return;
            }
            this->setEnabled(false);

            if(_currentFileIndex == 0)
                _currentFileIndex = _musicsInFolder.size()-1;
            else
                _currentFileIndex--;

            SetCurrentRound(1);
            _currentMusic.clear();
            emit OpenMusicFile(_musicsInFolder[_currentFileIndex].absoluteFilePath());
            break;
        case ButtonType::BtnDown:
            if(_musicsInFolder.size() == 1)
                return;

            if(!StartThreadFileHandler()){
                emit PlaySoundNext(Sounds::Sound::filehandlingfail);
                return;
            }
            this->setEnabled(false);

            if(_currentFileIndex == (_musicsInFolder.size()-1))
                _currentFileIndex = 0;
            else
                _currentFileIndex++;

            SetCurrentRound(1);
            _currentMusic.clear();
            emit OpenMusicFile(_musicsInFolder[_currentFileIndex].absoluteFilePath());
            break;
        case ButtonType::BtnOpen:
            filename = QFileDialog::getOpenFileName(this, "Abrir Musica", "Musicas", "(*.camusi)");
            if(filename.size()){
                if(!StartThreadFileHandler()){
                    emit PlaySoundNext(Sounds::Sound::filehandlingfail);
                    return;
                }
                this->setEnabled(false);
                emit OpenMusicFile(filename);
            }
            break;
        default:
            break;
        }
        break;

    case Gamemode::OneMusic:
    case Gamemode::OneRandom:
        switch(btn){
        case ButtonType::Btn1:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 0);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn2:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 1);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn3:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 2);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn4:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 3);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn5:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 4);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn6:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 5);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn7:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 6);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn8:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 7);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn9:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 8);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn10:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 9);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn11:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 10);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        case ButtonType::Btn12:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 11);
            emit PlayTone(toneaux);
            CheckGameState(toneaux);
            break;
        default:
            break;
        }
        break;

    case Gamemode::TwoMusic:
        break;

    case Gamemode::TwoRandom:
        break;

    case Gamemode::TwoMakeSong:
        break;

    case Gamemode::Creation:
        switch(btn){
        case ButtonType::Btn1:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 0);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn2:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 1);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn3:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 2);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn4:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 3);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn5:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 4);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn6:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 5);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn7:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 6);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn8:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 7);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn9:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 8);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn10:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 9);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn11:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 10);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::Btn12:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            toneaux = Sounds::GetTone(_currentoctave, 11);
            emit PlayTone(toneaux);
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(toneaux) + ",");
            break;
        case ButtonType::BtnSilence:
            if(_buttonFromPlaying){
                _buttonFromPlaying = false;
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
                return;
            }
            if(_isRecording)
                _ui->textConsole->insertPlainText(Sounds::GetToneString(Sounds::Sound::silence) + ",");
            break;
        case ButtonType::BtnUp:
            if(_ui->comboOctave->currentIndex() == (_ui->comboOctave->count()-1))
                _ui->comboOctave->setCurrentIndex(0);
            else
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->currentIndex()+1);
            break;
        case ButtonType::BtnDown:
            if(_ui->comboOctave->currentIndex() == 0)
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->count()-1);
            else
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->currentIndex()-1);
            break;

        case ButtonType::BtnRecord:
            _isRecording ^= 1;
            if(_isRecording){
                _ui->labelInfo->setText("Gravando");
                _ui->buttonRecord->setText("Parar Gravação");
            }else{
                _ui->labelInfo->setText("Aguardando...");
                _ui->buttonRecord->setText("Gravar");
            }
            break;

        case ButtonType::BtnPlay:
            straux = _ui->textConsole->toPlainText();
            intaux = _ui->textConsole->textCursor().position();
            if(intaux!=0){
                while(straux[intaux-1]!=',' && intaux < straux.size()){
                    intaux++;
                    _ui->textConsole->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
                }
                if(intaux == straux.size())
                    _ui->textConsole->moveCursor(QTextCursor::Start);
                else
                    straux.remove(0,intaux);
            }
            if(straux.size() < 2)
                return;
            _isPlaying ^= 1;
            if(_isPlaying){
                _isRecording = false;
                _ui->buttonRecord->setText("Gravar");
                _ui->buttonRecord->setEnabled(false);
                _ui->labelInfo->setText("Tocando");
                _ui->buttonPlay->setText("Parar Música");
                emit PlayTonesFromString(straux, _ui->spinClock->value());
            }else{
                _ui->buttonRecord->setEnabled(true);
                emit StopPlaying();
                if(_isRecording)
                    _ui->labelInfo->setText("Gravando");
                else
                    _ui->labelInfo->setText("Aguardando...");
                _ui->buttonPlay->setText("Tocar");
            }
            break;

        case ButtonType::BtnSave:
            straux = _ui->textConsole->toPlainText();
            if(straux.size() < 2)
                return;
            filename = QFileDialog::getSaveFileName(this, "Salvar Musica", "Musicas","(*.camusi)");
            if(filename.size()){
                if(!StartThreadFileHandler()){
                    emit PlaySoundNext(Sounds::Sound::unabletosave);
                    return;
                }
                this->setEnabled(false);
                emit SaveMusicFile(filename, straux, _ui->spinClock->value());
            }
            break;

        case ButtonType::BtnOpen:
            filename = QFileDialog::getOpenFileName(this, "Abrir Musica", "Musicas", "(*.camusi)");
            if(filename.size()){
                if(!StartThreadFileHandler()){
                    emit PlaySoundNext(Sounds::Sound::filehandlingfail);
                    return;
                }
                this->setEnabled(false);
                emit OpenMusicFile(filename);
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void MainWindow::ResetShortcuts(){
    _ui->button1->setShortcut(QKeySequence(Qt::Key_Q));
    _ui->button2->setShortcut(QKeySequence(Qt::Key_W));
    _ui->button3->setShortcut(QKeySequence(Qt::Key_E));
    _ui->button4->setShortcut(QKeySequence(Qt::Key_R));
    _ui->button5->setShortcut(QKeySequence(Qt::Key_A));
    _ui->button6->setShortcut(QKeySequence(Qt::Key_S));
    _ui->button7->setShortcut(QKeySequence(Qt::Key_D));
    _ui->button8->setShortcut(QKeySequence(Qt::Key_F));
    _ui->button9->setShortcut(QKeySequence(Qt::Key_Z));
    _ui->button10->setShortcut(QKeySequence(Qt::Key_X));
    _ui->button11->setShortcut(QKeySequence(Qt::Key_C));
    _ui->button12->setShortcut(QKeySequence(Qt::Key_V));
    _ui->buttonSilence->setShortcut(QKeySequence(Qt::Key_Space));
    _ui->buttonBack->setShortcut(QKeySequence(Qt::Key_M));
}

void MainWindow::DeleteThread(QThread **threadptr){
    QThread *ptr = *threadptr;

    if(ptr){
        ptr->quit();
        if(!ptr->wait(5000)){
            ptr->terminate();
            ptr->wait();
        }
        delete *threadptr;
        *threadptr = nullptr;
    }
}

void MainWindow::SetScoreboard(){
    QString scoretext = "<p style=\"text-align: center;\"><b>Placar de Hoje:</b><br>";

    for(int i=0; i<_scoreToday.size() ;i++){
        scoretext.append(QString::number(_scoreToday[i]) + "<br>");
    }

    scoretext.append("<br><b>Placar do Dispositivo:</b><br>");

    for(int i=0; i<_scoreFile.size() ;i++){
        scoretext.append(QString::number(_scoreFile[i]) + "<br>");
    }

    scoretext.append("</p>");

    _ui->textScores->setText(scoretext);
}

void MainWindow::SetCurrentRound(uint32_t value){
    _currentRound = value;
    _ui->labelRound->setText(QString::number(value));
    _currentPressedTones = 0;
    _currentToneIndex = 0;
    if(_currentMusic.size()){
        while(_currentMusic[_currentToneIndex]==Sounds::Sound::silence) // File handling checks for music without tones
            _currentToneIndex++;
    }
}

void MainWindow::StopMusic(){
    emit StopPlaying();
    _isPlaying = false;
}

void MainWindow::SetTonesRed(){
    _ui->button1->setStyleSheet("");
    _ui->button2->setStyleSheet("");
    _ui->button3->setStyleSheet("");
    _ui->button5->setStyleSheet("");
    _ui->button6->setStyleSheet("");
    _ui->button7->setStyleSheet("");
    _ui->button10->setStyleSheet("");
    _ui->widgetTones->setStyleSheet(""
    "QPushButton{"
        "font: 20pt \"Source Code Pro\";"
        "color: rgb(0, 0, 0);"
        "background-color: rgb(240, 150, 150);"
    "}"
    "QPushButton:pressed { background-color: red; }"
    "QPushButton::disabled{"
        "font: 20pt \"Source Code Pro\";"
        "color: rgb(0, 0, 0);"
        "background-color: rgb(50, 50, 50);"
    "}");

}

void MainWindow::SetTonesGreen(){

    QString style = "QPushButton{"
                        "font: 20pt \"Source Code Pro\";"
                        "color: rgb(0, 0, 0);"
                        "background-color: rgb(150, 240, 150);"
                    "}"
                    "QPushButton:pressed { background-color: red; }"
                    "QPushButton::disabled{"
                        "font: 20pt \"Source Code Pro\";"
                        "color: rgb(0, 0, 0);"
                        "background-color: rgb(50, 50, 50);"
                    "}";

    if(_currentgamemode == Gamemode::Initial || _currentgamemode == Gamemode::Welcome){
        _ui->button1->setStyleSheet(style);
        _ui->button2->setStyleSheet(style);
        _ui->button3->setStyleSheet(style);
        _ui->button5->setStyleSheet(style);
        _ui->button6->setStyleSheet(style);
        _ui->button7->setStyleSheet(style);
        _ui->button10->setStyleSheet(style);
        style = "QPushButton{"
                    "font: 20pt \"Source Code Pro\";"
                    "color: rgb(0, 0, 0);"
                    "background-color: rgb(240, 240, 240);"
                "}"
                "QPushButton:pressed { background-color: red; }"
                "QPushButton::disabled{"
                    "font: 20pt \"Source Code Pro\";"
                    "color: rgb(0, 0, 0);"
                    "background-color: rgb(50, 50, 50);"
                "}";
        _ui->widgetTones->setStyleSheet(style);
    }else{
        _ui->widgetTones->setStyleSheet(style);
        style = "";
        _ui->button1->setStyleSheet(style);
        _ui->button2->setStyleSheet(style);
        _ui->button3->setStyleSheet(style);
        _ui->button5->setStyleSheet(style);
        _ui->button6->setStyleSheet(style);
        _ui->button7->setStyleSheet(style);
        _ui->button10->setStyleSheet(style);
    }
}

void MainWindow::SetTonesWhite(){
    QString style = "QPushButton{"
                        "font: 20pt \"Source Code Pro\";"
                        "color: rgb(0, 0, 0);"
                        "background-color: rgb(240, 240, 240);"
                    "}"
                    "QPushButton:pressed { background-color: red; }"
                    "QPushButton::disabled{"
                        "font: 20pt \"Source Code Pro\";"
                        "color: rgb(0, 0, 0);"
                        "background-color: rgb(50, 50, 50);"
                    "}";
    _ui->widgetTones->setStyleSheet(style);
    style = "";
    _ui->button1->setStyleSheet(style);
    _ui->button2->setStyleSheet(style);
    _ui->button3->setStyleSheet(style);
    _ui->button5->setStyleSheet(style);
    _ui->button6->setStyleSheet(style);
    _ui->button7->setStyleSheet(style);
    _ui->button10->setStyleSheet(style);
}

void MainWindow::CheckGameState(Sounds::Sound tone){
    switch(_currentgamemode){
    case Gamemode::Practice:
        if(_currentToneIndex >= _currentMusic.size()){
            SetCurrentRound(1);
            SetTonesWhite();
            _ui->textConsole->moveCursor(QTextCursor::Start);
            emit PlayTones(_currentMusic, _ui->spinClock->value(), _currentRound,1000);
            return;
        }
        if(tone == _currentMusic[_currentToneIndex]){
            SetTonesGreen();
            _currentPressedTones++;
            _currentToneIndex++;

            if(_currentPressedTones == _currentRound){

                // Check if this is the last tone
                while(_currentToneIndex < _currentMusic.size()){
                    if(_currentMusic[_currentToneIndex] != Sounds::Sound::silence)
                        break;
                    _currentToneIndex++;
                }
                if(_currentToneIndex == _currentMusic.size()){
                    emit PlaySoundNext(Sounds::Sound::musiccomplete);
                    return;
                }

                SetCurrentRound(_currentRound+1);

                SetTonesWhite();
                _ui->textConsole->moveCursor(QTextCursor::Start);
                emit PlayTones(_currentMusic, _ui->spinClock->value(), _currentRound, 1000);
            }else{
                while(_currentMusic[_currentToneIndex] == Sounds::Sound::silence)
                    _currentToneIndex++;
                _ui->comboOctave->setCurrentText(QString::number(Sounds::GetOctave(_currentMusic[_currentToneIndex])));
            }
        }else{
            SetTonesRed();
            emit PlaySoundNext(Sounds::Sound::youmissed);
        }
        break;

    case Gamemode::OneMusic:
        if(_currentToneIndex >= _currentMusic.size()){
            SetGamemode(Gamemode::Initial);
            return;
        }
        if(tone == _currentMusic[_currentToneIndex]){
            SetTonesGreen();
            _currentPressedTones++;
            _currentToneIndex++;

            if(_currentPressedTones == _currentRound){

                // Check if this is the last tone
                while(_currentToneIndex < _currentMusic.size()){
                    if(_currentMusic[_currentToneIndex] != Sounds::Sound::silence)
                        break;
                    _currentToneIndex++;
                }
                if(_currentToneIndex == _currentMusic.size()){
                    emit PlaySoundNext(Sounds::Sound::musiccomplete);
                    _currentgamemode = Gamemode::OneRandom;
                    SetTonesWhite();
                    _currentMusic.append(Sounds::GetRandomTone());
                    SetCurrentRound(_currentRound+1);
                    _ui->labelInfo->setText("Tocando...");
                    emit PlayTones(_currentMusic, _ui->spinClock->value(), _currentRound, 2000);
                    return;
                }

                SetCurrentRound(_currentRound+1);

                SetTonesWhite();
                _ui->labelInfo->setText("Tocando...");
                emit PlayTones(_currentMusic, _ui->spinClock->value(), _currentRound, 1000);
            }else{
                while(_currentMusic[_currentToneIndex] == Sounds::Sound::silence)
                    _currentToneIndex++;
                _ui->comboOctave->setCurrentText(QString::number(Sounds::GetOctave(_currentMusic[_currentToneIndex])));
            }
        }else{
            SetTonesRed();
            _ui->labelInfo->setText("Você Errou");
            emit PlaySoundNext(Sounds::Sound::youmissed);
            _currentToneIndex = _currentMusic.size();
            CheckScore();
        }
        break;

    case Gamemode::OneRandom:
        if(_currentToneIndex >= _currentMusic.size()){
            SetGamemode(Gamemode::Initial);
            return;
        }
        if(tone == _currentMusic[_currentToneIndex]){
            SetTonesGreen();
            _currentPressedTones++;
            _currentToneIndex++;

            if(_currentPressedTones == _currentRound){

                SetCurrentRound(_currentRound+1);

                SetTonesWhite();
                _ui->labelInfo->setText("Tocando...");
                _currentMusic.append(Sounds::GetRandomTone());
                emit PlayTones(_currentMusic, _ui->spinClock->value(), _currentRound, 1000);
            }else{
                //This check is needed in case this mode comes after music mode
                while(_currentMusic[_currentToneIndex] == Sounds::Sound::silence)
                    _currentToneIndex++;
                _ui->comboOctave->setCurrentText(QString::number(Sounds::GetOctave(_currentMusic[_currentToneIndex])));
            }
        }else{
            SetTonesRed();
            _ui->labelInfo->setText("Você Errou");
            emit PlaySoundNext(Sounds::Sound::youmissed);
            _currentToneIndex = _currentMusic.size();
            CheckScore();
        }
        break;

    default:
        break;
    }
}

void MainWindow::CheckScore(){
    for(int i=0; i<_scoreToday.size() ;i++){
        if(_currentRound > _scoreToday[i]){
            _scoreToday.insert(i, _currentRound);
            _scoreToday.removeLast();
            emit PlaySoundNext(Sounds::Sound::newdailyrecord);
            break;
        }
    }

    for(int i=0; i<_scoreFile.size() ;i++){
        if(_currentRound > _scoreFile[i]){
            _scoreFile.insert(i, _currentRound);
            _scoreFile.removeLast();
            emit PlaySoundNext(Sounds::Sound::newdevicerecord);
            if(!StartThreadFileHandler()){
                emit PlaySoundNext(Sounds::Sound::unabletosave);
            }else{
                emit SetScoreFile(_scoreFile);
            }
            break;
        }
    }

    SetScoreboard();
}

void MainWindow::OctaveChanged(){
    _currentoctave = _ui->comboOctave->currentText().toUInt();
}

void MainWindow::ScoreFile(QVector<uint> score){
    _scoreFile = score;
    SetScoreboard();
}

void MainWindow::FileError(QString text){
    emit StopPlaying();
    emit PlaySoundNext(Sounds::Sound::filehandlingfail);
    _ui->labelInfo->setText(text);
}

void MainWindow::FileHandlingFinished(){
    this->setEnabled(true);
    DeleteThread(&_threadFileHandler);
}

void MainWindow::InvalidMusicStr(){
    emit PlaySoundNext(Sounds::Sound::invalidmusictext);
}

void MainWindow::MusicFinished(){
    _isPlaying = false;

    switch(_currentgamemode){
    case Gamemode::Practice:
        _ui->comboOctave->setCurrentText(QString::number(Sounds::GetOctave(_currentMusic[_currentToneIndex])));
        SetTonesGreen();
        break;

    case Gamemode::OneMusic:
    case Gamemode::OneRandom:
        _ui->comboOctave->setCurrentText(QString::number(Sounds::GetOctave(_currentMusic[_currentToneIndex])));
        _ui->labelInfo->setText("Sua Vez!");
        SetTonesGreen();
        break;

    case Gamemode::Creation:
        _ui->buttonRecord->setEnabled(true);
        if(_isRecording)
            _ui->labelInfo->setText("Gravando");
        else
            _ui->labelInfo->setText("Aguardando...");
        _ui->buttonPlay->setText("Tocar");
        break;

    default:
        break;
    }
}

void MainWindow::MusicPressButton(uint8_t octave, uint8_t pos){

    _buttonFromPlaying = true;

    if(pos == 12){
        ButtonClicked(ButtonType::BtnSilence);
        return;
    }

    _ui->comboOctave->setCurrentText(QString::number(octave));

    switch(pos){
    case 0:
        _ui->button1->animateClick();
        break;
    case 1:
        _ui->button2->animateClick();
        break;
    case 2:
        _ui->button3->animateClick();
        break;
    case 3:
        _ui->button4->animateClick();
        break;
    case 4:
        _ui->button5->animateClick();
        break;
    case 5:
        _ui->button6->animateClick();
        break;
    case 6:
        _ui->button7->animateClick();
        break;
    case 7:
        _ui->button8->animateClick();
        break;
    case 8:
        _ui->button9->animateClick();
        break;
    case 9:
        _ui->button10->animateClick();
        break;
    case 10:
        _ui->button11->animateClick();
        break;
    case 11:
        _ui->button12->animateClick();
        break;
    default:
        break;
    }
}

void MainWindow::ReceivedFileMusic(QString filename, QString music, int clock, QVector<Sounds::Sound> musicvector){

    _ui->spinClock->setValue(clock);
    _currentMusic = musicvector;

    switch(_currentgamemode){
    case Gamemode::Practice:
        _ui->labelInfo->setText(filename);
        _ui->textConsole->clear();
        _ui->textConsole->insertPlainText(music);
        SetTonesWhite();
        _ui->textConsole->moveCursor(QTextCursor::Start);
        SetCurrentRound(1);
        emit PlayTones(_currentMusic, clock, _currentRound, 1000);
        break;

    case Gamemode::OneMusic:
        SetTonesWhite();
        SetCurrentRound(1);
        _ui->labelInfo->setText("Tocando...");
        emit PlayTones(_currentMusic, clock, _currentRound, 1000);
        break;

    case Gamemode::Creation:
        _ui->labelInfo->setText(filename);
        _ui->textConsole->clear();
        _ui->textConsole->insertPlainText(music);
        _ui->textConsole->moveCursor(QTextCursor::Start);
        break;

    default:
        break;
    }
}

void MainWindow::On_button1_Clicked(){
    ButtonClicked(ButtonType::Btn1);
}
void MainWindow::On_button2_Clicked(){
    ButtonClicked(ButtonType::Btn2);
}
void MainWindow::On_button3_Clicked(){
    ButtonClicked(ButtonType::Btn3);
}
void MainWindow::On_button4_Clicked(){
    ButtonClicked(ButtonType::Btn4);
}
void MainWindow::On_button5_Clicked(){
    ButtonClicked(ButtonType::Btn5);
}
void MainWindow::On_button6_Clicked(){
    ButtonClicked(ButtonType::Btn6);
}
void MainWindow::On_button7_Clicked(){
    ButtonClicked(ButtonType::Btn7);
}
void MainWindow::On_button8_Clicked(){
    ButtonClicked(ButtonType::Btn8);
}
void MainWindow::On_button9_Clicked(){
    ButtonClicked(ButtonType::Btn9);
}
void MainWindow::On_button10_Clicked(){
    ButtonClicked(ButtonType::Btn10);
}
void MainWindow::On_button11_Clicked(){
    ButtonClicked(ButtonType::Btn11);
}
void MainWindow::On_button12_Clicked(){
    ButtonClicked(ButtonType::Btn12);
}
void MainWindow::On_buttonSilence_Clicked(){
    ButtonClicked(ButtonType::BtnSilence);
}
void MainWindow::On_buttonRecord_Clicked(){
    ButtonClicked(ButtonType::BtnRecord);
}
void MainWindow::On_buttonPlay_Clicked(){
    ButtonClicked(ButtonType::BtnPlay);
}
void MainWindow::On_buttonSave_Clicked(){
    ButtonClicked(ButtonType::BtnSave);
}
void MainWindow::On_buttonOpen_Clicked(){
    ButtonClicked(ButtonType::BtnOpen);
}
void MainWindow::On_buttonBack_Clicked(){
    ButtonClicked(ButtonType::BtnBack);
}

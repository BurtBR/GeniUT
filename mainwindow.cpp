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
    connect(worker, &WorkerFileHandler::FileHandlingFinished, this, &MainWindow::FileHandlingFinished);
    connect(worker, &WorkerFileHandler::FileHandlingError, this, &MainWindow::FileError);
    connect(worker, &WorkerFileHandler::ScoreFile, this, &MainWindow::ScoreFile);

    worker->moveToThread(_threadFileHandler);
    _threadFileHandler->start();

    return true;
}

void MainWindow::SetGamemode(Gamemode mode){

    emit StopPlaying();

    switch(mode){

    case Gamemode::Welcome:
        SetUIMode(UIMode::Initial);
        _currentgamemode = Gamemode::Initial;
        emit PlaySoundNext(Sounds::Sound::welcome);
        emit PlaySoundNext(Sounds::Sound::choosegamemode);
        break;

    case Gamemode::Initial:
        SetUIMode(UIMode::Initial);
        _currentgamemode = Gamemode::Initial;
        emit PlaySoundNext(Sounds::Sound::choosegamemode);
        break;

    case Gamemode::Practice:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicpractice);
            return;
        }
        SetUIMode(UIMode::Practice);
        _currentgamemode = Gamemode::Practice;
        break;

    case Gamemode::OneMusic:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicplay);
            return;
        }
        SetUIMode(UIMode::Playing);
        _currentgamemode = Gamemode::OneMusic;
        break;

    case Gamemode::OneRandom:
        SetUIMode(UIMode::Playing);
        _currentgamemode = Gamemode::OneRandom;
        break;

    case Gamemode::TwoMusic:
        if(QDir("Musicas").isEmpty()){
            emit PlaySoundNext(Sounds::Sound::nomusicplay);
            return;
        }
        SetUIMode(UIMode::Playing);
        _currentgamemode = Gamemode::TwoMusic;
        break;

    case Gamemode::TwoRandom:
        SetUIMode(UIMode::Playing);
        _currentgamemode = Gamemode::TwoRandom;
        break;

    case Gamemode::TwoMakeSong:
        SetUIMode(UIMode::PlayingCreate);
        _currentgamemode = Gamemode::TwoMakeSong;
        break;

    case Gamemode::Creation:
        SetUIMode(UIMode::Creation);
        _currentgamemode = Gamemode::Creation;
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
        break;

    case UIMode::Practice:
        _ui->textConsole->setProperty("readOnly", true);
        _ui->textConsole->show();
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
        _ui->buttonPlay->show();
        _ui->labelRoundLabel->show();
        _ui->labelRound->show();
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
        break;

    case UIMode::Creation:
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
        break;

    default:
        break;
    }
    ResetShortcuts();
}

void MainWindow::ButtonClicked(ButtonType btn){

    if(_currentgamemode == Gamemode::Initial){
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
    }else{
        switch(btn){
        case ButtonType::Btn1:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 0));
            break;
        case ButtonType::Btn2:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 1));
            break;
        case ButtonType::Btn3:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 2));
            break;
        case ButtonType::Btn4:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 3));
            break;
        case ButtonType::Btn5:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 4));
            break;
        case ButtonType::Btn6:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 5));
            break;
        case ButtonType::Btn7:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 6));
            break;
        case ButtonType::Btn8:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 7));
            break;
        case ButtonType::Btn9:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 8));
            break;
        case ButtonType::Btn10:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 9));
            break;
        case ButtonType::Btn11:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 10));
            break;
        case ButtonType::Btn12:
            if(!_isPlaying)
                emit PlayTone(Sounds::GetTone(_currentoctave, 11));
            break;
        case ButtonType::BtnSilence:
            break;
        case ButtonType::BtnBack:
            SetGamemode(Gamemode::Initial);
            break;
        case ButtonType::BtnUp:
            if(_ui->comboOctave->currentIndex() == (_ui->comboOctave->count()-1)){
                _ui->comboOctave->setCurrentIndex(0);
            }else{
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->currentIndex()+1);
            }
            break;
        case ButtonType::BtnDown:
            if(_ui->comboOctave->currentIndex() == 0){
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->count()-1);
            }else{
                _ui->comboOctave->setCurrentIndex(_ui->comboOctave->currentIndex()-1);
            }
            break;
        case ButtonType::BtnDeleteLast:
            break;
        default:
            break;
        }
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

void MainWindow::OctaveChanged(){
    _currentoctave = _ui->comboOctave->currentText().toUInt();
}

void MainWindow::ScoreFile(QVector<uint> score){
    _scoreFile = score;
    SetScoreboard();
}

void MainWindow::FileError(QString text){
    emit StopPlaying();
    emit PlaySoundNext(Sounds::Sound::unabletosave);
    _ui->labelInfo->setText(text);
}

void MainWindow::FileHandlingFinished(){
    this->setEnabled(true);
    DeleteThread(&_threadFileHandler);
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

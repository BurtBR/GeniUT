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

    _ui->comboOctave->addItem("2");
    _ui->comboOctave->addItem("3");
    _ui->comboOctave->addItem("4");
    _ui->comboOctave->addItem("5");
    _ui->comboOctave->addItem("6");

    SetGamemode(Gamemode::Initial);
}

MainWindow::~MainWindow(){
    delete _ui;
}

bool MainWindow::Init(){

    return true;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event){

    if(event->type() == QEvent::KeyPress){
        if(((QKeyEvent*)event)->modifiers().testFlag(Qt::KeypadModifier)){
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
            default:
                break;
            }
        }
    }

    return QMainWindow::eventFilter(target, event);
}

void MainWindow::SetGamemode(Gamemode mode){
    switch(mode){

    case Gamemode::Initial:
        SetUIMode(UIMode::Initial);
        _currentgamemode = Gamemode::Initial;
        break;

    default:
        break;
    }
}

void MainWindow::SetUIMode(UIMode mode){
    switch(mode){

    case UIMode::Initial:
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
        break;

    case UIMode::Practice:
        break;

    case UIMode::Creation:
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

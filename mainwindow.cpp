#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    connect(_ui->button1, &QPushButton::clicked, this, &MainWindow::On_button1_Clicked);

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
        ResetShortcuts();
        break;

    case UIMode::Practice:
        break;

    case UIMode::Creation:
        break;

    default:
        break;
    }
}

void MainWindow::On_button1_Clicked(){
    qDebug() << "Butao 1";
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

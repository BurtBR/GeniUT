#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QKeyEvent>
#include <QDir>

#include "workersoundplayer.h"
#include "workerfilehandler.h"

// DEBUG
#include <QDebug>
// DEBUG

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

    enum class UIMode{
        Initial,
        Practice,
        Playing,
        Creation
    };

    enum class Gamemode{
        Welcome,
        Initial,
        Practice,
        OneMusic,
        OneRandom,
        TwoMusic,
        TwoRandom,
        TwoMakeSong,
        Creation
    };

    enum class ButtonType{
        Btn1,
        Btn2,
        Btn3,
        Btn4,
        Btn5,
        Btn6,
        Btn7,
        Btn8,
        Btn9,
        Btn10,
        Btn11,
        Btn12,
        BtnSilence,
        BtnRecord,
        BtnPlay,
        BtnSave,
        BtnOpen,
        BtnBack,
        BtnUp,
        BtnDown,
        BtnDeleteLast
    };

private:
    Ui::MainWindow *_ui;
    QThread *_threadSoundPlayer = nullptr;
    QThread *_threadFileHandler = nullptr;
    Gamemode _currentgamemode = Gamemode::Welcome;
    QVector<uint> _scoreToday, _scoreFile;

    bool eventFilter(QObject *target, QEvent *event);
    bool StartThreadSoundPlayer();
    bool StartThreadFileHandler();
    void SetGamemode(Gamemode mode);
    void SetUIMode(UIMode mode);
    void ButtonClicked(ButtonType btn);
    void ResetShortcuts();
    void DeleteThread(QThread **threadptr);
    void SetScoreboard();

    void On_button1_Clicked();
    void On_button2_Clicked();
    void On_button3_Clicked();
    void On_button4_Clicked();
    void On_button5_Clicked();
    void On_button6_Clicked();
    void On_button7_Clicked();
    void On_button8_Clicked();
    void On_button9_Clicked();
    void On_button10_Clicked();
    void On_button11_Clicked();
    void On_button12_Clicked();
    void On_buttonSilence_Clicked();
    void On_buttonRecord_Clicked();
    void On_buttonPlay_Clicked();
    void On_buttonSave_Clicked();
    void On_buttonOpen_Clicked();
    void On_buttonBack_Clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool Init();

private slots:
    void ScoreFile(QVector<uint> score);
    void FileError(QString text);
    void FileHandlingFinished();

signals:
    void PlaySoundNext(Sounds::Sound);
    void StopPlaying();
    void GetScoreFile();
};
#endif // MAINWINDOW_H

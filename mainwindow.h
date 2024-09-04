#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QKeyEvent>

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
        Creation
    };

    enum class Gamemode{
        Initial,
        Practice,
        OneMusic,
        OneRandom,
        TwoMusic,
        TwoRandom,
        TwoMakeSong,
        Creation
    };

private:
    Ui::MainWindow *_ui;
    Gamemode _currentgamemode = Gamemode::Initial;

    bool eventFilter(QObject *target, QEvent *event);
    void SetGamemode(Gamemode mode);
    void SetUIMode(UIMode mode);
    void ResetShortcuts();

    void On_button1_Clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool Init();
};
#endif // MAINWINDOW_H

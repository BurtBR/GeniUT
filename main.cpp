#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]){

#ifdef Q_OS_LINUX
    qputenv("QT_MEDIA_BACKEND", "gstreamer");
#endif

    QApplication a(argc, argv);
    MainWindow w;

#ifdef _IS_PIODEVICE
    a.setOverrideCursor(QCursor(Qt::BlankCursor));
#endif

    w.showMaximized();

    if(!w.Init()){
        QMessageBox::warning(nullptr, "Erro", "Falha ao inicializar! Reinicie o programa.");
        return 0;
    }

    return a.exec();
}

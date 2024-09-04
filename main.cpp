#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    if(!w.Init()){
        QMessageBox::warning(nullptr, "Erro", "Falha ao inicializar! Reinicie o programa.");
        return 0;
    }

    return a.exec();
}

#ifndef WORKERGPIO_H
#define WORKERGPIO_H

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

class WorkerGPIO : public QObject{
    Q_OBJECT

public:
    WorkerGPIO(QObject *parent = nullptr);
    ~WorkerGPIO();
    void GPIO_Init();

signals:
};

#endif // WORKERGPIO_H

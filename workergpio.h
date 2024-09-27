#ifndef WORKERGPIO_H
#define WORKERGPIO_H

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

// TEMP
#include <QThread>

class WorkerGPIO : public QObject{
    Q_OBJECT

private:
    static const QVector<const char*> _pins;
    static void UnexportPins();

public:
    WorkerGPIO(QObject *parent = nullptr);
    ~WorkerGPIO();
    static bool GPIO_Init();

signals:
};

#endif // WORKERGPIO_H

#ifndef WORKERGPIO_H
#define WORKERGPIO_H

#include <QObject>
#include <QFile>

// TEMP
#include <QDebug>

class WorkerGPIO : public QObject{
    Q_OBJECT

private:
    static uint32_t* _gpio_base;

public:
    WorkerGPIO(QObject *parent = nullptr);
    ~WorkerGPIO();
    static bool GPIO_Init();

signals:
};

#endif // WORKERGPIO_H

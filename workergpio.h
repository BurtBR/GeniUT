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
    bool GPIO_Init();

public slots:
    void Init();

signals:
    void GPIOError(QString);
};

#endif // WORKERGPIO_H

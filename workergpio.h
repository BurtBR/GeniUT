#ifndef WORKERGPIO_H
#define WORKERGPIO_H

#include <QObject>
#include <QFile>

// TEMP
#include <QDebug>
#include <QThread>

class WorkerGPIO : public QObject{
    Q_OBJECT

public:
    enum class LED{
        LED1,LED2 ,LED3 ,LED4,
        LED5,LED6 ,LED7 ,LED8,
        LED9,LED10,LED11,LED12
    };

private:
    static volatile uint32_t* _gpio_base;
    static QFile *_memfile;

public:
    WorkerGPIO(QObject *parent = nullptr);
    ~WorkerGPIO();
    bool GPIO_Init();

public slots:
    void Init();
    void AllOn();
    void AllOff();
    void TurnOn(LED btn);
    void TurnOff(LED btn);

signals:
    void GPIOError(QString);
};

#endif // WORKERGPIO_H

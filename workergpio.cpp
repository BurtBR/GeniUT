#include "workergpio.h"

const QVector<const char*> WorkerGPIO::_pins = {
    "514"
};

WorkerGPIO::WorkerGPIO(QObject *parent) : QObject{parent}{

}

WorkerGPIO::~WorkerGPIO(){

    UnexportPins();

    QObject::~QObject();
}

bool WorkerGPIO::GPIO_Init(){
    if(!QFileInfo::exists("/sys/class/gpio/export"))
        return false;

    QFile fp("/sys/class/gpio/export");

    if(!fp.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    for(int i=0; i<_pins.size() ;i++){
        fp.write(_pins[i]);
    }

    fp.close();
    return true;
}

void WorkerGPIO::UnexportPins(){
    if(!QFileInfo::exists("/sys/class/gpio/unexport"))
        return;

    QFile fp("/sys/class/gpio/unexport");

    if(!fp.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    for(int i=0; i<_pins.size() ;i++){
        fp.write(_pins[i]);
    }

    fp.close();
}

#include "workergpio.h"

const QVector<const char*> WorkerGPIO::_pins = {
    "514","515","516","517","518","519","520","521","522","523","524","525"
};

WorkerGPIO::WorkerGPIO(QObject *parent) : QObject{parent}{

}

WorkerGPIO::~WorkerGPIO(){

    UnexportPins();

    //QObject::~QObject();
}

bool WorkerGPIO::GPIO_Init(){
    if(!QFileInfo::exists("/sys/class/gpio/export"))
        return false;

    QFile *fp;

    try{
        fp = new QFile("/sys/class/gpio/export");
    }catch(...){
        return false;
    }

    for(int i=0; i<_pins.size() ;i++){
        if(!fp->open(QIODevice::WriteOnly | QIODevice::Text)){
            delete fp;
            UnexportPins();
            return false;
        }
        fp->write(_pins[i]);
        fp->close();
    }
    delete fp;

    for(int i=0; i<_pins.size() ;i++){

        QThread::msleep(100);
        try{
            fp = new QFile(QString("/sys/class/gpio/gpio" + QString(_pins[i]) + "/direction"));
        }catch(...){
            return false;
        }

        if(!fp->open(QIODevice::WriteOnly | QIODevice::Text)){
            delete fp;
            UnexportPins();
            return false;
        }
        fp->write("out");
        fp->close();
        delete fp;
    }

    return true;
}

void WorkerGPIO::UnexportPins(){
    if(!QFileInfo::exists("/sys/class/gpio/unexport"))
        return;

    QFile fp("/sys/class/gpio/unexport");

    for(int i=0; i<_pins.size() ;i++){
        if(!fp.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        fp.write(_pins[i]);
        fp.close();
    }

    fp.close();
}

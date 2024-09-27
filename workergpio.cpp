#include "workergpio.h"

WorkerGPIO::WorkerGPIO(QObject *parent) : QObject{parent}{

}

WorkerGPIO::~WorkerGPIO(){
    QObject::~QObject();
}

#include "workergpio.h"

#define _GPIO_MEM_FILE "/dev/gpiomem"
#define _GPIO_FSEL_INPUT 0b000U
#define _GPIO_FSEL_OUTPUT 0b001U

#define _GPIO_FSEL0 _gpio_base[0]
#define _GPIO_FSEL1 _gpio_base[1]
#define _GPIO_FSEL2 _gpio_base[2]
#define _GPIO_FSEL3 _gpio_base[3]
#define _GPIO_FSEL4 _gpio_base[4]
#define _GPIO_FSEL5 _gpio_base[5]

#define _GPIO_SET0 _gpio_base[7]
#define _GPIO_SET1 _gpio_base[8]

#define _GPIO_CLR0 _gpio_base[10]
#define _GPIO_CLR1 _gpio_base[11]

#define _GPIO_LEV0 _gpio_base[13]
#define _GPIO_LEV1 _gpio_base[14]

#define _GPIO4_FSEL_Shift (12U)
#define _GPIO4_FSEL_Msk (0b111U << _GPIO4_FSEL_Shift)
#define _GPIO4_OUTPUT (0x1U << 4)

#define _GPIO5_FSEL_Shift (15U)
#define _GPIO5_FSEL_Msk (0b111U << _GPIO5_FSEL_Shift)
#define _GPIO5_OUTPUT (0x1U << 5)

#define _GPIO6_FSEL_Shift (18U)
#define _GPIO6_FSEL_Msk (0b111U << _GPIO6_FSEL_Shift)
#define _GPIO6_OUTPUT (0x1U << 6)

#define _GPIO9_FSEL_Shift (27U)
#define _GPIO9_FSEL_Msk (0b111U << _GPIO9_FSEL_Shift)
#define _GPIO9_OUTPUT (0x1U << 9)

#define _GPIO12_FSEL_Shift (6U)
#define _GPIO12_FSEL_Msk (0b111U << _GPIO12_FSEL_Shift)
#define _GPIO12_OUTPUT (0x1U << 12)

#define _GPIO13_FSEL_Shift (9U)
#define _GPIO13_FSEL_Msk (0b111U << _GPIO13_FSEL_Shift)
#define _GPIO13_OUTPUT (0x1U << 13)

#define _GPIO16_FSEL_Shift (18U)
#define _GPIO16_FSEL_Msk (0b111U << _GPIO16_FSEL_Shift)
#define _GPIO16_OUTPUT (0x1U << 16)

#define _GPIO17_FSEL_Shift (21U)
#define _GPIO17_FSEL_Msk (0b111U << _GPIO17_FSEL_Shift)
#define _GPIO17_OUTPUT (0x1U << 17)

#define _GPIO18_FSEL_Shift (24U)
#define _GPIO18_FSEL_Msk (0b111U << _GPIO18_FSEL_Shift)
#define _GPIO18_OUTPUT (0x1U << 18)

#define _GPIO19_FSEL_Shift (27U)
#define _GPIO19_FSEL_Msk (0b111U << _GPIO19_FSEL_Shift)
#define _GPIO19_OUTPUT (0x1U << 19)

#define _GPIO20_FSEL_Shift (0U)
#define _GPIO20_FSEL_Msk (0b111U << _GPIO20_FSEL_Shift)
#define _GPIO20_OUTPUT (0x1U << 20)

#define _GPIO21_FSEL_Shift (3U)
#define _GPIO21_FSEL_Msk (0b111U << _GPIO21_FSEL_Shift)
#define _GPIO21_OUTPUT (0x1U << 21)

#define _GPIO22_FSEL_Shift (6U)
#define _GPIO22_FSEL_Msk (0b111U << _GPIO22_FSEL_Shift)
#define _GPIO22_OUTPUT (0x1U << 22)

#define _GPIO23_FSEL_Shift (9U)
#define _GPIO23_FSEL_Msk (0b111U << _GPIO23_FSEL_Shift)
#define _GPIO23_OUTPUT (0x1U << 23)

#define _GPIO24_FSEL_Shift (12U)
#define _GPIO24_FSEL_Msk (0b111U << _GPIO24_FSEL_Shift)
#define _GPIO24_OUTPUT (0x1U << 24)

#define _GPIO25_FSEL_Shift (15U)
#define _GPIO25_FSEL_Msk (0b111U << _GPIO25_FSEL_Shift)
#define _GPIO25_OUTPUT (0x1U << 25)

#define _GPIO26_FSEL_Shift (18U)
#define _GPIO26_FSEL_Msk (0b111U << _GPIO26_FSEL_Shift)
#define _GPIO26_OUTPUT (0x1U << 26)

#define _GPIO27_FSEL_Shift (21U)
#define _GPIO27_FSEL_Msk (0b111U << _GPIO27_FSEL_Shift)
#define _GPIO27_OUTPUT (0x1U << 27)

#define _GPIO_LED1 _GPIO5_OUTPUT
#define _GPIO_LED2 _GPIO22_OUTPUT
#define _GPIO_LED3 _GPIO27_OUTPUT
#define _GPIO_LED4 _GPIO17_OUTPUT
#define _GPIO_LED5 _GPIO12_OUTPUT
#define _GPIO_LED6 _GPIO26_OUTPUT
#define _GPIO_LED7 _GPIO13_OUTPUT
#define _GPIO_LED8 _GPIO6_OUTPUT
#define _GPIO_LED9 _GPIO18_OUTPUT
#define _GPIO_LED10 _GPIO23_OUTPUT
#define _GPIO_LED11 _GPIO24_OUTPUT
#define _GPIO_LED12 _GPIO25_OUTPUT

volatile uint32_t * WorkerGPIO::_gpio_base = nullptr;
QFile * WorkerGPIO::_memfile = nullptr;

WorkerGPIO::WorkerGPIO(QObject *parent) : QObject{parent}{

}

WorkerGPIO::~WorkerGPIO(){
    if(_gpio_base){
        AllOff();
        _memfile->unmap((uchar*)_gpio_base);
        delete _memfile;
        _memfile = nullptr;
        _gpio_base = nullptr;
    }
}

void WorkerGPIO::Init(){
    if(!GPIO_Init())
        emit GPIOError("Failed to initialize GPIO");
}

bool WorkerGPIO::GPIO_Init(){

    try{
        _memfile = new QFile(_GPIO_MEM_FILE);
    }catch(...){
        return false;
    }

    if(!_memfile->open(QIODevice::ReadWrite)){
        delete _memfile;
        _memfile = nullptr;
        return false;
    }

    _gpio_base = (uint32_t *) _memfile->map(0,244);

    _memfile->close();

    if(_gpio_base == nullptr){
        delete _memfile;
        _memfile = nullptr;
        return false;
    }

    // Set Output Mode
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO5_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO5_FSEL_Shift);
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO6_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO6_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO12_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO12_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO13_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO13_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO17_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO17_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO18_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO18_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO22_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO22_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO23_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO23_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO24_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO24_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO25_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO25_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO26_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO26_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO27_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO27_FSEL_Shift);

    return true;
}

void WorkerGPIO::AllOn(){
    _GPIO_SET0 = _GPIO_LED1 | _GPIO_LED2  | _GPIO_LED3  | _GPIO_LED4 |
                 _GPIO_LED5 | _GPIO_LED6  | _GPIO_LED7  | _GPIO_LED8 |
                 _GPIO_LED9 | _GPIO_LED10 | _GPIO_LED11 | _GPIO_LED12 ;
}

void WorkerGPIO::AllOff(){
    _GPIO_CLR0 = _GPIO_LED1 | _GPIO_LED2  | _GPIO_LED3  | _GPIO_LED4 |
                 _GPIO_LED5 | _GPIO_LED6  | _GPIO_LED7  | _GPIO_LED8 |
                 _GPIO_LED9 | _GPIO_LED10 | _GPIO_LED11 | _GPIO_LED12 ;
}

void WorkerGPIO::TurnOn(LED btn){
    switch(btn){
    case LED::LED1:
        _GPIO_SET0 = _GPIO_LED1;
        break;
    case LED::LED2:
        _GPIO_SET0 = _GPIO_LED2;
        break;
    case LED::LED3:
        _GPIO_SET0 = _GPIO_LED3;
        break;
    case LED::LED4:
        _GPIO_SET0 = _GPIO_LED4;
        break;
    case LED::LED5:
        _GPIO_SET0 = _GPIO_LED5;
        break;
    case LED::LED6:
        _GPIO_SET0 = _GPIO_LED6;
        break;
    case LED::LED7:
        _GPIO_SET0 = _GPIO_LED7;
        break;
    case LED::LED8:
        _GPIO_SET0 = _GPIO_LED8;
        break;
    case LED::LED9:
        _GPIO_SET0 = _GPIO_LED9;
        break;
    case LED::LED10:
        _GPIO_SET0 = _GPIO_LED10;
        break;
    case LED::LED11:
        _GPIO_SET0 = _GPIO_LED11;
        break;
    case LED::LED12:
        _GPIO_SET0 = _GPIO_LED12;
        break;
    default:
        break;
    }
}

void WorkerGPIO::TurnOff(LED btn){
    switch(btn){
    case LED::LED1:
        _GPIO_CLR0 = _GPIO_LED1;
        break;
    case LED::LED2:
        _GPIO_CLR0 = _GPIO_LED2;
        break;
    case LED::LED3:
        _GPIO_CLR0 = _GPIO_LED3;
        break;
    case LED::LED4:
        _GPIO_CLR0 = _GPIO_LED4;
        break;
    case LED::LED5:
        _GPIO_CLR0 = _GPIO_LED5;
        break;
    case LED::LED6:
        _GPIO_CLR0 = _GPIO_LED6;
        break;
    case LED::LED7:
        _GPIO_CLR0 = _GPIO_LED7;
        break;
    case LED::LED8:
        _GPIO_CLR0 = _GPIO_LED8;
        break;
    case LED::LED9:
        _GPIO_CLR0 = _GPIO_LED9;
        break;
    case LED::LED10:
        _GPIO_CLR0 = _GPIO_LED10;
        break;
    case LED::LED11:
        _GPIO_CLR0 = _GPIO_LED11;
        break;
    case LED::LED12:
        _GPIO_CLR0 = _GPIO_LED12;
        break;
    default:
        break;
    }
}

void WorkerGPIO::AlternateBlink(){
    _blinkstate ^= 1;
    if(_blinkstate){
        _GPIO_SET0 = _GPIO_LED1  | _GPIO_LED3  |
                     _GPIO_LED6  | _GPIO_LED8  |
                     _GPIO_LED9  | _GPIO_LED11 ;
        _GPIO_CLR0 = _GPIO_LED2  | _GPIO_LED4  |
                     _GPIO_LED5  | _GPIO_LED7  |
                     _GPIO_LED10 | _GPIO_LED12 ;
    }else{
        _GPIO_SET0 = _GPIO_LED2  | _GPIO_LED4  |
                     _GPIO_LED5  | _GPIO_LED7  |
                     _GPIO_LED10 | _GPIO_LED12 ;
        _GPIO_CLR0 = _GPIO_LED1 | _GPIO_LED3  |
                     _GPIO_LED6 | _GPIO_LED8  |
                     _GPIO_LED9 | _GPIO_LED11 ;
    }
}

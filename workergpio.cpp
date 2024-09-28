#include "workergpio.h"

#define _GPIO_MEM_FILE "/dev/gpiomem"
#define _GPIO_FSEL_INPUT 0b000U
#define _GPIO_FSEL_OUTPUT 0b001U

#define _GPIO_FSEL0 *(_gpio_base + 0)
#define _GPIO_FSEL1 *(_gpio_base + 1)
#define _GPIO_FSEL2 *(_gpio_base + 2)
#define _GPIO_FSEL3 *(_gpio_base + 3)
#define _GPIO_FSEL4 *(_gpio_base + 4)
#define _GPIO_FSEL5 *(_gpio_base + 5)

#define _GPIO_SET0 *(_gpio_base + 7)
#define _GPIO_SET1 *(_gpio_base + 8)

#define _GPIO_CLR0 *(_gpio_base + 10)
#define _GPIO_CLR1 *(_gpio_base + 11)

#define _GPIO0_FSEL_Shift (0U)
#define _GPIO0_FSEL_Msk (0b111U << _GPIO0_FSEL_Shift)
#define _GPIO0_OUTPUT (0x1U << 0)

#define _GPIO1_FSEL_Shift (3U)
#define _GPIO1_FSEL_Msk (0b111U << _GPIO1_FSEL_Shift)
#define _GPIO1_OUTPUT (0x1U << 1)

#define _GPIO5_FSEL_Shift (15U)
#define _GPIO5_FSEL_Msk (0b111U << _GPIO5_FSEL_Shift)
#define _GPIO5_OUTPUT (0x1U << 5)

#define _GPIO6_FSEL_Shift (18U)
#define _GPIO6_FSEL_Msk (0b111U << _GPIO6_FSEL_Shift)
#define _GPIO6_OUTPUT (0x1U << 18)

#define _GPIO7_FSEL_Shift (21U)
#define _GPIO7_FSEL_Msk (0b111U << _GPIO7_FSEL_Shift)
#define _GPIO7_OUTPUT (0x1U << 7)

#define _GPIO12_FSEL_Shift (6U)
#define _GPIO12_FSEL_Msk (0b111U << _GPIO12_FSEL_Shift)
#define _GPIO12_OUTPUT (0x1U << 12)

#define _GPIO13_FSEL_Shift (9U)
#define _GPIO13_FSEL_Msk (0b111U << _GPIO13_FSEL_Shift)
#define _GPIO13_OUTPUT (0x1U << 13)

#define _GPIO16_FSEL_Shift (18U)
#define _GPIO16_FSEL_Msk (0b111U << _GPIO16_FSEL_Shift)
#define _GPIO16_OUTPUT (0x1U << 16)

#define _GPIO19_FSEL_Shift (27U)
#define _GPIO19_FSEL_Msk (0b111U << _GPIO19_FSEL_Shift)
#define _GPIO19_OUTPUT (0x1U << 19)

#define _GPIO20_FSEL_Shift (0U)
#define _GPIO20_FSEL_Msk (0b111U << _GPIO20_FSEL_Shift)
#define _GPIO20_OUTPUT (0x1U << 20)

#define _GPIO21_FSEL_Shift (3U)
#define _GPIO21_FSEL_Msk (0b111U << _GPIO21_FSEL_Shift)
#define _GPIO21_OUTPUT (0x1U << 21)

#define _GPIO26_FSEL_Shift (18U)
#define _GPIO26_FSEL_Msk (0b111U << _GPIO26_FSEL_Shift)
#define _GPIO26_OUTPUT (0x1U << 26)

uint32_t * WorkerGPIO::_gpio_base = nullptr;

WorkerGPIO::WorkerGPIO(QObject *parent) : QObject{parent}{

}

WorkerGPIO::~WorkerGPIO(){
    if(_gpio_base)
        QFile(_GPIO_MEM_FILE).unmap((uchar*)_gpio_base);
}

void WorkerGPIO::Init(){
    if(!GPIO_Init())
        emit GPIOError("Failed to initialize GPIO");
}

bool WorkerGPIO::GPIO_Init(){
    QFile fp(_GPIO_MEM_FILE);

    if(!fp.open(QIODevice::ReadWrite))
        return false;

    _gpio_base = (uint32_t *) fp.map(0,244);

    fp.close();

    if(_gpio_base == nullptr)
        return false;

    // Set Output Mode
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO0_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO0_FSEL_Shift);
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO1_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO1_FSEL_Shift);
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO5_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO5_FSEL_Shift);
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO6_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO6_FSEL_Shift);
    _GPIO_FSEL0 = (_GPIO_FSEL0 & (~_GPIO7_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO7_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO12_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO12_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO13_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO13_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO16_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO16_FSEL_Shift);
    _GPIO_FSEL1 = (_GPIO_FSEL1 & (~_GPIO19_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO19_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO20_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO20_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO21_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO21_FSEL_Shift);
    _GPIO_FSEL2 = (_GPIO_FSEL2 & (~_GPIO26_FSEL_Msk))|(_GPIO_FSEL_OUTPUT << _GPIO26_FSEL_Shift);

    //Set Value
    _GPIO_SET0 = _GPIO26_OUTPUT;

    return true;
}

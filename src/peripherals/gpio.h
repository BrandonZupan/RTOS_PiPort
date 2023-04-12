// gpio.h
// https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson01/include/peripherals/gpio.h

#ifndef	_P_GPIO_H
#define	_P_GPIO_H

#include "base.h"

#define GPFSEL1         (PBASE+0x00200004)
#define GPSET0          (PBASE+0x0020001C)
#define GPCLR0          (PBASE+0x00200028)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

#endif  /*_P_GPIO_H */
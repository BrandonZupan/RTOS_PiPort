// utils.h
// https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson01/include/utils.h

#ifndef	_BOOT_H
#define	_BOOT_H

extern void delay ( unsigned long);
extern void put32 ( unsigned long, unsigned int );
extern unsigned int get32 ( unsigned long );

#endif  /*_BOOT_H */
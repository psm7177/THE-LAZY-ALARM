#ifndef _BUTTON
#define _BUTTON
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1
 
#define BLOCK_SIZE      (4*1024)
#define PERI_BASE 0x20000000
#define GPIO_BASE       (PERI_BASE + 0x00200000)


static volatile unsigned int *gpio;
int button_wait;

void pin25_Mode(int mode)
{
    int fsel, shift;
	// int alt;

    fsel = 2;
    shift = 15;

	if (mode == INPUT)
		*(gpio + fsel) = (*(gpio + fsel) & ~(7 << shift));
	else if (mode == OUTPUT)
		*(gpio + fsel) = (*(gpio + fsel) & ~(7 << shift)) | (1 << shift);
}

int Read_pin25()
{
    int gpioToGPLEV=13;
	if ((*(gpio + gpioToGPLEV) & (1 << (25 & 31))) != 0) return HIGH; else return LOW;
}

#endif
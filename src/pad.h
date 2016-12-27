#ifndef PAD_H
#define PAD_H

#include <stdint.h>

#define padRegister (*(volatile uint8_t*)0x4016)

#define padA 0x80
#define padB 0x40
#define padSelect 0x20
#define padStart 0x10
#define padUp 0x08
#define padDown 0x04
#define padLeft 0x02
#define padRight 0x01

extern uint8_t padLastState;
#pragma zpsym("padLastState");

extern uint8_t padState;
#pragma zpsym("padState");

void padTick(void);

#endif
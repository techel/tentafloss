#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

typedef void(*staInitHandler)(void);
typedef void(*staTickHandler)(void);
typedef void(*staDeinitHandler)(void);

typedef struct
{
	staInitHandler Initialize;
	staTickHandler Tick;
	staDeinitHandler Deinitialize;
}staStateHandler;

extern const staStateHandler *staNewState;
#pragma zpsym("staNewState")

#define staNumTimers 2

typedef void(*staTimerHandler)(void);

void staSetTimer(uint8_t id, uint8_t ticks, staTimerHandler handler);
void staKillTimer(uint8_t id);

#endif
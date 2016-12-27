#include "main.h"
#include "ppu.h"
#include "pad.h"
#include "sound.h"
#include "intro.h"

typedef struct
{
	uint8_t Ticks, CurrentTicks;
	staTimerHandler Handler;

}Timer;

Timer Timers[staNumTimers];

#pragma bss-name("ZEROPAGE")
static const staStateHandler *ActiveState;
const staStateHandler *staNewState;
static uint8_t TickCount;
static Timer *pTimer;

void staSetTimer(uint8_t id, uint8_t ticks, staTimerHandler handler)
{
	pTimer = &Timers[id];
	pTimer->Ticks = ticks;
	pTimer->CurrentTicks = 0;
	pTimer->Handler = handler;
}
void staKillTimer(uint8_t id)
{
	Timers[id].Handler = 0;
}
void main()
{
	ppuControl = ppuCtrlNMI | ppuCtrlSprBase;
	ppuMask =  ppuMaskBg | ppuMaskSpr;
		
	ActiveState = &inrStateHandler;
	ActiveState->Initialize();
	staNewState = 0;

	for(;;)
	{
		while(ppuNMITicks == 0);
		ppuNMITicks = 0;
		
		sndTick();
		
		++TickCount;
		if(TickCount == 2)
		{
			padTick();
			ActiveState->Tick();
			
			if(staNewState)
			{
				ActiveState->Deinitialize();
				ActiveState = staNewState;
				staNewState = 0;
				ActiveState->Initialize();
			}
			
			for(TickCount = 0; TickCount < staNumTimers; ++TickCount)
			{
				pTimer = &Timers[TickCount];
				if(pTimer->Handler)
				{
					++pTimer->CurrentTicks;
					if(pTimer->CurrentTicks > pTimer->Ticks)
					{
						pTimer->CurrentTicks = 0;
						pTimer->Handler();
					}
				}
			}
			TickCount = 0;
		}		
	}
}
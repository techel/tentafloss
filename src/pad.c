#include "pad.h"

#pragma bss-name("ZEROPAGE")
uint8_t padLastState, padState;
static uint8_t i;

void padTick(void)
{
	padLastState = padState;
	padState = 0;
	padRegister = 1;
	padRegister = 0;
	for(i = 0; i < 8; ++i)
	{
		padState <<= 1;
		padState |= (padRegister & 1);
	}
}

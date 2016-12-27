#include "game.h"
#include "ppu.h"
#include "pad.h"
#include "sound.h"
#include "titlescreen.h"
#include <stdint.h>

#pragma bss-name("ZEROPAGE")

static const char Text[] = "insert game here";

static void gameInit(void)
{
	ppuNTClear();
	
	ppuBgPalette[0] = 0x0F;
	ppuSprPalette[0] = ppuBgPalette[1] = 0x1C;
	ppuSprPalette[1] = ppuBgPalette[2] = 0x2B;
	ppuSprPalette[2] = ppuBgPalette[3] = 0x39;
	
	ppuNTDrawText(0x204, Text, sizeof(Text));
}
static void gameTick(void)
{
	if(padState & padSelect)
	{
		ppuNTClear();
		sndPlay(&bgmIntro);
		staNewState = &titStateHandler;
	}
}
static void gameDeinit(void)
{
}

const staStateHandler gamStateHandler = { gameInit, gameTick, gameDeinit };
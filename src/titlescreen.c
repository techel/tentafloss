#include "titlescreen.h"
#include "sound.h"
#include "ppu.h"
#include "pad.h"
#include "game.h"
#include <stdint.h>

#pragma bss-name("ZEROPAGE")
static uint8_t TextVisible;
static int8_t UFOWave;

static const char Subtitle[] = "Press any button";

static void screenInit(void);

static void screenTimer1(void)
{
	TextVisible++;
	if(TextVisible & 1)
		ppuNTClearBlock(0x308, sizeof(Subtitle), 1);
	else
		ppuNTDrawText(0x308, Subtitle, sizeof(Subtitle));

	if(TextVisible == 2)
	{
		ppuSprDefineBlock(0, 0, 0, 5, 2);
		ppuSprPositionizeBlock(0, 0, 32, 5, 2);
	}

	if(TextVisible & 1)
		UFOWave = -UFOWave;
}
static void screenTimer2(void)
{
	sndPlay(&bgmBegin);
	staNewState = &gamStateHandler;
}
static void screenInit(void)
{	
	ppuBgPalette[0] = 0x0F;
	ppuSprPalette[0] = ppuBgPalette[1] = 0x18;
	ppuSprPalette[1] = ppuBgPalette[2] = 0x28;
	ppuSprPalette[2] = ppuBgPalette[3] = 0x39;

	ppuNTDrawBlock(0x128, 0x60, 16, 2);
	ppuNTDrawBlock(0x174, 0x80, 9, 8);
	
	staSetTimer(0, 12, screenTimer1);

	UFOWave = 1;
}
static void screenTick(void)
{
	if(TextVisible >= 2)
		ppuSprMoveBlock(0, 10, 2, UFOWave);

	if(padLastState == 0 && padState != 0 && TextVisible != 0xFF)
	{
		TextVisible = 0xFF;
		sndStop();
		sndPlaySfx(&sfxStart);
		staSetTimer(0, 20, screenTimer2);
	}
}
static void screenDeinit(void)
{
	staKillTimer(0);
	ppuSprClear(0, 10);
}

const staStateHandler titStateHandler = { screenInit, screenTick, screenDeinit };
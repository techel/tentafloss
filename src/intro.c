#include "intro.h"
#include "titlescreen.h"
#include "sound.h"
#include "ppu.h"
#include "pad.h"
#include <stdint.h>

#pragma bss-name("ZEROPAGE")

static const char *IntroText[] = { "The world has changed.",
									"The syndicate",
									"destroyed all planets",
									"except..." };
static uint8_t IntroTextLen[] = { 22, 13, 21, 9 };
static uint8_t IntroTextTicks[] = { 2, 6, 2, 8 };
static uint8_t IntroState, TextVisible, i;

static void introTimer2(void)
{
	i = IntroTextLen[IntroState-1];
	if(TextVisible < i)
	{
		do
		{
			++TextVisible;
		}
		while(IntroText[IntroState-1][TextVisible-1] == ' ');
		ppuNTDrawText(0x181 + 0x40*(IntroState-1) + (0x10-i/2), IntroText[IntroState-1], TextVisible);
	}
}
static void introTimer1(void)
{
	++IntroState;
	if(IntroState == 5)
	{
		staNewState = &titStateHandler;
	}
	else if(IntroState > 0)
	{
		staSetTimer(0, IntroState == 4 ? 100 : 94, introTimer1);
		staSetTimer(1, IntroTextTicks[IntroState-1], introTimer2);
		TextVisible = 0;
	}
}
static void introInit(void)
{
	sndPlay(&bgmIntro);
	
	ppuBgPalette[0] = 0x0F;
	ppuBgPalette[1] = 0x18;
	ppuBgPalette[2] = 0x28;
	ppuBgPalette[3] = 0x39;
	
	ppuNTClear();
	IntroState = 0;
	TextVisible = 0xFF;

	staSetTimer(0, 5, introTimer1);
}
static void introTick(void)
{
	if(padLastState == 0 && padState != 0)
	{
		sndPlaySfx(&sfxStart);
		staNewState = &titStateHandler;
	}
}
static void introDeinit(void)
{
	staKillTimer(0);
	staKillTimer(1);
	ppuNTClear();
}

const staStateHandler inrStateHandler = { introInit, introTick, introDeinit };
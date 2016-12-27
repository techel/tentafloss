#include "ppu.h"
#include <stdint.h>

#pragma bss-name("ZEROPAGE")
static uint16_t i;
static uint8_t x, y;
static uint8_t *p;

void ppuNTClear(void)
{
	for(i = 0; i < 1024; ++i)
		ppuNTBuffer[i] = 0;
}
void ppuNTDrawBlock(uint16_t ntidx, uint8_t patternidx, uint8_t w, uint8_t h)
{
	p = &ppuNTBuffer[ntidx];
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
			p[y * 32 + x] = patternidx + y * 16 + x;
	}
}
void ppuNTDrawText(uint16_t ntidx, const char *str, uint8_t size)
{
	p = &ppuNTBuffer[ntidx];
	x = 0;
	while(size--> 0)
		p[x++] = ppuChar(*str++);
}
void ppuNTClearBlock(uint16_t ntidx, uint8_t w, uint8_t h)
{
	p = &ppuNTBuffer[ntidx];
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
			p[y * 32 + x] = 0;
	}
}
void ppuSprClear(uint8_t spridx, uint8_t num)
{
	p = &ppuSprBuffer[spridx * 4];
	while(num--> 0)
	{
		*p = 0xFF;
		p += 4;
	}
}
void ppuSprDefineBlock(uint8_t spridx, uint8_t patternidx, uint8_t attrib, uint8_t w, uint8_t h)
{
	p = &ppuSprBuffer[spridx * 4 + 1];
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			*p = patternidx + y * 16 + x;
			++p;
			*p = attrib;
			p += 3;
		}
	}
}
void ppuSprPositionizeBlock(uint8_t spridx, uint8_t ox, uint8_t oy, uint8_t w, uint8_t h)
{
	p = &ppuSprBuffer[spridx * 4];
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			*p = oy + y * 8;
			p += 3;
			*p = ox + x * 8;
			++p;
		}
	}
}
void ppuSprMoveBlock(uint8_t spridx, uint8_t num, uint8_t x, uint8_t y)
{
	p = &ppuSprBuffer[spridx * 4];
	while(num--> 0)
	{
		*p += y;
		p += 3;
		*p += x;
		++p;
	}
}
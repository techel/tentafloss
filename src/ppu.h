#ifndef PPU_H
#define PPU_H

#include <stdint.h>

#define ppuControl  (*(volatile uint8_t*)0x2000)
#define ppuMask  (*(volatile uint8_t*)0x2001)
#define ppuStatus  (*(volatile const uint8_t*)0x2002)
#define ppuScroll   (*(volatile uint8_t*)0x2005)
#define ppuAddress  (*(volatile uint8_t*)0x2006)
#define ppuData  (*(volatile uint8_t*)0x2007)

#define ppuCtrl32Incr 0x04
#define ppuCtrlSprBase 0x08
#define ppuCtrlBgBase 0x10
#define ppuCtrlNMI 0x80

#define ppuMaskGrey 0x01
#define ppuMaskBg0 0x02
#define ppuMaskSpr0 0x04
#define ppuMaskBg 0x08
#define ppuMaskSpr 0x10

extern volatile uint8_t ppuNMITicks;
#pragma zpsym("ppuNMITicks");

extern uint8_t ppuBgPalette[4];
#pragma zpsym("ppuBgPalette");

extern uint8_t ppuSprPalette[3];
#pragma zpsym("ppuSprPalette");

extern uint8_t ppuNTBuffer[1024];
void ppuNTClear(void);
void ppuNTDrawBlock(uint16_t ntidx, uint8_t patternidx, uint8_t w, uint8_t h);
void ppuNTDrawText(uint16_t ntidx, const char *str, uint8_t size);
void ppuNTClearBlock(uint16_t ntidx, uint8_t w, uint8_t h);

#define ppuChar(c) ((c) - 0x20)

#define ppuSprBuffer (*(uint8_t(*)[256])0x200)
#define ppuSprAttrBehind 0x20
#define ppuSprAttribFlipH 0x40
#define ppuSprAttribFlipV 0x80

void ppuSprClear(uint8_t spridx, uint8_t num);
void ppuSprDefineBlock(uint8_t spridx, uint8_t patternidx, uint8_t attrib, uint8_t w, uint8_t h);
void ppuSprPositionizeBlock(uint8_t spridx, uint8_t ox, uint8_t oy, uint8_t w, uint8_t h);
void ppuSprMoveBlock(uint8_t spridx, uint8_t num, uint8_t x, uint8_t y);

#endif
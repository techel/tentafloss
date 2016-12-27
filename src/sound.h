#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

extern void bgmIntro;
extern void bgmBegin;
extern void sfxStart;
extern void sfxText;

void sndTick(void);
void fastcall sndPlay(const void *bgm);
void sndStop(void);
void fastcall sndPlaySfx(const void *sfx);

#endif
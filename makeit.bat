@echo off

cd src

cc65 -o main.tmp main.c
ca65 main.tmp

cc65 -o ppu.tmp ppu.c
ca65 ppu.tmp

cc65 -o pad.tmp pad.c
ca65 pad.tmp

cc65 -o intro.tmp intro.c
ca65 intro.tmp

cc65 -o titlescreen.tmp titlescreen.c
ca65 titlescreen.tmp

cc65 -o game.tmp game.c
ca65 game.tmp

ca65 init.s

ld65 -C ../nes.cfg -o ../tentafloss.nes init.o main.o ppu.o pad.o intro.o titlescreen.o game.o nes.lib

del *.tmp
del *.o


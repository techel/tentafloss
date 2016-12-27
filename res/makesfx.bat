@echo off
famitracker "%1" -export "%~n1.nsf"
tools\nsf2data "%~n1.nsf" -ca65 -ntsc
del "%~n1.nsf"
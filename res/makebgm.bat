@echo off
famitracker "%1" -export "%~n1.txt"
tools\text2vol "%~n1.txt" -ca65 -ntsc
del "%~n1.txt"
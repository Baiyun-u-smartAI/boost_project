@echo off
gcc answer2.c correct.hpp -shared -fPIC -o libcorrect.dll
gcc correct.c correct.hpp libcorrect.dll -o correct.exe
exit

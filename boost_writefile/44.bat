@echo off
echo ÎÒ°®ÖÐ¹ú
gcc "C:\Users\pc002\CLionProjects\boost_writefile\m.c" -o test1.exe
echo.5980> input.txt 
echo.33086>> input.txt
echo. >> input.txt
if exist test1.exe (
start /B test1.exe < input.txt)
pause
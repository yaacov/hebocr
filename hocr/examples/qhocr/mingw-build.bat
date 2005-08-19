@echo off

rem *
rem * A small utility for compiling Qt applications on windows. 
rem * Sets paths, and calls the make utility.
rem * Version 0.01, Diego Iastrubni <elcuco@kde.org> 2005
rem *
rem * Public domain

rem *
rem * you only need to change the 2 sets, match the dir where you installed Qt-free and mingw
rem *
set MINGWDIR=C:\MinGW\bin
set QTDIR=C:\Qt\4.0.0

set PATH=C:\Qt\4.0.0\bin
set PATH=%PATH%;C:\MinGW\bin
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++
set APPNAME=qhocr

echo Setting up a MinGW/Qt only environment...
echo -- QTDIR set to %QTDIR%
echo -- PATH set to %QTDIR%\bin
echo -- Adding %MINGWDIR%\bin to PATH
echo -- Adding %SystemRoot%\System32 to PATH
echo -- QMAKESPEC set to %QMAKESPEC%

rem DO NOT EDIT BELLOW THIS LINE
rem -------------------------------------------------

echo.
echo.
echo Calling qmake...
qmake 

echo Compiling using mingw32-make
del /q release\*.exe
mingw32-make all

if EXIST release\*.exe GOTO COMPILATION_OK

:COMPILATION_FAILED
echo.
echo.
echo Compilation failed. 
echo.

goto END_BATCH

:COMPILATION_OK
copy release\*.exe .
echo.
echo.
echo Application %APPNAME% has been compiled. Enjoy!
echo.

:END_BATCH
pause

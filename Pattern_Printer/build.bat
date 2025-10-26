@echo off
REM =============================
REM Cross-platform Build Script (Windows)
REM =============================

REM Check for history folder
if not exist history (
    mkdir history
    echo Created history folder.
)

REM Check for build folder
if not exist build (
    mkdir build
    echo Created build folder.
)

REM Set executable name
set EXE=build\pattern_printer.exe

REM Compile the program (source files are in src/)
echo Compiling Pattern Printer...
gcc src\main.c src\patterns.c -o %EXE% -Wall -Wextra -O2

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

echo Compilation successful!
echo Running program...
%EXE%

echo.
echo Program finished. Output stored in history\output.txt
pause

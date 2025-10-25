@echo off
REM =============================
REM Cross-platform Build Script
REM =============================

REM Check for history folder
if not exist history (
    mkdir history
    echo Created history folder.
)

REM Detect OS
ver | findstr /i "windows" >nul
if %errorlevel%==0 (
    set EXE=pattern_printer.exe
) else (
    set EXE=pattern_printer
)

REM Compile the program
echo Compiling Pattern Printer...
gcc main.c patterns.c -o %EXE% -Wall -Wextra -O2

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

echo Compilation successful!
echo Running program...
%EXE%

echo.
echo Program finished. Output stored in history/output.txt
pause

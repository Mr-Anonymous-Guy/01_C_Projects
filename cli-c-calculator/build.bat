@echo off
REM =====================================================
REM Cross-Platform Windows Build Script
REM =====================================================

REM Create build folder
IF NOT EXIST build (
    mkdir build
)

REM Detect C++ first, then C
IF EXIST src\*.cpp (
    g++ src\*.cpp -Iinclude -Wall -std=c++17 -o build\app.exe -lm
) ELSE IF EXIST src\*.c (
    gcc src\*.c -Iinclude -Wall -o build\app.exe -lm
) ELSE (
    echo ❌ No source files found in src\ folder!
    pause
    exit /b 1
)

REM Check if compilation succeeded
IF %ERRORLEVEL% NEQ 0 (
    echo ❌ Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

REM Run the program
echo 🚀 Running program...
build\app.exe

pause

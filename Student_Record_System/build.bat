@echo off
REM ===============================
REM Student Record System Build & Run
REM ===============================

echo ----------------------------------------
echo Building Student Record System...
echo ----------------------------------------

REM Try to remove old executable
IF EXIST student_system.exe (
    echo Removing old executable...
    DEL /F /Q student_system.exe
)

REM Compile the project using gcc
gcc -Wall -Wextra -Iincludes src\main.c src\student.c -o student_system.exe

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Compilation failed!
    pause
    exit /b 1
)

echo ✅ Compilation successful!
echo ----------------------------------------
echo Running the program...
echo ----------------------------------------

REM Run the executable
student_system.exe

echo ----------------------------------------
echo Program terminated.
pause

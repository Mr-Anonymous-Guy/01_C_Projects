@echo off
setlocal enabledelayedexpansion

:: -----------------------------------------------------------------------------
:: ATM MANAGEMENT SYSTEM — BATCH LAUNCHER & BUILD SCRIPT
:: Supports Windows CMD / PowerShell, VS Code terminal detection, standard colors,
:: and compilation modes: clean, debug, and release.
:: -----------------------------------------------------------------------------

:: 1. Color setup using virtual terminal escape characters
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set "ESC=%%b"
set "RED=%ESC%[91m"
set "GREEN=%ESC%[92m"
set "YELLOW=%ESC%[93m"
set "BLUE=%ESC%[94m"
set "CYAN=%ESC%[96m"
set "RESET=%ESC%[0m"

:: 2. Terminal UI Header
cls
echo %CYAN%============================================================%RESET%
echo %CYAN%                 💎 ATM MANAGEMENT SYSTEM 💎                %RESET%
echo %CYAN%============================================================%RESET%
echo.

:: 3. VS Code terminal environment check
if "%TERM_PROGRAM%"=="vscode" (
    echo %BLUE%[INFO]%RESET% VS Code Terminal environment detected.
)

:: 4. Evaluate command line flags (clean, debug, release)
set "CLEAN_ONLY=0"
set "BUILD_MODE=Standard"
set "EXTRA_FLAGS="

if /I "%1"=="clean" (
    set "CLEAN_ONLY=1"
    goto do_clean
)
if /I "%1"=="debug" (
    set "BUILD_MODE=Debug"
    set "EXTRA_FLAGS=-g -DDEBUG"
)
if /I "%1"=="release" (
    set "BUILD_MODE=Release"
    set "EXTRA_FLAGS=-O2"
)

:: 5. Verify compiler existence
echo %BLUE%[INFO]%RESET% Checking compiler...
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo %RED%[ERROR] GCC compiler not found.%RESET%
    echo %YELLOW%[HELP] Install MinGW-w64 and add GCC to your environment variables PATH.%RESET%
    echo.
    pause
    exit /b 1
)
echo %GREEN%[SUCCESS]%RESET% GCC found:
gcc --version | findstr /i "gcc"

:: 6. Verify directory/source integrity
if not exist src\main.c (
    echo %RED%[ERROR] Missing source files. Check that src\main.c exists.%RESET%
    pause
    exit /b 1
)

:: 7. Create build folder if missing
echo %BLUE%[INFO]%RESET% Creating build directory...
if not exist build (
    mkdir build
    if !errorlevel! neq 0 (
        echo %RED%[ERROR] Failed to create build folder.%RESET%
        pause
        exit /b 1
    )
)

:: 8. Clean target executable before building
echo %BLUE%[INFO]%RESET% Cleaning old executable...
if exist build\atm.exe (
    del /f /q build\atm.exe
    if !errorlevel! neq 0 (
        echo %RED%[ERROR] Failed to delete build\atm.exe.%RESET%
        echo %YELLOW%[REASON] The program might be running in another terminal.%RESET%
        pause
        exit /b 1
    )
)

:: 9. Compilation step
echo %BLUE%[INFO]%RESET% Compiling project in %BUILD_MODE% mode...
gcc -Wall -Wextra -std=c99 %EXTRA_FLAGS% -Iinclude src/*.c -o build/atm.exe

if %errorlevel% neq 0 (
    echo.
    echo %RED%[ERROR] Compilation failed.%RESET%
    pause
    exit /b 1
)
echo %GREEN%[SUCCESS]%RESET% Build completed: build\atm.exe
echo.

:: 10. Execute binary
echo Launching ATM System...
echo %CYAN%------------------------------------------------------------%RESET%
build\atm.exe
goto :eof

:: 11. Cleanup routine
:do_clean
echo %BLUE%[INFO]%RESET% Cleaning build outputs...
if exist build (
    rd /s /q build
    echo %GREEN%[SUCCESS]%RESET% Build directory cleaned successfully.
) else (
    echo %YELLOW%[INFO]%RESET% Nothing to clean.
)
exit /b 0

@echo off
setlocal

title NovaShell Launcher

echo ========================================
echo           NovaShell Launcher
echo ========================================
echo.

REM --- 1. Check WSL availability ---
where wsl >nul 2>&1
if errorlevel 1 (
    echo [ERROR] WSL not found. Install with: wsl --install
    pause
    exit /b 1
)

REM --- 2. Resolve Windows project directory ---
REM %~dp0 = Drive+Path of this .bat, regardless of where it's launched from.
set "WIN_DIR=%~dp0"
if "%WIN_DIR:~-1%"=="\" set "WIN_DIR=%WIN_DIR:~0,-1%"

echo [INFO] Windows path : %WIN_DIR%

REM --- 3. Write the Windows project path to a temp file ---
REM This completely isolates it from CMD's quote/escape parser.
REM %TEMP% is guaranteed to be short and space-free on all Windows installs.
set "TMPFILE=%TEMP%\novashell_path.tmp"
echo %WIN_DIR%>"%TMPFILE%"

REM --- 4. Translate the temp file's own location to WSL format ---
REM %TEMP% path has no spaces so this single wslpath call is safe.
set "WSL_TMPFILE="
for /f "usebackq delims=" %%L in (`wsl wslpath -u "%TMPFILE%"`) do set "WSL_TMPFILE=%%L"

if not defined WSL_TMPFILE (
    del "%TMPFILE%" >nul 2>&1
    echo [ERROR] Cannot translate temp file path. Is WSL installed and running?
    pause
    exit /b 1
)

REM --- 5. Also translate the script directory to WSL format ---
REM We use nova_wslpath.sh (a bash helper living in the project root).
REM This helper accepts the WSL path to the temp file and returns the
REM translated WSL project path. All dangerous characters (<, >, |, %)
REM stay inside Bash and never touch CMD's parser.
set "WSL_SCRIPT_DIR="
for /f "usebackq delims=" %%L in (`wsl wslpath -u "%WIN_DIR%"`) do set "WSL_SCRIPT_DIR=%%L"

if not defined WSL_SCRIPT_DIR (
    del "%TMPFILE%" >nul 2>&1
    echo [ERROR] Failed to resolve WSL script directory.
    pause
    exit /b 1
)

REM --- 6. Run the Bash helper to get WSL project dir from temp file ---
set "WSL_DIR="
for /f "usebackq delims=" %%L in (`wsl bash "%WSL_SCRIPT_DIR%/nova_wslpath.sh" "%WSL_TMPFILE%"`) do set "WSL_DIR=%%L"

del "%TMPFILE%" >nul 2>&1

if not defined WSL_DIR (
    echo [ERROR] Failed to translate project path to WSL format.
    pause
    exit /b 1
)

echo [INFO] WSL path     : %WSL_DIR%
echo.

REM --- 7. Default command ---
set "NOVA_CMD=%*"
if not defined NOVA_CMD set "NOVA_CMD=run"

echo [INFO] Command      : %NOVA_CMD%
echo [INFO] Handing off to WSL...
echo.

REM --- 8. Execute inside WSL ---
wsl bash -lc "cd '%WSL_DIR%' && chmod +x run.sh && ./run.sh %NOVA_CMD%"
set "EXIT_CODE=%ERRORLEVEL%"

echo.
echo ========================================
if %EXIT_CODE% EQU 0 (
    echo [OK] NovaShell exited cleanly ^(code 0^).
) else (
    echo [FAIL] Exit code: %EXIT_CODE%
)
echo ========================================
echo.

REM Pause only when double-clicked (Explorer uses cmd /c)
echo %cmdcmdline% | find /i "/c" >nul 2>&1
if not errorlevel 1 pause

exit /b %EXIT_CODE%

@echo off
echo ========================================
echo    xmake Build Script
echo ========================================
echo.

echo Checking xmake...
where xmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: xmake not found in PATH
    pause
    exit /b 1
)

echo Building firmware with xmake -v...
echo.
cd /d "%~dp0"
xmake build -v
if errorlevel 1 (
    echo ERROR: Failed to build root project
    pause
    exit /b 1
)
echo Root build firmware completed successfully.
echo.

pause
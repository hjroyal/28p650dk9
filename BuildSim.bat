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


echo Building sim/buck project with xmake -F xmake.lua...
echo.
cd /d "%~dp0sim\buck"
xmake -F xmake.lua
if errorlevel 1 (
    echo ERROR: Failed to build sim/buck project
    pause
    exit /b 1
)
echo Sim/buck build completed successfully.
echo.

pause
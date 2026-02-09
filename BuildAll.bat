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

echo [Step 1/2] Building root project with xmake -v...
echo.
cd /d "%~dp0"
xmake build -v
if errorlevel 1 (
    echo ERROR: Failed to build root project
    pause
    exit /b 1
)
echo Root build completed successfully.
echo.

@REM pause


echo [Step 2/2] Building sim/buck project with xmake -F xmake.lua...
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

echo ========================================
echo    Build Summary
echo ========================================
echo All builds completed successfully!
echo.
echo Output locations:
echo - Root:      user\build\dev_28p650dk9_v1_10.out
echo - Sim/Buck:  sim\buck\build\*
echo ========================================
echo.
pause
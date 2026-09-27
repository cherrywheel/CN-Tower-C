@echo off
rem sets up everything to build and play cn tower on windows
rem on everything else use setup.sh
rem
rem usage: setup.cmd [options]
rem   --yes      install whatever is missing without asking
rem   --gcc      build with gcc or clang even if msvc is there
rem   --no-test  skip playing through to the win after the build
rem   --help     show this

setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

set "YES=0"
set "TEST=1"
set "PREFER_GCC=0"

:args
if "%~1"=="" goto args_done
if /i "%~1"=="--yes" set "YES=1"
if /i "%~1"=="-y" set "YES=1"
if /i "%~1"=="--gcc" set "PREFER_GCC=1"
if /i "%~1"=="--no-test" set "TEST=0"
if /i "%~1"=="--help" goto help
if /i "%~1"=="-h" goto help
shift
goto args
:args_done

echo.
echo == looking around
echo system: windows %PROCESSOR_ARCHITECTURE%

rem vcvarsall wants x64 x86 or arm64
set "VCARCH=x64"
if /i "%PROCESSOR_ARCHITECTURE%"=="x86" set "VCARCH=x86"
if /i "%PROCESSOR_ARCHITECTURE%"=="ARM64" set "VCARCH=arm64"

echo.
echo == c compiler
call :find_msvc
call :find_gcc
if "%PREFER_GCC%"=="1" if defined GCC goto build_gcc
if defined VS_PATH goto build_msvc
if defined GCC goto build_gcc

echo no c compiler yet
where winget >nul 2>nul
if errorlevel 1 (
    echo install visual studio build tools with the c++ workload or mingw and rerun this
    exit /b 1
)
if "%YES%"=="1" goto install_msvc
choice /c yn /m "install visual studio build tools with winget"
if errorlevel 2 (
    echo ok then install a c compiler yourself and rerun this
    exit /b 1
)

:install_msvc
winget install --id Microsoft.VisualStudio.2022.BuildTools -e --accept-source-agreements --accept-package-agreements --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
call :find_msvc
if defined VS_PATH goto build_msvc
echo still no compiler so open a new terminal and rerun this
exit /b 1

:build_msvc
echo using msvc from !VS_PATH!
call "!VS_PATH!\VC\Auxiliary\Build\vcvarsall.bat" %VCARCH% >nul
if errorlevel 1 exit /b 1
echo.
echo == building
pushd src
nmake /nologo
if errorlevel 1 (
    popd
    exit /b 1
)
popd
goto test

:build_gcc
echo using !GCC!
echo.
echo == building
pushd src
!GCC! -std=c99 -Wall -Wextra -O2 -I..\include dialogues.c game.c main.c ui.c utils.c -o cn_tower_game.exe
if errorlevel 1 (
    popd
    exit /b 1
)
popd
goto test

:test
if "%TEST%"=="0" goto done
echo.
echo == playing through to the win
pushd src
set "CN_TOWER_SEED=1"
cn_tower_game.exe < ..\tests\win_path.txt > ..\win_output.txt 2>&1
set "CN_TOWER_SEED="
popd
findstr /c:"(Win)" win_output.txt >nul
if errorlevel 1 (
    type win_output.txt
    echo the build runs but didnt win something is off
    exit /b 1
)
del win_output.txt
echo won the game so the build works

:done
echo.
echo == all set
echo play it
echo   cd src ^&^& cn_tower_game.exe
exit /b 0

rem finds visual studio with the c++ tools through vswhere
:find_msvc
set "VS_PATH="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" exit /b 0
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_PATH=%%i"
exit /b 0

rem finds gcc or clang from mingw msys2 llvm and friends
:find_gcc
set "GCC="
for %%c in (gcc clang cc) do (
    if not defined GCC (
        where %%c >nul 2>nul && set "GCC=%%c"
    )
)
exit /b 0

:help
echo sets up everything to build and play cn tower on windows
echo on everything else use setup.sh
echo.
echo usage: setup.cmd [options]
echo   --yes      install whatever is missing without asking
echo   --gcc      build with gcc or clang even if msvc is there
echo   --no-test  skip playing through to the win after the build
echo   --help     show this
exit /b 0

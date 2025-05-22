@echo off
setlocal enabledelayedexpansion

REM Detect Visual Studio installation
set "VSPATH="

REM Check for VS2022
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional" (
    set "VSPATH=C:\Program Files\Microsoft Visual Studio\2022\Professional"
    set "VSVER=2022"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise" (
    set "VSPATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
    set "VSVER=2022"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community" (
    set "VSPATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
    set "VSVER=2022"
)

REM Check for VS2019 if VS2022 not found
if "%VSPATH%"=="" (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional"
        set "VSVER=2019"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise"
        set "VSVER=2019"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community"
        set "VSVER=2019"
    )
)

REM Check for VS2017 if VS2019 not found
if "%VSPATH%"=="" (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional"
        set "VSVER=2017"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise"
        set "VSVER=2017"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community" (
        set "VSPATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community"
        set "VSVER=2017"
    )
)

if "%VSPATH%"=="" (
    echo Visual Studio installation not found.
    echo Supported versions: 2017, 2019, 2022
    echo Please install Visual Studio or update this script with the correct path.
    exit /b 1
)

echo Using Visual Studio %VSVER% found at: %VSPATH%

REM Create VS2022 directory if it doesn't exist
if not exist VS2022 mkdir VS2022
pushd VS2022

REM Download NuGet if it doesn't exist
if not exist nuget.exe (
    echo Downloading NuGet...
    powershell -Command "Invoke-WebRequest -Uri 'https://dist.nuget.org/win-x86-commandline/latest/nuget.exe' -OutFile 'nuget.exe'"
)

REM Restore packages
echo Restoring packages...
nuget.exe restore NyaViewer.sln

REM Build the solution
echo Building solution...
"%VSPATH%\MSBuild\Current\Bin\MSBuild.exe" NyaViewer.sln /p:Configuration=Release

popd

if %errorlevel% neq 0 (
    echo Build failed
    exit /b 1
)

echo Build completed successfully
exit /b 0

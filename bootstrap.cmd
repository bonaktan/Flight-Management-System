@echo off
setlocal EnableDelayedExpansion
title Drogon Bootstrap - VS 2026

:: ============================================================
::  bootstrap-drogon.cmd
::  Bootstraps a full Drogon C++ dev environment on Windows:
::    - VS 2026 Build Tools (MSVC v145, Win11 SDK, CMake, built-in vcpkg component)
::    - External vcpkg at C:\vcpkg (classic mode for Drogon)
::    - Node.js v24.15.0 LTS
:: ============================================================

:: ── Run with Administrator privileges ─────────────────────
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

:: If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
    "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    pushd "%CD%"
    CD /D "%~dp0"


:: ── Target versions ─────────────────────────────────────────
set "NODE_TARGET_MAJOR=24"
set "NODE_VERSION=24.15.0"
set "NODE_URL=https://nodejs.org/dist/v24.15.0/node-v24.15.0-x64.msi"

:: ── Paths ───────────────────────────────────────────────────
set "TEMP_DIR=%TEMP%\drogon-bootstrap"
set "VS_INSTALLER=%TEMP_DIR%\vs_buildtools.exe"
set "VS_INSTALL_DIR=A:\Programs\FlightMgmtTools\BuildTools"
set "VCPKG_ROOT=A:\Programs\FlightMgmtTools\vcpkg"
set "VS_URL=https://aka.ms/vs/18/stable/vs_buildtools.exe"
set "VS_VCPKG=%VS_INSTALL_DIR%\VC\vcpkg\vcpkg.exe"
set "VS_VCPKG_TOOLCHAIN=%VS_INSTALL_DIR%\VC\vcpkg\scripts\buildsystems\vcpkg.cmake"

if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo.
echo ============================================================
echo   Drogon Bootstrap for Windows - Flight Management System
echo   VS 2026 Build Tools + vcpkg + Node v%NODE_VERSION% LTS
echo ============================================================


:: ── Resolve Powershell ─────────────────────────────────────────
set "PS_EXE="
for %%D in (
    "%SystemRoot%\System32\WindowsPowerShell\v1.0"
    "%SystemRoot%\SysNative\WindowsPowerShell\v1.0"
    "%ProgramFiles%\PowerShell\7"
    "%ProgramFiles(x86)%\PowerShell\7"
) do (
    if "!PS_EXE!"=="" (
        if exist "%%~D\pwsh.exe"       set "PS_EXE=%%~D\pwsh.exe"
        if exist "%%~D\powershell.exe" if "!PS_EXE!"=="" set "PS_EXE=%%~D\powershell.exe"
    )
)
if "!PS_EXE!"=="" (
    echo [ERR] Cannot locate powershell.exe or pwsh.exe.
    echo       Ensure PowerShell is installed and try again.
    pause & exit /b 1
)
echo     [OK] PowerShell resolved: !PS_EXE!

:: ── Helper: append a directory to the machine PATH (safe) ──
:: Usage: call :AppendPath "C:\some\dir"
:: Uses PowerShell to read+write the full PATH atomically.
:: Also appends to the current session PATH for immediate use.
goto :SkipFunctions

:AppendPath
    set "_AP_DIR=%~1"
    :: Add to current session immediately
    set "PATH=%PATH%;!_AP_DIR!"
    :: Persist to machine PATH via PowerShell (safe for long PATHs)
    "!PS_EXE!" -NoProfile -NonInteractive -Command ^
        "$p = [Environment]::GetEnvironmentVariable('PATH','Machine');" ^
        "if ($p -notlike '*!_AP_DIR!*') {" ^
        "  [Environment]::SetEnvironmentVariable('PATH', $p + ';!_AP_DIR!', 'Machine')" ^
        "}"
    goto :eof

:SetMachineVar
    :: Usage: call :SetMachineVar VARNAME "value"
    "!PS_EXE!" -NoProfile -NonInteractive -Command ^
        "[Environment]::SetEnvironmentVariable('%~1','%~2','Machine')"
    goto :eof

:SkipFunctions


:: ════════════════════════════════════════════════════════════
:: 1. Download VS 2026 Build Tools bootstrapper
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [1/8] Downloading Visual Studio 2026 Build Tools...

curl -L --progress-bar -o "%VS_INSTALLER%" "%VS_URL%"
if %errorlevel% neq 0 (
    echo [ERR] Failed to download VS 2026 Build Tools.
    pause & exit /b 1
)
echo     [OK] Downloaded vs_buildtools.exe


:: ════════════════════════════════════════════════════════════
:: 2. Install MSVC v145, Win11 SDK, CMake, built-in vcpkg
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [2/8] Installing VS 2026 Build Tools components...
echo     - Microsoft.VisualStudio.Workload.VCTools  ^(MSVC v145^)
echo     - Microsoft.VisualStudio.Component.VC.CMake.Project
echo     - Microsoft.VisualStudio.Component.Windows11SDK.22621
echo     - Microsoft.VisualStudio.Component.Vcpkg
echo     ^(This may take 15-30 minutes^)

"%VS_INSTALLER%" ^
    --passive ^
    --wait ^
    --norestart ^
    --nocache ^
    --installPath "%VS_INSTALL_DIR%" ^
    --add Microsoft.VisualStudio.Workload.VCTools ^
    --add Microsoft.VisualStudio.Component.VC.CMake.Project ^
    --add Microsoft.VisualStudio.Component.Windows11SDK.22621 ^
    --add Microsoft.VisualStudio.Component.Vcpkg ^
    --includeRecommended

if %errorlevel% neq 0 (
    if %errorlevel% neq 3010 (
        echo [ERR] VS 2026 Build Tools installer failed ^(exit code %errorlevel%^)
        pause & exit /b 1
    )
)
echo     [OK] VS 2026 Build Tools installed at %VS_INSTALL_DIR%


:: ════════════════════════════════════════════════════════════
:: 3. Integrate built-in vcpkg + add CMake to PATH
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [3/8] Integrating built-in vcpkg and CMake...

if exist "%VS_VCPKG%" (
    "%VS_VCPKG%" integrate install >nul 2>&1
    echo     [OK] Built-in vcpkg integrated
) else (
    echo     [!!] Built-in vcpkg not found. Will use external vcpkg toolchain.
    set "VS_VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
)

set "CMAKE_DIR=%VS_INSTALL_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
if exist "%CMAKE_DIR%\cmake.exe" (
    call :AppendPath "%CMAKE_DIR%"
    echo     [OK] CMake added to PATH
) else (
    echo     [!!] CMake not found at expected VS path.
)


:: ════════════════════════════════════════════════════════════
:: 4. Install Git
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [4/8] Checking for Git...

where git >nul 2>&1
if %errorlevel% neq 0 (
    echo     Git not found. Downloading Git for Windows...
    set "GIT_INSTALLER=%TEMP_DIR%\git-installer.exe"
    curl -L --progress-bar -o "!GIT_INSTALLER!" "https://github.com/git-for-windows/git/releases/download/v2.44.0.windows.1/Git-2.44.0-64-bit.exe"
    if !errorlevel! neq 0 ( echo [ERR] Failed to download Git. & pause & exit /b 1 )

    "!GIT_INSTALLER!" /VERYSILENT /NORESTART /NOCANCEL /SP- /CLOSEAPPLICATIONS /NOICONS /COMPONENTS="icons,ext\reg\shellhere,assoc,assoc_sh"
    if !errorlevel! neq 0 ( echo [ERR] Git installation failed. & pause & exit /b 1 )

    :: Git installs to Program Files\Git\cmd — append it now
    call :AppendPath "%ProgramFiles%\Git\cmd"
    echo     [OK] Git installed
) else (
    echo     [OK] Git already available
)


:: ════════════════════════════════════════════════════════════
:: 5. Clone and bootstrap external vcpkg
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [5/8] Setting up external vcpkg at %VCPKG_ROOT%...

if exist "%VCPKG_ROOT%\.git" (
    echo     vcpkg already cloned, pulling latest...
    git -C "%VCPKG_ROOT%" pull --quiet
) else (
    git clone https://github.com/microsoft/vcpkg.git "%VCPKG_ROOT%" --depth 1
    if !errorlevel! neq 0 ( echo [ERR] Failed to clone vcpkg. & pause & exit /b 1 )
)

set "PATH=!PS_EXE:~0,-15!;%PATH%"

call "%VCPKG_ROOT%\bootstrap-vcpkg.bat" -disableMetrics
if %errorlevel% neq 0 ( echo [ERR] vcpkg bootstrap failed. & pause & exit /b 1 )

call :SetMachineVar VCPKG_ROOT "%VCPKG_ROOT%"
call :AppendPath "%VCPKG_ROOT%"
echo     [OK] vcpkg ready at %VCPKG_ROOT%\vcpkg.exe


:: ════════════════════════════════════════════════════════════
:: 6. Install Drogon via external vcpkg
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [6/8] Installing Drogon via vcpkg ^(x64-windows^)...
echo     ^(This may take 30-60 minutes^)

"%VCPKG_ROOT%\vcpkg.exe" install drogon:x64-windows
if %errorlevel% neq 0 ( echo [ERR] vcpkg install drogon failed. & pause & exit /b 1 )
echo     [OK] Drogon installed


:: ════════════════════════════════════════════════════════════
:: 7. Node.js v24 LTS
:: ════════════════════════════════════════════════════════════
echo.
echo =^> [7/8] Checking Node.js installation...

set "NODE_NEEDS_INSTALL=1"
set "NODE_FOUND_VERSION=none"

where node >nul 2>&1
if %errorlevel% equ 0 (
    for /f "tokens=*" %%V in ('node --version 2^>nul') do set "NODE_FOUND_VERSION=%%V"
    set "NODE_FOUND_STRIPPED=!NODE_FOUND_VERSION:~1!"
    for /f "tokens=1 delims=." %%M in ("!NODE_FOUND_STRIPPED!") do set "NODE_FOUND_MAJOR=%%M"
    if "!NODE_FOUND_MAJOR!"=="%NODE_TARGET_MAJOR%" set "NODE_NEEDS_INSTALL=0"
)

if "!NODE_NEEDS_INSTALL!"=="0" (
    echo     [OK] Node.js v%NODE_TARGET_MAJOR%.x already installed ^(!NODE_FOUND_VERSION!^) -- skipping
) else (
    if not "!NODE_FOUND_VERSION!"=="none" (
        echo     [!!] Found Node.js !NODE_FOUND_VERSION! -- wrong major version.
        echo          Installing Node.js v%NODE_VERSION% LTS ^(replaces existing^).
    ) else (
        echo     Node.js not found. Installing v%NODE_VERSION% LTS...
    )

    set "NODE_INSTALLER=%TEMP_DIR%\node-installer.msi"
    curl -L --progress-bar -o "!NODE_INSTALLER!" "%NODE_URL%"
    if !errorlevel! neq 0 ( echo [ERR] Failed to download Node.js. & pause & exit /b 1 )

    msiexec /i "!NODE_INSTALLER!" /quiet /norestart ADDLOCAL=ALL
    if !errorlevel! neq 0 ( echo [ERR] Node.js installation failed. & pause & exit /b 1 )

    :: Refresh session PATH from machine (Node MSI manages its own entry)
    for /f "usebackq tokens=*" %%P in (
        `"!PS_EXE!" -NoProfile -NonInteractive -Command "[Environment]::GetEnvironmentVariable('PATH','Machine')"`
    ) do set "PATH=%%P"

    echo     [OK] Node.js v%NODE_VERSION% LTS installed
)

:: ════════════════════════════════════════════════════════════
:: 8. Project-Specific Configuration
:: ════════════════════════════════════════════════════════════
echo =^> [8/8] Installing Project...
git clone https://github.com/bonaktan/Flight-Management-System.git
if !errorlevel! neq 0 (
    echo [ERR] Failed to clone bonaktan/Flight-Management-System.git & pause & exit /b 1
)
cd Flight-Management-System/frontend
call npm install
if !errorlevel! neq 0 (
    pwd
    echo [ERR] Failed to install frontend Packages.
)
echo     [OK] Project Installed
:: ════════════════════════════════════════════════════════════
:: Done
:: ════════════════════════════════════════════════════════════

echo.
echo ============================================================
echo   Bootstrap complete!
echo ============================================================
echo   Installed:
echo     - MSVC v145 Build Tools       ^(C:\BuildTools^)
echo     - Windows 11 SDK 22621
echo     - CMake  ^(bundled with VS 2026 Build Tools^)
echo     - Built-in vcpkg component    ^(manifest mode + toolchain^)
echo     - External vcpkg              ^(C:\vcpkg, classic mode^)
echo     - Drogon + dependencies       ^(x64-windows^)
echo     - Node.js v%NODE_VERSION% LTS
echo.
echo.
echo   Next steps in VSCode:
echo     1. Install recommended extensions when prompted (C/C++, CMake)
echo     2. To Configure Backend: VSCode -^>Ctrl+Shift+P -^> CMake: Configure
echo     3. To Build Backend: VSCode -^>Ctrl+Shift+P -^> CMake: Build
echo     4. Run the binary
echo     5. To run Frontend in Dev Mode: cd frontend ^&^& npm run dev
echo ============================================================
echo.
pause
exit /b 1
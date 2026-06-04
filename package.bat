@echo off
REM Dong goi Ezsnagit (C++/Qt6 MinGW) thanh thu muc portable + (neu co Inno) installer .exe.
REM Dung sau khi build.bat da tao build\Ezsnagit.exe.
setlocal
set QT=C:\Qt\6.8.3\mingw_64
set MINGW=C:\Qt\Tools\mingw1310_64\bin
set PATH=%QT%\bin;%MINGW%;%PATH%
set ROOT=%~dp0
set DIST=%ROOT%dist\Ezsnagit

if not exist "%ROOT%build\Ezsnagit.exe" ( echo [LOI] Chua co build\Ezsnagit.exe - chay build.bat truoc & exit /b 1 )

echo [1/3] Don dist cu...
if exist "%DIST%" rmdir /s /q "%DIST%"
mkdir "%DIST%"

echo [2/3] Chep exe + windeployqt...
copy /y "%ROOT%build\Ezsnagit.exe" "%DIST%\Ezsnagit.exe" >nul
windeployqt --release --no-translations --compiler-runtime "%DIST%\Ezsnagit.exe"
if errorlevel 1 ( echo [LOI] windeployqt that bai & exit /b 1 )

echo [3/3] Bien dich installer (neu co Inno Setup)...
set ISCC=
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" set ISCC=C:\Program Files (x86)\Inno Setup 6\ISCC.exe
if exist "C:\Program Files\Inno Setup 6\ISCC.exe" set ISCC=C:\Program Files\Inno Setup 6\ISCC.exe
if exist "%LOCALAPPDATA%\Programs\Inno Setup 6\ISCC.exe" set ISCC=%LOCALAPPDATA%\Programs\Inno Setup 6\ISCC.exe
if "%ISCC%"=="" ( echo [BO QUA] Chua cai Inno Setup - chi co ban portable o "%DIST%". & goto done )
"%ISCC%" "%ROOT%installer.iss"
if errorlevel 1 ( echo [LOI] ISCC that bai & exit /b 1 )
echo [OK] Installer o "%ROOT%installer\".

:done
echo [XONG] Portable: "%DIST%"
endlocal

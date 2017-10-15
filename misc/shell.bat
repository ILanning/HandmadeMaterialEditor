@echo off

REM  To run this at startup, use this as your shortcut target:
REM  %windir%\system32\cmd.exe /k w:\handmade\misc\shell.bat

call "F:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
set path="%~dp0";%path%
set exe_path=%~dp0
set handmade_folder=%exe_path:~0,-5%
set exe_path=%handmade_folder%build\win32_handmade.exe
REM call devenv %exe_path%

echo Environment Variables:
echo ===========================
echo exe_path:        %exe_path%
echo handmade_folder: %handmade_folder%
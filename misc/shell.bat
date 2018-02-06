@echo off

REM  To run this at startup, use this as your shortcut target:
REM  %windir%\system32\cmd.exe /k w:\handmade\misc\shell.bat

REM call "F:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set path="%~dp0";%path%
set game_path=%~dp0
set handmade_folder=%game_path:~0,-5%
set game_path=%handmade_folder%build\win32_handmade.exe
set test_path=%handmade_folder%build\win32_testmain.exe
REM call devenv "%game_path%"

call help.bat

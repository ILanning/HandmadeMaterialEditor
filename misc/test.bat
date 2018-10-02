@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_ALLOW_NEW=1 -DHANDMADE_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref opengl32.lib "%handmade_folder%code\libraries\glew32.lib"

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

IF NOT EXIST glew32.dll xcopy ..\code\libraries\glew32.dll

REM 64-bit test build
echo.
echo Compiling Tests...
echo ====================
cl %CommonCompilerFlags% /EHsc ..\code\test\win32_testmain.cpp -Fmwin32_testmain.map /link %CommonLinkerFlags%

echo.
win32_testmain %test_suite_args% %*
popd

@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -FC -Z7 -EHsc
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib "%handmade_folder%code\libraries\glew32.lib"

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

IF NOT EXIST glew32.dll xcopy ..\code\libraries\glew32.dll

REM 64-bit test build
del *.pdb > NUL 2> NUL
REM cl %CommonCompilerFlags% ..\code\handmade.cpp -Fmhandmade.map -LD /link -incremental:no -opt:ref -PDB:handmade_%random%.pdb -EXPORT:GameGetSoundSamples -EXPORT:GameUpdateAndRender
echo(
echo Compiling
echo ====================
cl %CommonCompilerFlags% ..\code\test\win32_testmain.cpp -Fmwin32_testmain.map /link %CommonLinkerFlags%

echo(
win32_testmain %*
popd

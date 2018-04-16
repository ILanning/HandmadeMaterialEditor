@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_ALLOW_NEW=1 -DHANDMADE_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref opengl32.lib "%handmade_folder%code\libraries\glew32.lib"

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

IF NOT EXIST glew32.dll xcopy ..\code\libraries\glew32.dll

echo.
echo == Robocopying data TO build ==
robocopy ..\data ..\build /e /xo /njh /ns /nc /nfl /ndl

REM 32-bit build
REM cl %CommonCompilerFlags% ..\handmade\code\win32_handmade.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

REM 64-bit build
del *.pdb > NUL 2> NUL
cl %CommonCompilerFlags% ..\code\handmade.cpp -Fmhandmade.map -LD /link %CommonLinkerFlags% -PDB:handmade_%random%.pdb -EXPORT:GameInitialize -EXPORT:GameProcessInput -EXPORT:GameUpdateAndRender -EXPORT:GameGetSoundSamples
cl %CommonCompilerFlags% ..\code\win32\win32_handmade.cpp -Fmwin32_handmade.map /link %CommonLinkerFlags% -opt:ref user32.lib gdi32.lib winmm.lib -manifest:embed -manifestinput:"%handmade_folder%code\win32\win32_handmade.exe.manifest" 
popd
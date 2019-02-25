echo Compiling Game...
echo ==================================

IF NOT EXIST build mkdir build
pushd build

del *.pdb > NUL 2> NUL
cl %CommonCompilerFlags% ..\code\handmade_unityfile.cpp -Fmhandmade.map -LD /link %CommonLinkerFlags% -OUT:handmade.dll -PDB:handmade_%random%.pdb -EXPORT:GameInitialize -EXPORT:GameProcessInput -EXPORT:GameUpdateAndRender -EXPORT:GameGetSoundSamples
cl %CommonCompilerFlags% ..\code\win32\win32_handmade_unityfile.cpp -Fmwin32_handmade.map /link %CommonLinkerFlags% -OUT:win32_handmade.exe -opt:ref user32.lib gdi32.lib winmm.lib -manifest:embed -manifestinput:"%handmade_folder%code\win32\win32_handmade.exe.manifest" 

popd
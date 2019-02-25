echo Copying libraries and \data to \build...
echo ==================================

IF NOT EXIST build mkdir build
pushd build

IF NOT EXIST glew32.dll xcopy ..\code\libraries\glew32.dll

robocopy ..\data ..\build /e /xo /njh /ns /nc /nfl /ndl

popd
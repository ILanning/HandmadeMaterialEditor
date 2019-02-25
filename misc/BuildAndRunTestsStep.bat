echo Compiling Tests...
echo ==================================

IF NOT EXIST build mkdir build
pushd build

cl %CommonCompilerFlags% /EHsc ..\code\test\win32_testmain.cpp -Fmwin32_testmain.map /link %CommonLinkerFlags%

echo.
win32_testmain %test_suite_args% %*

popd
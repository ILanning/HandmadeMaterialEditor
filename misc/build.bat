@echo off

call SetUpVariablesStep.bat

pushd ..
echo Phase 1
call CopyToBuildStep.bat

echo.
echo Phase 2
call BuildGameStep.bat

echo.
echo Phase 3!
call BuildAndRunTestsStep.bat
popd

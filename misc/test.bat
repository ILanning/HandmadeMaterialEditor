@echo off

call SetUpVariablesStep.bat

pushd ..
call CopyToBuildStep.bat

call BuildAndRunTestsStep.bat
popd
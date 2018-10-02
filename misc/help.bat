@echo off

echo Environment Variables:
echo ===========================
echo %%game_path%%:        %game_path%
echo %%test_path%%:        %test_path%
echo %%handmade_folder%%:  %handmade_folder%
echo.
echo Commands:
echo ===========================
echo build:  Builds the program.  If done while the program is running, the dll will
echo         be rebuilt and reloaded by the program, allowing live edits.  This also
echo         builds and runs the test program.
echo.
echo test:   Builds and runs the doctest testing program.  Passes arguments on to
echo         the program.
echo.
echo help:   Prints this text to the console.
echo.
echo doxygen *.doxyfile:   Generates documentation for the program using Doxygen.
echo.
echo %%test_suite_args%%:
echo         Set this to automatically give the tests the same set of commands 
echo         every time they are run, including when run as part of build.
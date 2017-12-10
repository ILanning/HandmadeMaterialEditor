@echo off

echo Environment Variables:
echo ===========================
echo game_path:        %game_path%
echo test_path:        %test_path%
echo handmade_folder:  %handmade_folder%
echo.
echo Commands:
echo ===========================
echo build:  Builds the program.  If done while the program is running, the dll will
echo         be rebuilt and reloaded by the program, allowing live edits.
echo test:   Builds and runs the doctest testing program.  Passes arguments on to
echo         the program.
echo help:   Prints this text to the console.
echo doxygen *.doxyfile:   Generates documentation for the program using Doxygen.
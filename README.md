# Handmade Material Editor

A simple program to edit .obj model materials, built with a number of Handmade Hero principles.

#### Basics

This program was more meant as a personal challenge and crash course in OpenGL and a few interesting programming concepts (particularly ones related to game development), so it's built with a few quirks:

* Uses as few libraries as possible/practical, so no STL/STD/Boost
* Will avoid use of new and delete, to encourage experimentation with writing custom allocators
* Designed to allow hot reloading of the game code without interrupting what's going on in memory
* As a result, breaks vanilla C++ v-tables (but encourages a custom try at polymorphism)

#### Building

Builds are done with Visual C++. They have not been tested with any other compiler at this time.  
The build process is pretty simple, but it can require a little custom setup:

1. After downloading the code, navigate to the "misc" directory.
2. Open shell.bat in a text editor.
3. Edit the path for the call to vcvarsall.bat to match the path to that file on your own system.  It should be similar to one of the two paths already in shell.bat, depending on your Visual Studio version.

After that's been done once:

1. Double click the Command Prompt shortcut or run `shell` in a command window
2. run `build`
3. It should (attempt to) compile!

#### 
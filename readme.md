### Notice

this project is not suitable for you to commit now. also, not so friendly for you to run. some fundamental works haven't done yet. thanks for you watching this project. when I am done those works, let's make a complete 2D turn-based game together!!!

### RoadMaps

1. vscode debugger plugin for lua debugging
2. improve GenMake.sh
3. separate core game and engine
4. divide c and lua's work
5. add a gui-system

### Introduction

this project is intend to build a relative complete 2D game. using only C++ and OpenGL,fully source code built.

this project is using CMake as build system. the author is worked on both Windows and Mac. this repo has 3 projects. one is SimpleEngine, the second one is external and the third is debugger. this external is used to build libs which used by SimpleEngine and debugger.

external project contains third party libs. its CMakeLists.txt is in the external/  dir. including:

1. asio 	(for async network)
2. assimp 	(doesn't use now)
3. freetype (font lib)
4. glew,glm,glfw	(for OpenGL)
5. imgui	(for debugging)
6. SOIL		(image file read)
7. nlohmann	(json lib)
8. lua53,luasocket,lua-cjson,mobdebug

debugger project is not complete now. its CMakeLists.txt is in the debugger/  dir.

SimpleEngine is our main project. its CMakeLists.txt in the repo root dir(SimpleEngine/).

### How to build

1. install git and cmake
		
		git clone --recursive https://github.com/oceancx/SimpleEngine

2. build external project. 
using cmake.exe open external/CMakeLists,set the generate project folder as external/build. then always using the default setting, until Xcode project or visual studio project was generated. using the IDE open the project, and build INSTALL.(which will automaticly copy libs and include files to external/libs and external/include )

3. build SimpleEngine project.
first, in SimpleEngine/, sh GenMake.sh. then using cmake.exe open SimpleEngine/CMakeLists, set the generate project folder as SimpleEngine/build. always using the default setting, until Xcode project or visual studio project was generated.using the IDE open the project, and build SimpleEngine.
	
4. download resource. [baidu yun disk](https://pan.baidu.com/s/1o7Gn0t8), unzip to SimpleEngine/Data.

5. run SimpleEngine.

### Program Entrance

program.cpp -> Window.cpp -> Game.cpp -> Engine.cpp -> SceneManager.cpp -> Scene.cpp

### Extra Resource

https://pan.baidu.com/s/1kV9qrEB

### Glance

<img src="https://github.com/oceancx/SimpleEngine/blob/master/1.gif" width="50%" height="50%">

<img src="https://github.com/oceancx/SimpleEngine/blob/master/2.gif" width="50%" height="50%">


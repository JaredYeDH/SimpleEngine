# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (SimpleEngine)


# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
Message( " " )
Message( FATAL_ERROR "Source and build directories are the same.
Create an empty build directory,
change into it and re-invoke cmake")
endif() 

# adding glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(external/glew-cmake)
include_directories(external/glew-cmake/include external/glew-cmake/src Astar)

# adding glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Do not build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Do not build the GLFW tests programs" FORCE)
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
add_subdirectory(external/glfw)
include_directories(SYSTEM external/glfw/include)

# adding glm
include_directories(SYSTEM external/glm)

# adding asio
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_ATOMIC)
add_definitions(-DASIO_HAS_STD_SHARED_PTR) 
add_definitions(-DASIO_HAS_STD_FUNCTION) 
add_definitions(-DASIO_HAS_STD_ADDRESSOF) 
add_definitions(-DASIO_HAS_STD_ARRAY) 
add_definitions(-DASIO_HAS_VARIADIC_TEMPLATES)
add_definitions(-DASIO_HAS_STD_CHRONO)
add_definitions(-DASIO_HAS_CSTDINT)
add_definitions(-DASIO_HAS_STD_TYPE_TRAITS)
include_directories(SYSTEM external/asio/asio/include)

# adding imgui
include_directories(SYSTEM external/imgui)

# adding assimp
#set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build the tools" FORCE)
#set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build the tests" FORCE)
#add_subdirectory(external/assimp)
#include_directories(SYSTEM external/assimp/include)

# adding SOIL
add_subdirectory(external/SOIL)
include_directories(SYSTEM external/SOIL/include)

if(WIN32)
	find_package(OpenGL REQUIRED)
	#set(LIBS glfw opengl32 libglew_static SOIL_Static assimp)
	set(LIBS glfw opengl32 libglew_static SOIL_Static)
	list(APPEND LIBS ${OPENGL_LIBRARY}) 
elseif(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  # Linux packages native to CMake
  set(LIBS glfw)
  list(APPEND LIBS libglew_static)
  list(APPEND LIBS SOIL_Static)
 #list(APPEND LIBS assimp)
  find_package(OpenGL REQUIRED)
  list(APPEND LIBS ${OPENGL_gl_LIBRARY}) # setting LIBS for the first time
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  list(APPEND LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xinerama_LIB} ${X11_Xi_LIB} ${X11_Xcursor_LIB})
  find_package(Threads REQUIRED)
  list(APPEND LIBS ${CMAKE_THREAD_LIBS_INIT})
  #find_library(RT_LIB rt)
  #list(APPEND LIBS ${RT_LIB})
  # append non-native packages
else()
  set(LIBS )
endif(WIN32)

# Because we use glfw3 we need to link the application with Cocoa
IF(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(Cocoa_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(Cocoa_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${Cocoa_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  set(LIBS ${LIBS} ${APPLE_LIBS})
ENDIF(APPLE)



# clang && macosx
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
# c++11
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# clang && debug adds address sanitizer
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND NOT APPLE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()



set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)


add_definitions(
	-std=c++11
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


include_directories(
	.
)



add_executable(${PROJECT_NAME}
ThirdParty/NetEase/MAP.cpp
ThirdParty/NetEase/MAP.h
ThirdParty/NetEase/Sprite2.h
ThirdParty/NetEase/Sprite2.cpp
ThirdParty/NetEase/WAS.h
ThirdParty/NetEase/WAS.cpp
ThirdParty/NetEase/WDF.cpp
ThirdParty/NetEase/WDF.h
program.cpp
Environment.h
#main.cpp
image.h
global.h
defines.h
Objects/Object.h
Objects/Object.inl
Objects/Object.cpp
Objects/MapObject.h
Objects/MapObject.cpp
Components/BaseComponent.h
Components/BaseComponent.cpp
Components/TransformComponent.h
Components/TransformComponent.cpp
Network/Client.cpp
Network/Client.h
Network/Message.cpp
Network/Message.h
Projects/TestSence.cpp
Projects/TestSence.h
Projects/Demo.h
Projects/Demo.cpp
Projects/Player.h
Projects/Player.cpp
Projects/Astar.h
Projects/Astar.cpp
Projects/GameMap.cpp
Projects/GameMap.h
Projects/Config.h
Projects/config.txt
Projects/Config.cpp
Projects/AlphaSence.cpp
Projects/AlphaSence.h
InputManager.h
InputManager.cpp
Window.h
Window.cpp
Random.cpp
Random.h
TextureManager.h
TextureManager.cpp
Texture.h
Texture.cpp
Sprite.h
Sprite.cpp
SpriteBatch.h
SpriteBatch.cpp
SpriteRenderer.h
SpriteRenderer.cpp
ResourceManager.h
ResourceManager.cpp
CubeBatch.h
CubeBatch.cpp
Singleton.h
Singleton.inl
Shader.h
Shader.cpp
SenceManager.h
SenceManager.cpp
Game.h
Game.cpp
Engine.h
Engine.cpp
Event/Event.h
Shader/SimpleVertexShader.vert
Shader/SimpleFragmentShader.frag
Shader/MvpFragmentShader
Shader/MvpVertexShader
IUpdateDraw.h
GMath.h
GMath.cpp
Pos.h
FrameAnimation.h
FrameAnimation.cpp
external/imgui/imgui.cpp
external/imgui/imgui.h
external/imgui/imgui_demo.cpp
external/imgui/imgui_draw.cpp
external/imgui/imgui_internal.h
external/imgui/imconfig.h 
external/imgui/stb_rect_pack.h
external/imgui/stb_textedit.h
external/imgui/stb_truetype.h
imgui_impl_glfw_gl3.h
imgui_impl_glfw_gl3.cpp
Astar/include/Node.h
Astar/include/InputHandler.h
Astar/include/SquareGraph.h
Astar/Node.cpp
Astar/InputHandler.cpp
Astar/SquareGraph.cpp
)

target_link_libraries(${PROJECT_NAME}
	${LIBS}
)


set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Xcode and Visual working directories

#create_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")
#create_default_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")
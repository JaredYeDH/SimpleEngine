# CMake entry point
cmake_minimum_required (VERSION 3.1)

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
Message( " " )
Message( FATAL_ERROR "Source and build directories are the same.
Create an empty build directory,
change into it and re-invoke cmake")
endif() 


find_library(LUA_LIBS lua  PATHS ${PROJECT_SOURCE_DIR}/external/lib/lua)

find_library(LUA_SOCKET_LIBS luasocket  PATHS ${PROJECT_SOURCE_DIR}/external/lib/lua)

find_package(BZip2 QUIET REQUIRED)
find_package(PNG QUIET REQUIRED)

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
  ./Astar
  ../external/include/freetype2
  ../external/include/lua
  ../external/include
)


link_directories(
  ../external/lib
)




set(COMMON_LIBS 
  glfw3
  glewd 
  SOIL
  assimp
  freetyped
  ${LUA_LIBS}
  ${LUA_SOCKET_LIBS}
)

if(WIN32)
	find_package(OpenGL REQUIRED)
  list(APPEND LIBS opengl32) 
  list(APPEND LIBS ${COMMON_LIBS}) 
	list(APPEND LIBS ${OPENGL_LIBRARY}) 
elseif(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  list(APPEND LIBS ${OPENGL_gl_LIBRARY})
  list(APPEND LIBS ${COMMON_LIBS}) 
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  list(APPEND LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xinerama_LIB} ${X11_Xi_LIB} ${X11_Xcursor_LIB})
  find_package(Threads REQUIRED)
  list(APPEND LIBS ${CMAKE_THREAD_LIBS_INIT})
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


set(SRC_CPP_FILES 
Actor.cpp
Actor.h
Astar/include/InputHandler.h
Astar/include/Node.h
Astar/include/SquareGraph.h
Astar/InputHandler.cpp
Astar/Node.cpp
Astar/SquareGraph.cpp
Components/BaseComponent.cpp
Components/BaseComponent.h
Components/TransformComponent.cpp
Components/TransformComponent.h
core/Graphics2D.cpp
core/Graphics2D.h
core/Object2D.cpp
core/Object2D.h
core/Renderer.cpp
core/Renderer.h
core/Shader.cpp
core/Shader.h
core/Texture.cpp
core/Texture.h
core/Transform.cpp
core/Transform.h
CubeBatch.cpp
CubeBatch.h
defines.h
defineTypes.h
Engine.cpp
Engine.h
Entity.cpp
Entity.h
Environment.cpp
Environment.h
Event/Event.h
FrameAnimation.cpp
FrameAnimation.h
Game.cpp
Game.h
global.h
GMath.cpp
GMath.h
image.h
ImageLoader.cpp
ImageLoader.h
imgui_impl_glfw_gl3.cpp
imgui_impl_glfw_gl3.h
InputManager.cpp
InputManager.h
IUpdateDraw.h
Logger.cpp
Logger.h
LuaVM.cpp
LuaVM.h
Network/Client.cpp
Network/Client.h
Network/Message.cpp
Network/Message.h
Objects/MapObject.cpp
Objects/MapObject.h
Objects/Object.cpp
Objects/Object.h
Objects/Object.inl
Pos.h
precompile.h
program.cpp
Projects/AlphaSence.cpp
Projects/AlphaSence.h
Projects/Astar.cpp
Projects/Astar.h
Projects/Config.cpp
Projects/Config.h
Projects/Demo.cpp
Projects/Demo.h
Projects/GameMap.cpp
Projects/GameMap.h
Projects/Heap.h
Projects/mhxy.h
Projects/Player.cpp
Projects/Player.h
Projects/TestLua.cpp
Projects/TestSence.cpp
Projects/TestSence.h
Random.cpp
Random.h
Resource.cpp
Resource.h
ResourceManager.cpp
ResourceManager.h
SenceManager.cpp
SenceManager.h
Sences/BaseSence.cpp
Sences/BaseSence.h
Singleton.h
Singleton.inl
Sprite.cpp
Sprite.h
SpriteBatch.cpp
SpriteBatch.h
SpriteRenderer.cpp
SpriteRenderer.h
TextRenderer.cpp
TextRenderer.h
TextureManager.cpp
TextureManager.h
ThirdParty/NetEase/MAP.cpp
ThirdParty/NetEase/MAP.h
ThirdParty/NetEase/Sprite2.cpp
ThirdParty/NetEase/Sprite2.h
ThirdParty/NetEase/WAS.cpp
ThirdParty/NetEase/WAS.h
ThirdParty/NetEase/WDF.cpp
ThirdParty/NetEase/WDF.h
Time/Time.cpp
Time/Time.h
Time/TimeManager.cpp
Time/TimeManager.h
Time/Timer.cpp
Time/Timer.h
Time/TimerManager.cpp
Time/TimerManager.h
Window.cpp
Window.h

)

add_executable(${PROJECT_NAME}
${SRC_CPP_FILES}
${IMGUI_SRCS}
)


if (BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIR}) 
  list(APPEND LIBS ${BZIP2_LIBRARIES} ) 
endif ()

if (PNG_FOUND)
  add_definitions(${PNG_DEFINITIONS})
  include_directories(${PNG_INCLUDE_DIRS})
  list(APPEND LIBS ${PNG_LIBRARIES}) 
endif ()

target_link_libraries(${PROJECT_NAME}
  ${LIBS}
)




set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Xcode and Visual working directories

#create_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")
#create_default_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")

# CMake entry point
cmake_minimum_required (VERSION 3.1)

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
Message( " " )
Message( FATAL_ERROR "Source and build directories are the same.
Create an empty build directory,
change into it and re-invoke cmake")
endif() 


find_library(LUA_STATIC lua_static  PATHS ${PROJECT_SOURCE_DIR}/external/lib/lua)
find_library(LUA_SOCKET luasocket  PATHS ${PROJECT_SOURCE_DIR}/external/lib/lua)


if(APPLE)
  find_package(BZip2 QUIET REQUIRED)
  find_package(PNG QUIET REQUIRED)
endif(APPLE)

add_definitions(
  -std=c++11
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
  -D_WIN32_WINNT=0x0501
)

if(WIN32)
	add_definitions(-DZLIB_WINAPI)
endif(WIN32)

include_directories(
  .
  ./Astar
  ../external/include/freetype2
  ../external/include/lua
  ../external/NESupport/src
  ../external/include
)


link_directories(
  ../external/lib
)

if(WIN32)
  set(ASSIMP_LIB assimp-vc140-mt)
elseif(UNIX)
  set(ASSIMP_LIB assimp)
endif(WIN32)

set(FREETYPE_LIB freetyped)

set(COMMON_LIBS 
  glfw3
  glewd 
  SOIL
  #NESupport
  ${ASSIMP_LIB}
  ${FREETYPE_LIB}
  ${LUA_STATIC}
  ${LUA_SOCKET}
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
actor/Actor.cpp
actor/Actor.h
actor/ActorManager.cpp
actor/ActorManager.h
actor/Player.cpp
actor/Player.h
actor/PlayerState.cpp
actor/PlayerState.h
algorithm.cpp
animation/animation_database.h
animation/FrameAnimation.cpp
animation/FrameAnimation.h
astar/Astar.cpp
astar/Astar.h
astar/Heap.h
astar/include/InputHandler.h
astar/include/Node.h
astar/include/SquareGraph.h
astar/InputHandler.cpp
astar/Node.cpp
astar/SquareGraph.cpp
Bitmap.cpp
Bitmap.h
combat/Combat.cpp
combat/Combat.h
combat/Skill.cpp
combat/Skill.h
components/BaseComponent.cpp
components/BaseComponent.h
components/TransformComponent.cpp
components/TransformComponent.h
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
editor/resource_viewer.cpp
editor/resource_viewer.h
Engine.cpp
Engine.h
Entity.cpp
Entity.h
Environment.cpp
Environment.h
event/Event.h
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
Message.cpp
Message.h
network/Client.cpp
network/Client.h
network/Message.cpp
network/Message.h
objects/MapObject.cpp
objects/MapObject.h
objects/Object.cpp
objects/Object.h
objects/Object.inl
Pos.h
precompile.h
program.cpp
Random.cpp
Random.h
Resource.cpp
Resource.h
ResourceManager.cpp
ResourceManager.h
scene/BaseScene.cpp
scene/BaseScene.h
scene/GameMap.cpp
scene/GameMap.h
scene/Scene.cpp
scene/Scene.h
scene/SceneManager.cpp
scene/SceneManager.h
scene/SplashScene.cpp
scene/SplashScene.h
scene/UIScene.cpp
scene/UIScene.h
scene/WASViewerScene.cpp
scene/WASViewerScene.h
simple_engine.h
Singleton.h
Singleton.inl
SpriteBatch.cpp
SpriteBatch.h
SpriteRenderer.cpp
SpriteRenderer.h
State.cpp
State.h
TextRenderer.cpp
TextRenderer.h
TextureManager.cpp
TextureManager.h
time/Time.cpp
time/Time.h
time/TimeManager.cpp
time/TimeManager.h
time/Timer.cpp
time/Timer.h
time/TimerManager.cpp
time/TimerManager.h
tsv.h
Window.cpp
Window.h
)

set(SRC_EXTERNAL_CPP_FILES
../external/NESupport/src/NESupport.cpp
)

add_executable(${PROJECT_NAME}
${SRC_CPP_FILES}
${IMGUI_SRCS}
${SRC_EXTERNAL_CPP_FILES}
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

if (GZIP_FOUND)
  add_definitions(${GZIP_DEFINITIONS})
  include_directories(${GZIP_INCLUDE_DIRS})
  list(APPEND LIBS ${GZIP_LIBRARIES}) 
endif ()


target_link_libraries(${PROJECT_NAME}
  ${LIBS}
)




set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Xcode and Visual working directories

#create_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")
#create_default_target_launcher(${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/")

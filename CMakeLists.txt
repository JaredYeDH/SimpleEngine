# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (SimpleEngine)


SET(WORK_DIR  "${PROJECT_SOURCE_DIR}")  

configure_file (  
  "${PROJECT_SOURCE_DIR}/precompile.h.in"  
  "${PROJECT_SOURCE_DIR}/src/precompile.h"  
  )  


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

set(IMGUI_SRCS 
../external/imgui/imgui.cpp
../external/imgui/imgui.h
../external/imgui/imgui_demo.cpp
../external/imgui/imgui_draw.cpp
../external/imgui/imgui_internal.h
../external/imgui/imconfig.h 
../external/imgui/stb_rect_pack.h
../external/imgui/stb_textedit.h
../external/imgui/stb_truetype.h
)

# adding assimp
#set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build the tools" FORCE)
#set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build the tests" FORCE)
#add_subdirectory(external/assimp)
#include_directories(SYSTEM external/assimp/include)


# adding freetype
add_subdirectory(external/freetype)
include_directories(SYSTEM external/freetype/include)

# adding SOIL
add_subdirectory(external/SOIL)
include_directories(SYSTEM external/SOIL/include)

# adding Lua
include_directories(external/include/lua)


add_subdirectory(src)


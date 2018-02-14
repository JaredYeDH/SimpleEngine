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



add_subdirectory(src)



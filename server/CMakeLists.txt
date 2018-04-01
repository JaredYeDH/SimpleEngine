# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (server)

SET(WORK_DIR  "${PROJECT_SOURCE_DIR}")  
configure_file (  
  "${PROJECT_SOURCE_DIR}/src/defines.h.in"  
  "${PROJECT_SOURCE_DIR}/src/defines.h"  
  )  

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
include_directories(../external/asio/asio/include)

include_directories(
	.
	${PROJECT_SOURCE_DIR}/../external/include/lua
	${PROJECT_SOURCE_DIR}/../external/include
)

add_definitions(
  -std=c++11
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
)

add_subdirectory(src)

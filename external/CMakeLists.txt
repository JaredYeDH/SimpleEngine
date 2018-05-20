# Copyright (C) 2007-2013 LuaDist.
# Created by Peter Drahoš, David Manura
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project ( externals  )
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} )

cmake_minimum_required ( VERSION 2.8 )


# adding glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(glew-cmake)


# adding glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Do not build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Do not build the GLFW tests programs" FORCE)
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
add_subdirectory(glfw)


# adding glm
add_subdirectory(glm)


# adding assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build the tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build the tests" FORCE)
add_subdirectory(assimp)

# adding freetype
add_subdirectory(freetype)


# adding SOIL
add_subdirectory(SOIL)


#adding luadist
add_subdirectory(luadist)

#adding NESupport
add_subdirectory(NESupport)


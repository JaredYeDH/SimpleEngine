# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (server)



include_directories(
	.
	${PROJECT_SOURCE_DIR}/../external/include/lua
)


find_library(LUA_LIBS lua  PATHS ${PROJECT_SOURCE_DIR}/../external/lib/lua)
find_library(LUA_SOCKET_LIBS luasocket  PATHS ${PROJECT_SOURCE_DIR}/../external/lib/lua)

add_executable(${PROJECT_NAME}
main.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${LUA_LIBS} ${LUA_SOCKET_LIBS}
)

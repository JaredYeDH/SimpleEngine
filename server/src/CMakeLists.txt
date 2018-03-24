find_library(LUA_STATIC lua_static  PATHS ${PROJECT_SOURCE_DIR}/../external/lib/lua)
find_library(LUA_SOCKET luasocket  PATHS ${PROJECT_SOURCE_DIR}/../external/lib/lua)

set(SRC_CPP_FILES 
connection.cpp
connection.hpp
connection_manager.cpp
connection_manager.hpp
DebugServer.cpp
DebugServer.h
defines.h
header.hpp
main.cpp
mime_types.cpp
mime_types.hpp
reply.cpp
reply.hpp
request.hpp
request_handler.cpp
request_handler.hpp
request_parser.cpp
request_parser.hpp
server.cpp
server.hpp
test_asio.cpp
test_asio.h
)

add_executable(${PROJECT_NAME}
${SRC_CPP_FILES}
)

if(WIN32)
set(LUA_SOCKET_LINK_LIB ws2_32)
endif(WIN32)

target_link_libraries(${PROJECT_NAME}
${LUA_SOCKET} ${LUA_STATIC}  ${LUA_SOCKET_LINK_LIB}
)

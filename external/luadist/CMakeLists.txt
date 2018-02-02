# Copyright (C) 2007-2013 LuaDist.
# Created by Peter Drahoš, David Manura
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

cmake_minimum_required ( VERSION 2.8 )


configure_file ( lua/src/luaconf.h.orig ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h)

#################luasocket start################
# The standard luasocket build has this enabled in release mode.
# It is required for running the test suite.
add_definitions ( -DLUASOCKET_DEBUG )
if ( MINGW )
  add_definitions ( -DLUASOCKET_INET_PTON -D_WIN32_WINNT=0x501 )
endif ( )

# Shared library exporting.
if ( WIN32 )
  add_definitions ( "-DMIME_API=__declspec(dllexport)" "-DLUASOCKET_API=__declspec(dllexport)" )
endif ( )

if ( WIN32 AND NOT CYGWIN )
  set ( LUA_SOCKETC luasocket/src/wsocket.c )
else ( )
  set ( LUA_SOCKETC luasocket/src/usocket.c )
endif ( )

set ( LUA_SRC_SOCKET luasocket/src/luasocket.c luasocket/src/timeout.c luasocket/src/buffer.c luasocket/src/io.c luasocket/src/auxiliar.c 
  luasocket/src/options.c luasocket/src/inet.c luasocket/src/tcp.c luasocket/src/udp.c luasocket/src/except.c luasocket/src/select.c ${LUA_SOCKETC} )

set ( LUA_HEADER_SOCKET luasocket/src/luasocket.h luasocket/src/timeout.h luasocket/src/buffer.h luasocket/src/io.h luasocket/src/auxiliar.h 
  luasocket/src/options.h luasocket/src/inet.h luasocket/src/tcp.h luasocket/src/udp.h luasocket/src/except.h luasocket/src/select.h ${LUA_SOCKETC} )

set ( LUA_SRC_UNIX_SOCKET luasocket/src/unix.c luasocket/src/io.c luasocket/src/auxiliar.c luasocket/src/timeout.c luasocket/src/options.c
  luasocket/src/buffer.c ${LUA_SOCKETC} )

set ( LUA_HEADER_UNIX_SOCKET luasocket/src/unix.h luasocket/src/io.h luasocket/src/auxiliar.h luasocket/src/timeout.h luasocket/src/options.h
  luasocket/src/buffer.h  )

if ( WIN32 AND NOT CYGWIN )
  set ( LUA_LIB_SOCKET ws2_32 )
endif ()

# Solaris needs specific links
if ( CMAKE_SYSTEM_NAME STREQUAL "SunOS" )
  set ( LUA_LIB_SOCKET socket resolv rt )
endif ( )


include_directories ( luasocket/src )


add_library ( luasocket   ${LUA_SRC_SOCKET}  ${LUA_LIB_SOCKET} )
target_link_libraries ( luasocket ${LUA_LIB_SOCKET} )
install (FILES ${LUA_HEADER_SOCKET} DESTINATION include/lua)
install (TARGETS luasocket DESTINATION lib/lua)




#################luasocket end################


#################lua start################


add_definitions(-DLUA_PATH=${CMAKE_INSTALL_PREFIX}/lib/lua;;)
include_directories ( lua/src ${CMAKE_CURRENT_BINARY_DIR})

include_directories ( luasocket/src )



set ( LUA_SRC_CORE lua/src/lapi.c lua/src/lcode.c lua/src/lctype.c lua/src/ldebug.c lua/src/ldo.c lua/src/ldump.c 
  lua/src/lfunc.c lua/src/lgc.c lua/src/llex.c lua/src/lmem.c lua/src/lobject.c lua/src/lopcodes.c lua/src/lparser.c 
  lua/src/lstate.c lua/src/lstring.c lua/src/ltable.c lua/src/ltm.c lua/src/lundump.c lua/src/lvm.c lua/src/lzio.c )
set ( LUA_SRC_LIB lua/src/lauxlib.c lua/src/lbaselib.c lua/src/lbitlib.c lua/src/lcorolib.c lua/src/ldblib.c 
  lua/src/liolib.c lua/src/lmathlib.c lua/src/loslib.c lua/src/lstrlib.c lua/src/ltablib.c lua/src/linit.c )

set ( LUA_SRC_LUA lua/src/lua.c )
set ( LUA_SRC_LUAC lua/src/luac.c  )

if ( LUA_USE_RELATIVE_LOADLIB )
  # Use modified loadlib
  list ( APPEND LUA_SRC_LIB lua/src/loadlib_rel.c )
else ( )
  list ( APPEND LUA_SRC_LIB lua/src/loadlib.c )
endif ( )

add_library ( lua ${LUA_SRC_CORE}  ${LUA_SRC_LIB} )
target_link_libraries ( lua  luasocket)

install ( FILES lua/src/lua.h lua/src/lualib.h lua/src/lauxlib.h lua/src/lua.hpp ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h DESTINATION include/lua)
install ( TARGETS lua  DESTINATION lib/lua)

#################lua end################



#add_executable ( client_test  client_test.cpp)
#target_link_libraries ( client_test liblua )
#install (TARGETS client_test DESTINATION bin)



#add_executable (  server_test server_test.cpp)
#target_link_libraries ( server_test liblua )
#install (TARGETS server_test DESTINATION bin)


	


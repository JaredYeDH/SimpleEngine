#pragma once
#include <stdint.h>
   
#define WORK_DIR "D:/Github/SimpleEngine/test_client"
   

typedef int64_t 	int64;
typedef int32_t 	int32;
typedef int16_t 	int16;
typedef int8_t 		int8;

typedef uint64_t	uint64;
typedef uint32_t 	uint32;
typedef uint16_t 	uint16;
typedef uint8_t 	uint8;

typedef float		float32;
typedef double		float64;
typedef long double float128;

typedef uint8_t   byte;

#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <memory>
#include <utility>

#include <vector>
#include <list>
#include <map>
#include <deque>
#include <set>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


template <typename T>
inline void SafeDelete(T * &pointer)
{
	if(pointer != nullptr)
	{
		delete pointer;
		pointer = nullptr;
	}
}



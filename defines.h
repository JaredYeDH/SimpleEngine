#pragma once
#include <stdint.h>

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

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

template <typename T>
inline void SafeDelete(T * &pointer)
{
	if(pointer != nullptr)
	{
		delete pointer;
		pointer = nullptr;
	}
}



#pragma pack(push) 
#pragma pack(1)   
struct TGA_FILE_HEADER
{
	uint8 IdLength;
	uint8 ColorMapType;
	uint8 ImageType;
	uint16 ColorMapFirstIndex;
	uint16 ColorMapLength;
	uint8 ColorMapEntrySize;
	uint16 XOrigin;
	uint16 YOrigin;
	uint16 ImageWidth;
	uint16 ImageHeight;
	uint8 PixelDepth;
	uint8 ImageDescruptor;
};
#pragma pack(pop)

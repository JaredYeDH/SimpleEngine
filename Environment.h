#pragma once

#include "defines.h"

#ifdef _WIN32
#define WORK_DIR "F:/Github/SimpleEngine"
#else
#define WORK_DIR "/Users/oceancx/MHXY/SimpleEngine"
#endif


class Environment
{
public:
	Environment();
	~Environment();
	
	static std::string GetPath()
	{
		return WORK_DIR;
	}

	static std::string GetAbsPath(std::string localPath)
	{
		return GetPath()+"/"+localPath;
	}

    static std::string GetAssetsPath(std::string path)
    {
        return GetPath()+"/Resource/Assets/"+path;
    }

};

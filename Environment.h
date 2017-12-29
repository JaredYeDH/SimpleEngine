#pragma once

#include "defines.h"
#include "precompile.h"

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

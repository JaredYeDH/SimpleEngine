#pragma once

#include "defines.h"
#include "defineTypes.h"
#include "precompile.h"

#ifdef WIN32
#define LINE_SAPARATOR '\r\n'
#else
#define LINE_SAPARATOR '\n'
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

	static String GetTsvPath(String name)
	{
		return GetPath()+"/Resource/tables/"+name+".tsv";
	}

	static std::string GetAbsPath(std::string localPath)
	{
		return GetPath()+"/"+localPath;
	}

    static std::string GetAssetsPath(std::string path)
    {
        return GetPath()+"/Resource/Assets/"+path;
    }
	
	static std::string GetShaderPath(std::string path)
	{
		return GetPath() + "/Shader/" + path;
	}
	static std::string GetLuaPath(std::string path)
	{
		return GetPath() + "/scripts/client/" + path;
	}

};

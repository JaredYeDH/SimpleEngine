#pragma once

#include "defines.h"
#include "defineTypes.h"
#include "precompile.h"

#define LINE_SAPARATOR '\n'
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
	static std::string GetWDFPath(std::string path)
	{
		return GetPath() + "/Data/" + path;
	}
	static std::string GetMapPath(std::string path)
	{
		return GetPath() + "/Data/scene/" + path +".map";
	}
};

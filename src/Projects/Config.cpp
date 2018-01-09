#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Heap.h"
#include "mhxy.h"
#include "../Environment.h"
using namespace std;

Config::Config(std::string filename)
{
	fstream infile(filename);
	std::string line;
	int line_count = 0;
	while (getline(infile, line))
	{
		if (line[0] == '#')break;
		if (line_count == 0)
		{
			mRootDir = Environment::GetAbsPath(line);
			line_count++;
		}
		else
		{
			istringstream is(line);
			std::string key;
			is >> key;
			string param;
			while (is >> param) {
				mDict[key].push_back(param);
				std::cout << "param: "<< param <<std::endl;
			}
		}
	}

}
void Config::Print()
{
	std::map<std::string, std::vector<string>>::iterator
		map_it = mDict.begin();
	while (map_it != mDict.end())
	{
		printf("key: %s", (*map_it).first.c_str());
		for (int i = 0; i< (*map_it).second.size(); i++)
		{
			printf(" %s", (*map_it).second[i].c_str());
		}
		printf("\n");
		map_it++;
	}
}
Config::~Config()
{
	mDict.clear();
}

std::string Config::GetWdfPath(std::string wdfFilename)
{
	return mRootDir + "/" + wdfFilename;
}


std::string Config::GetMapPath(std::string mapFilename)
{
	return mRootDir + "/scene/" + mapFilename;
}


std::string Config::GetFilePath(std::string relativePath)
{
	return mRootDir + relativePath;
}


// int main()
// {
// 	Config config("config.txt");
// 	config.Print();
// 	Heap<Pos> a;
// }Config.cpp
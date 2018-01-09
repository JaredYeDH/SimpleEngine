#ifndef CONFIG_H
#define CONFIG_H 
#include <vector> 
#include <string>
#include <map>
template <typename >class Heap;
class Config
{

public:
	Config(std::string filename);
	~Config();
	std::string mRootDir;
	std::string GetWdfPath(std::string wdfFilename);
	std::string GetMapPath(std::string mapFilename);
	std::string GetFilePath(std::string relativePath);
	void Print();
	std::map<std::string, std::vector<std::string>> mDict;
};
#endif
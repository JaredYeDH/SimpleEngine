# C++/OpenGL 梦幻西游

个人学习练手项目

### Mac编译指南

1. 安装好git和cmake,然后敲命令
		
		git clone --recursive https://github.com/oceancx/SimpleEngine

2. 编译之前将Environment.h中WORK_DIR宏设置为SimpleEngine的路径，例如：
		
		#define WORK_DIR "/Users/oceancx/SimpleEngine"

3. 下载资源压缩包：[百度云盘](https://pan.baidu.com/s/1o7Gn0t8)，直接在SimpleEngine/下解压即可,你会看到Data/文件夹被创建


### 相关资料
未整理，不过里面东西很多。https://pan.baidu.com/s/1kV9qrEB
### 程序入口

1. main函数在program.cpp中
2. Window负责初始化Game，Game负责初始化Engine，Engine负责初始化Sence，Sence就是游戏的内容了。
3. 此Repo暂时还不稳定，稳定之后会发布相关文档，包括资源文件的id对应表，以及显示资源文件的工具，还有资源读取的相关源码，还有各种参考资料，汗。。。。


### 注意

项目近期改动比较频繁，对大家协同开发不是很友好，等版本稳定后，再请大家多多关注哈。

目前打算做：

1. 整理好项目目录，分别整合成server client sharelib 三个project 
2. 写好VsCode Debugger插件
3. 完善简单的GUI库

### 项目核心价值观

1. 做一个完整的MMORPG游戏，包括客户端，服务端，引擎，UI等等。
2. C++/Lua为主要语言，C++底层，Lua逻辑。OpenGL渲染。
3. 依赖的库全部要能源码构建，每一个依赖库为一个cmake target。可以一键编译安装所有库（.h 和 .lib)
4. 纯手工打造MMO的运动系统和战斗系统。


### 截图

<img src="https://github.com/oceancx/SimpleEngine/blob/develop/ScreenShots/screenshot1.png" width="50%" height="50%">

<img src="https://github.com/oceancx/SimpleEngine/blob/develop/ScreenShots/screenshot2.png" width="50%" height="50%">



# C++/OpenGL Project Intro

 Fantasy Westward Journey, individual learning and training project.

### Mac build


1. install cmake and git,then:
		
		git clone --recursive https://github.com/oceancx/SimpleEngine

2. before build,in file "Environment.h",set WORK_DIR as SimpleEngine's path，for instance：

		#define WORK_DIR "/Users/oceancx/SimpleEngine"

3. download resource zip file ：[baidu pan](https://pan.baidu.com/s/1o7Gn0t8) , and extract to SimpleEngine/ , you will see a new dir Data/ was created.

4. then,happy to build:

		cd SimpleEngine/
		mkdir build
		cd build
		cmake ..
		make -j8
		./SimpleEngine

### Windows Build

Use CMake to generate Visual Studio Project file, Open it by VS and build.

### ScreenShots

<img src="https://github.com/oceancx/SimpleEngine/blob/develop/ScreenShots/screenshot1.png" width="50%" height="50%">

<img src="https://github.com/oceancx/SimpleEngine/blob/develop/ScreenShots/screenshot2.png" width="50%" height="50%">

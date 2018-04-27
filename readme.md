# 近期目标

由于项目还不稳定，暂时不会有文档。
目前正在加紧开发lua debugger。
战斗也开了个头。
接下来打算：
1. 开发完luaDebugger
2. 完善战斗系统
3. 完善简易GUI

以上搞定的话，就会抓紧时间写文档开源了。

# C++/OpenGL 梦幻西游

个人学习练手项目

### 编译指南

1. 安装好git和cmake,然后敲命令
		
		git clone --recursive https://github.com/oceancx/SimpleEngine

2. 用cmake打开external里面的CMakeLists.txt来编译库，一路默认生成项目工程，然后目标选INSTALL进行构建。成功后external下会多出include/和lib/文件夹

3. 用cmake打开SimpleEngine下的CMakeLists.txt来构建目标程序，一路默认生成项目工程，然后目标选SimpleEngine进行构建，成功后会生成可执行程序SimpleEngine。
	

4. 下载资源压缩包：[百度云盘](https://pan.baidu.com/s/1o7Gn0t8)，直接在SimpleEngine/下解压即可,你会看到Data/文件夹被创建

5. 运行SimpleEngine。

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


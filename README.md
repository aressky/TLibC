TLibC
======
**C语言常用函数库**

- platform 对Linux, Windows, Unix(Mac OSX)下的差异进行了非常简单的封装。
- core 常用的数据结构如链表， hash表， 定时器等。
- protocol 对xml, xlsx, mysql等数据的存取， 提供了统一的接口， 也支持类似于[Thrift](http://thrift.apache.org/)中Binary和Compact的数据格式。

项目依赖
========
- 开启读取Excel表格的功能， 需要安装[zlib](http://www.zlib.net)
- 开启读取mysql的功能， 需要安装[mysql](http://www.mysql.com)


安装方法
========
TLibC使用[CMake](http://www.cmake.org/)作为构建工具， 支持Linux, Windows, Unix(Mac OSX)操作系统。

**1.CMake生成工程文件**

假设需要在build目录下生成工程文件， 源代码位于TLibC目录。

    Linux/Unix:
        cd ./build
        cmake ../TLibC

    Windows:
        打开CMake界面。
        点击Where is the source code栏目右侧的Browse Source选择TLibC目录。
        点击Where is the source code栏目右侧的Browse Build选择build目录。

_CMake选项:_
- CMAKE_BUILD_TYPE 选择编译选项Debug|Release, Windows可以在Visual Studio里面选择。
- CMAKE_INSTALL_PREFIX 指定安装路径。
- TLIBC_INCLUDE_DIR 依赖包TLIBC的头文件目录。
- WITH_RE2C 选项可以重新生成词法分析部分的代码， 需要安装 [re2c](http://www.re2c.org)。

**2.编译安装**

    Linux/Unix:
        make
        make install

    Windows:
        打开build目录下的Visual Studio工程， 生成INSTALL项目。
	
教学
====

	请参考tutorial目录下面的例子。

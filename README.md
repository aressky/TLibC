TLibC
======
**C语言常用函数库**

这个库只有非常少的依赖项， 但是提供了对很多种格式数据的处理功能， 并且有着极佳的性能。

- platform 跨平台模块， 支持Linux,OS X。
- core 常用的数据结构如链表， hash表， 定时器等。
- protocol 支持读写XML, XLSX, MYSQL， 二进制缓存， 压缩二进制缓存

项目依赖
========
	不需要任何的依赖库就可以进行编译安装。
	开启读取Excel表格的功能， 需要安装zlib库。
	开启读取mysql的功能， 需要安装mysql库。


安装方法
========
- Windows:
用[CMake](http://www.cmake.org/)生成VS工程文件

		打开CMake界面。
		在Where is the source code栏目里填入源代码所在的目录， 如"D:/TData"。
		在Where to build the binaries栏目里填入生成工程文件的目录, 如"D:/build"。
		点击Configure进行设置。
		点击Generate生成VS工程文件。
		在弹出的界面中， 选择需要生成的工程类型， 如选择Visual Studio 11， 点击Finish。

	INSTALL

		打开解决方案Build名叫INSTALL的工程， 如果安装的目录是否需要管理员权限， 请点击右键选择用管理员方式启动VS。

- Linux:

	建立编译所需的目录

		mkdir build

	下载源代码

		cd build

	生成Makefile

		cmake ../TData

	下面的我们已经很熟悉了:)

		make
		sodu make install
	
教学
====

	请参考tutorial目录下面的例子。

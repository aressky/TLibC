SET(PNAME "TLibC")
SET(PONAME "tlibc")
project(${PNAME})

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${${PNAME}_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}" )
OPTION(WITH_RE2C "选择此项让re2c重新生成读取正则表达式的代码, 请确认re2c已被正确安装， 并且在命令行输入re2c可以访问。" OFF)

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
	ADD_DEFINITIONS(-D__LCC__)
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")
ENDIF()

SET(BUILDTYPE_DOCSTRING "Choose the type of build, options are: Debug(${CMAKE_C_FLAGS_DEBUG}) Release(${CMAKE_C_FLAGS_RELEASE})")
IF(WIN32)
	SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING ${BUILDTYPE_DOCSTRING} FORCE)
ELSE()
	SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING ${BUILDTYPE_DOCSTRING} FORCE)
	SET(CMAKE_INSTALL_PREFIX "/usr/local/${PNAME}" CACHE STRING "" FORCE)
ENDIF()

set(INCLUDE  ${${PNAME}_SOURCE_DIR}/include)
set(SOURCE  ${${PNAME}_SOURCE_DIR}/source)
set(BINARY  ${${PNAME}_BINARY_DIR})

find_package(ZLIB)
find_package(MySQL)

add_subdirectory(source)

install(DIRECTORY include DESTINATION ".")

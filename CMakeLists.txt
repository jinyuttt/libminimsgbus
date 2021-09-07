# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("libminimsgbus")

add_definitions(-std=c++11)  # 指定采用c++11进行编译（libminimsgbus需要c++11）
include_directories(
 " ${PROJECT_SOURCE_DIR}/deploy/include"
)
set(BUS_ROOT ${PROJECT_SOURCE_DIR})
set(DEPLOY_ROOT ${PROJECT_SOURCE_DIR})
set(DEPLOY_LIB ${PROJECT_SOURCE_DIR}/deploy/lib)
set(DEPLOY_INCLUDE ${PROJECT_SOURCE_DIR}/deploy/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
link_directories("${PROJECT_SOURCE_DIR}/deploy/lib")
# 包含子项目。

FILE(GLOB SC_FILES "*.cpp" "*.h")
add_subdirectory ("libminimsgbus")

add_subdirectory("msgtransport")

add_subdirectory("util")



add_subdirectory("demo")

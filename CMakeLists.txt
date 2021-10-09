# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#

cmake_minimum_required (VERSION 3.8)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project ("libminimsgbus")
set(CMAKE_CXX_STANDARD 11)

#add_definitions(-std=c++11)  # 指定采用c++11进行编译（libminimsgbus需要c++11）
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

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(NNG_WARN_FLAGS "-Wall -Wextra -fno-omit-frame-pointer")
elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(NNG_WARN_FLAGS "-Wall -Wextra -fno-omit-frame-pointer")
endif ()
if (NNG_ENABLE_COVERAGE)
    # NB: This only works for GCC and Clang 3.0 and newer.  If your stuff
    # is older than that, you will need to find something newer.  For
    # correct reporting, we always turn off all optimizations.
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(NNG_COVERAGE_C_FLAGS "-g -O0 --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS --coverage)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(NNG_COVERAGE_C_FLAGS "-g -O0 --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS --coverage)
    else ()
        message(FATAL_ERROR "Unable to enable coverage for your compiler.")
    endif ()
endif ()
link_directories("${PROJECT_SOURCE_DIR}/deploy/lib")
# 包含子项目。

FILE(GLOB SC_FILES "*.cpp" "*.h")
add_subdirectory ("libminimsgbus")

add_subdirectory("msgtransport")

add_subdirectory("util")



add_subdirectory("demo")

set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
        "${PROJECT_NAME}-v${NNG_PACKAGE_VERSION}-src")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "nng")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${NNG_PACKAGE_VERSION}")

add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
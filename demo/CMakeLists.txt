link_directories(${CMake_BINARY_DIR}/lib)
link_directories(${DEPLOY_LIB})
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
   ${BUS_ROOT}/util
    ${BUS_ROOT}/msgtransport
     ${BUS_ROOT}/libminimsgbus
)
link_directories(${DEPLOY_LIB})
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库

add_executable(demo "demo.cpp" "demo.h")
target_link_libraries(demo libminimsgbus msgtransport ipc nng zmq)

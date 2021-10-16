
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
   ${BUS_ROOT}/util
)
link_directories(${DEPLOY_LIB})
file(GLOB ALLHEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB SC_FILES "*.cpp" "*.h")
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
add_library (msgtransport SHARED ${SC_FILES})
target_link_libraries(msgtransport ipc nng zmq)

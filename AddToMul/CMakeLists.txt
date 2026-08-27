cmake_minimum_required(VERSION 3.20)
project(MyLLVMPasses)

# 设置C++标准（LLVM18对应的C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
if(NOT LLVM_PACKAGE_VERSION MATCHES "^18\\.")
    message(FATAL_ERROR "LLVM 18 is required, but found ${LLVM_PACKAGE_VERSION}")
endif()
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM_DIR = ${LLVM_DIR}")
message(STATUS "LLVM include dirs = ${LLVM_INCLUDE_DIRS}")

# 添加LLVM头文件路径以及宏定义
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# 导入LLVM构建辅助函数
list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
include(AddLLVM)

# 构建Pass，生成.so文件并链接LLVM库

add_llvm_pass_plugin(
    AddToMulPlugin
    AddToMulPass.cpp
)

# 去掉.so文件的lib前缀
set_target_properties(AddToMulPlugin PROPERTIES PREFIX "")

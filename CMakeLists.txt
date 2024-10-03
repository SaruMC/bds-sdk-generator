cmake_minimum_required(VERSION 3.30.4)
project(bds-sdk-generator)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_VERSION}")

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to use")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(bds-sdk-generator main.cpp)
target_link_libraries(bds-sdk-generator ${LLVM_LIBS} LLVMCore LLVMDemangle)

target_include_directories(SDKGenerator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SDKGenerator PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
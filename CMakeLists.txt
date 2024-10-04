cmake_minimum_required(VERSION 3.30.4)
project(bds-sdk-generator)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Include the directory where your headers are located
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add all source files to the target
add_executable(bds-sdk-generator main.cpp src/MemoryMappedFile.cpp)

# Link against the necessary LLVM libraries
target_link_libraries(bds-sdk-generator ${LLVM_LIBS} LLVMCore LLVMDemangle)

# Optionally set the output directory for the executable
set_target_properties(bds-sdk-generator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

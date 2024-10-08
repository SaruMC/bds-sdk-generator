cmake_minimum_required(VERSION 3.29.3)
project(bds-sdk-generator)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Enable Microsoft-specific extensions for declspec support
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdeclspec -fms-extensions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec -fms-extensions")

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

# Include the directory where your headers are located
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/PDB)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/PDB/Foundation)

# Collect all source files in the src/PDB directory
file(GLOB PDB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/PDB/*.cpp)

# Link directories where the precompiled libraries are located
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Add all source files to the target
add_executable(bds-sdk-generator
    main.cpp
    ${PDB_SOURCES}
    src/MemoryMappedFile.cpp src/PDBUtils.cpp src/Logger.cpp
)

# Link the PDB library and necessary LLVM libraries
target_link_libraries(bds-sdk-generator ${LLVM_LIBS} LLVMCore LLVMDemangle)

# Optionally set the output directory for the executable
set_target_properties(bds-sdk-generator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
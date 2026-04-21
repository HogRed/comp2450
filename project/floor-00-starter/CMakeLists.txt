cmake_minimum_required(VERSION 3.15)
project(antechamber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Put the executable at build/<name> regardless of generator. On
# multi-config generators (Visual Studio, Xcode) CMake normally buries it
# under build/Debug/ or build/Release/; we override every per-config
# variant so students always find the binary at the same path.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
foreach(cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR})
endforeach()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(antechamber main.cpp)

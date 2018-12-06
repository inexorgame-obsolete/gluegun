set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 2.8.12)
project(core CXX)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers-master/include")

add_executable(core
        core.cpp
        Player.h Player.cpp
        PlayerBuffer_generated.h)
target_link_libraries(core CONAN_PKG::nanomsgxx)



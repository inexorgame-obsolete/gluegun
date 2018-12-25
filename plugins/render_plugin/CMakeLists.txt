set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 2.8.12)
project(rendering_plugin CXX)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

# Flatbuffers (temporary, will become conan)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers-master/include")

# The parent dir of all plugins
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../..")

add_executable(rendering_plugin rendering_plugin.cpp)
target_link_libraries(rendering_plugin CONAN_PKG::nanomsgxx)



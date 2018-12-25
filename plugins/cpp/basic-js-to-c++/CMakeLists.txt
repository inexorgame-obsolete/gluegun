cmake_minimum_required(VERSION 3.8)
project(basic_js_to_c__)

set(CMAKE_CXX_STANDARD 17)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers-master/include")
set(SOURCE_FILES main.cpp DistributedStateAddress_generated.h
        Player.h Player.cpp
        PlayerBuffer_generated.h
        PlayerModule.cpp PlayerModule.h network_nanomsg.cpp network_nanomsg.h network_msg_to_module_dispatcher.cpp network_msg_to_module_dispatcher.h)
add_executable(basic_js_to_c__ ${SOURCE_FILES})
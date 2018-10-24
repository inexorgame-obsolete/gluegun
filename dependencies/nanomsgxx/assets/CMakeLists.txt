cmake_minimum_required(VERSION 3.0)

project(nanomsgxx)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(DetectCXXCompiler)
include(CompilerOptions)

#
# User-defined options.
#

option(NNXX_TESTS "Build nanomsgxx tests" ON)

#
# Conan.io config.
#

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#
# Compiler options.
#

if(MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	add_definitions("/wd4244 /wd4245 /wd4267 /wd4800")

	if(CONAN_LINK_RUNTIME)
	  set(flags
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_MINSIZEREL
		CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	  foreach(flag ${flags})
		if(${flag} MATCHES "/MD")
		  string(REPLACE "/MDd " "${CONAN_LINK_RUNTIME} " ${flag} "${${flag}}")
		  string(REPLACE "/MD " "${CONAN_LINK_RUNTIME} " ${flag} "${${flag}}")
		endif()
	  endforeach()
	endif()
elseif(CMAKE_COMPILER_IS_GNUCXX OR APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#
# Source files.
#

set(NANOMSG_EXT_HEADERS
	src/nanomsg/ext/nnxx_ext.h)

set(NANOMSG_EXT_SOURCES
	src/nanomsg/ext/nnxx_ext.c)

set(NNXX_HEADERS
	src/nnxx/message
	src/nnxx/socket
	src/nnxx/testing
	src/nnxx/bus.h
	src/nnxx/chrono.h
	src/nnxx/error.h
	src/nnxx/inproc.h
	src/nnxx/ipc.h
	src/nnxx/message.h
	src/nnxx/message_control.h
	src/nnxx/message_istream.h
	src/nnxx/message_iterator.h
	src/nnxx/message_ostream.h
	src/nnxx/message_streambuf.h
	src/nnxx/nn.h
	src/nnxx/pair.h
	src/nnxx/pipeline.h
	src/nnxx/poll.h
	src/nnxx/pubsub.h
	src/nnxx/reqrep.h
	src/nnxx/socket.h
	src/nnxx/survey.h
	src/nnxx/tcp.h
	src/nnxx/timeout.h
	src/nnxx/unittest.h
	src/nnxx/message_istream.hpp
	src/nnxx/message_ostream.hpp
	src/nnxx/message_streambuf.hpp
	src/nnxx/socket.hpp)

set(NNXX_SOURCES
	src/nnxx/error.cpp
	src/nnxx/message.cpp
	src/nnxx/message_control.cpp
	src/nnxx/message_istream.cpp
	src/nnxx/message_iterator.cpp
	src/nnxx/message_ostream.cpp
	src/nnxx/message_streambuf.cpp
	src/nnxx/nn.cpp
	src/nnxx/poll.cpp
	src/nnxx/pubsub.cpp
	src/nnxx/reqrep.cpp
	src/nnxx/socket.cpp
	src/nnxx/survey.cpp
	src/nnxx/tcp.cpp
	src/nnxx/timeout.cpp)

source_group("Header Files\\nanomsg" FILES ${NANOMSG_EXT_HEADERS})
source_group("Header Files\\nnxx" FILES ${NNXX_HEADERS})
source_group("Implementation Files\\nanomsg" FILES ${NANOMSG_EXT_SOURCES})
source_group("Implementation Files\\nnxx" FILES ${NNXX_SOURCES})

include_directories(
	src)

#
# Target.
#

add_library(nanomsgxx STATIC
	${NANOMSG_EXT_HEADERS}
	${NANOMSG_EXT_SOURCES}
	${NNXX_HEADERS}
	${NNXX_SOURCES})

if(MSVC)
	set_target_properties(nanomsgxx PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE "/LTCG")
endif()

#
# Tests.
#

if (NNXX_TESTS)
	if(MSVC)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	endif()

	add_executable(test_message tests/test_message.cpp)
	target_link_libraries(test_message nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_message RUNTIME DESTINATION bin)

	add_executable(test_message_istream tests/test_message_istream.cpp)
	target_link_libraries(test_message_istream nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_message_istream RUNTIME DESTINATION bin)

	add_executable(test_message_ostream tests/test_message_ostream.cpp)
	target_link_libraries(test_message_ostream nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_message_ostream RUNTIME DESTINATION bin)

	add_executable(test_nnxx_ext tests/test_nnxx_ext.cpp)
	target_link_libraries(test_nnxx_ext nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_nnxx_ext RUNTIME DESTINATION bin)

	add_executable(test_pair tests/test_pair.cpp)
	target_link_libraries(test_pair nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_pair RUNTIME DESTINATION bin)

	add_executable(test_pipeline tests/test_pipeline.cpp)
	target_link_libraries(test_pipeline nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_pipeline RUNTIME DESTINATION bin)

	add_executable(test_poll tests/test_poll.cpp)
	target_link_libraries(test_poll nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_poll RUNTIME DESTINATION bin)

	add_executable(test_pubsub tests/test_pubsub)
	target_link_libraries(test_pubsub nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_pubsub RUNTIME DESTINATION bin)

	add_executable(test_readme tests/test_readme.cpp)
	target_link_libraries(test_readme nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_readme RUNTIME DESTINATION bin)

	add_executable(test_reqrep tests/test_reqrep.cpp)
	target_link_libraries(test_reqrep nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_reqrep RUNTIME DESTINATION bin)

	add_executable(test_reqrep_multi tests/test_reqrep_multi.cpp)
	target_link_libraries(test_reqrep_multi nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_reqrep_multi RUNTIME DESTINATION bin)
	
	add_executable(test_socket tests/test_socket.cpp)
	target_link_libraries(test_socket nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_socket RUNTIME DESTINATION bin)
	
	add_executable(test_survey tests/test_survey.cpp)
	target_link_libraries(test_survey nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_survey RUNTIME DESTINATION bin)

	add_executable(test_timeout tests/test_timeout.cpp)
	target_link_libraries(test_timeout nanomsgxx ${CONAN_LIBS})
	install(TARGETS test_timeout RUNTIME DESTINATION bin)
endif()

#
# Installation.
#

install(FILES ${NNXX_HEADERS} DESTINATION include/nnxx)
install(FILES ${NANOMSG_EXT_HEADERS} DESTINATION include/nanomsg/ext)
install(TARGETS nanomsgxx
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

if (WIN32 AND CMAKE_GENERATOR MATCHES "(MinGW)|(MSYS)")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "")
endif()

set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

if (CMAKE_COMPILER_IS_GNUCXX)
	# High level of warnings.
	set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall")

	# The -Wno-long-long is required in 64bit systems when including sytem headers.
	if (X86_64)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-long-long")
	endif()

	# We need pthread's
	if (UNIX AND NOT ANDROID)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pthread")
	endif()

	if (WARNINGS_ARE_ERRORS)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
	endif()

	if (X86 AND NOT MINGW64 AND NOT X86_64 AND NOT APPLE)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -march=i686")
	endif()

	# Other optimizations
	if (ENABLE_OMIT_FRAME_POINTER)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fomit-frame-pointer")
	else()
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-omit-frame-pointer")
	endif()
	if (ENABLE_FAST_MATH)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -ffast-math")
	endif()
	if (ENABLE_POWERPC)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mcpu=G3 -mtune=G5")
	endif()
	if (ENABLE_SSE)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse")
	endif()
	if (ENABLE_SSE2 AND NOT ANDROID)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse2")
	endif()

	# SSE3 and further should be disabled under MingW because it generates compiler errors
	if (NOT MINGW)
		if (ENABLE_SSE3)
			set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse3")
		endif()

		if (${CMAKE_GCC_VERSION_NUM} GREATER 402)
			set(HAVE_GCC43_OR_NEWER 1)
		endif()
		if (${CMAKE_GCC_VERSION_NUM} GREATER 401)
			set(HAVE_GCC42_OR_NEWER 1)
		endif()

		if (HAVE_GCC42_OR_NEWER OR APPLE)
			if (ENABLE_SSSE3)
				set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mssse3")
			endif()
			if (HAVE_GCC43_OR_NEWER)
				if (ENABLE_SSE41)
					 set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse4.1")
				endif()
				if (ENABLE_SSE42)
					 set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse4.2")
				endif()
			endif()
		endif()
	endif(NOT MINGW)

	if (X86 OR X86_64)
		if (NOT APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 4)
			 if (ENABLE_SSE2)
				 set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mfpmath=sse")# !! important - be on the same wave with x64 compilers
			 else()
				 set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mfpmath=387")
			 endif()
		endif()
	endif()

	# Profiling?
	if (ENABLE_PROFILING)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pg -g")
		# turn off incompatible options
		foreach(flags CMAKE_CXX_FLAGS CMAKE_C_FLAGS CMAKE_CXX_FLAGS_RELEASE CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG EXTRA_C_FLAGS_RELEASE)
			string(REPLACE "-fomit-frame-pointer" "" ${flags} "${${flags}}")
			string(REPLACE "-ffunction-sections" "" ${flags} "${${flags}}")
		endforeach()
	elseif(NOT APPLE AND NOT ANDROID)
		# Remove unreferenced functions: function level linking
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -ffunction-sections")
	endif()

	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -DNDEBUG")
	set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} -O0 -DDEBUG -D_DEBUG")
	if(BUILD_WITH_DEBUG_INFO)
		set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} -ggdb3")
	endif()
endif()

if (MSVC)
	# Enable Unicode build.
	set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /D _UNICODE /D UNICODE")

	#
	# Debug compilation options.
	#

	# Enable minimal rebuild.
	set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} /Gm")
	
	#
	# Release compilation options.
	#

	# Full optimization.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Ox")
	# Enable link-time code generation.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /GL")
	# Enable intrinsic functions.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Oi")
	# Favor fast code.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Ot")
	# Omit frame pointers.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Oy")
	# Enable string pooling.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /GF")
	# Enable function-level linking.
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Gy")

	# Use link time code generation.
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /LTCG")
	# Eliminate unreferenced data.
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /OPT:REF")
	# Remove redundant COMREFs.
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /OPT:ICF")
	# Set checksum.
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /RELEASE")

	# 64-bit portability warnings, in MSVC80
	if (MSVC80)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Wp64")
	endif()

	# Enable debug info for Release builds
	set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Zi")
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /debug")

	# Remove unreferenced functions: function level linking
	if (NOT MSVC_VERSION LESS 1400)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /bigobj")
	endif()

	# 64-bit MSVC compiler uses SSE/SSE2 by default. ARM builds doesn't support it.
	if (NOT MSVC64 AND NOT CMAKE_GENERATOR STREQUAL "Visual Studio 11 ARM")
		if (ENABLE_SSE)
			set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /arch:SSE")
		endif()
		if (ENABLE_SSE2)
			set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /arch:SSE2")
		endif()
	endif()

	if (ENABLE_SSE3)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /arch:SSE3")
	endif()
	if (ENABLE_SSE4_1)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /arch:SSE4.1")
	endif()
	
	if (ENABLE_SSE OR ENABLE_SSE2 OR ENABLE_SSE3 OR ENABLE_SSE4_1)
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Oi")
	endif()
	
	if (X86 OR X86_64)
		if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND ENABLE_SSE2)
			set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /fp:fast")# !! important - be on the same wave with x64 compilers
		endif()
	endif()

	# Multiprocessor compilation.
	if (ENABLE_MP AND NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.8 AND NOT ${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} LESS 8.6)
		include(ProcessorCount)
		ProcessorCount(N)
		if (NOT N EQUAL 0)
			SET(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /MP${N} ")
		endif()
	endif()
endif()

if (ANDROID AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Turn on debugging information generation.
	set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} -g")
endif()

# Extra link libs if the user selects building static libs:
if (NOT BUILD_SHARED_LIBS AND CMAKE_COMPILER_IS_GNUCXX AND NOT ANDROID)
	# Android does not need these settings because they are already set by toolchain file
	set(LINKER_LIBS ${LINKER_LIBS} stdc++)
	set(EXTRA_C_FLAGS "-fPIC ${EXTRA_C_FLAGS}")
endif()

# Add user supplied extra options (optimization, etc...).
# ==========================================================
set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS}" CACHE INTERNAL "Extra compiler options")
set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}" CACHE INTERNAL "Extra compiler options for Release build")
set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG}" CACHE INTERNAL "Extra compiler options for Debug build")
set(EXTRA_EXE_LINKER_FLAGS "${EXTRA_EXE_LINKER_FLAGS}" CACHE INTERNAL "Extra linker flags")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE}" CACHE INTERNAL "Extra linker flags for Release build")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "${EXTRA_EXE_LINKER_FLAGS_DEBUG}" CACHE INTERNAL "Extra linker flags for Debug build")

# Combine all "extra" options.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG}")

if (MSVC)
	string(REPLACE "/W3" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REPLACE "/W3" "/W4" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
	string(REPLACE "/W3" "/W4" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

	# Remove /STACK:10000000 set by CMake. This value for stack size is very high, limiting the number
	# of threads we can spawn. Default value used by Windows is 1MB which is good enough.
	string(REGEX REPLACE "/STACK:[0-9]+" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
	string(REGEX REPLACE "/STACK:[0-9]+" "" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

	# Remove /Zm1000 set by CMake. This value for PCH memory limit is very high causing problems with MSVC2010+.
	string(REPLACE "/Zm1000" "/Zm256" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REPLACE "/Zm1000" "/Zm256" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

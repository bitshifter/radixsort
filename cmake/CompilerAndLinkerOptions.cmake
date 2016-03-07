# Include this before project or enable_language

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

enable_language(C)
enable_language(CXX)

if(NOT CMAKE_MACHINE_TYPE)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_MACHINE_TYPE 32)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_MACHINE_TYPE 64)
	endif()
endif()

if(UNIX)
	if(CMAKE_MACHINE_TYPE EQUAL 32)
		set(CMAKE_MACHINE_FLAGS -m32 -msse2)
	elseif(CMAKE_MACHINE_TYPE EQUAL 64)
		set(CMAKE_MACHINE_FLAGS -m64)
	endif()

	# set common C/C++ flags
	set(COMPILER_FLAGS
		-Wall
		-Wextra
		-ffast-math
		-g
		${CMAKE_MACHINE_FLAGS}
		)
	set(COMPILER_FLAGS_DEBUG
		-D_DEBUG
		-Og
		#-fsanitize=address
		)
	set(COMPILER_FLAGS_RELEASE
		-DNDEBUG
		-O3
		-fomit-frame-pointer
		-ffunction-sections
		)

	# set C flags
	set(CMAKE_C_FLAGS_LIST ${COMPILER_FLAGS}
		-std=c11
		)
	set(CMAKE_C_FLAGS_DEBUG_LIST ${COMPILER_FLAGS_DEBUG})
	set(CMAKE_C_FLAGS_RELEASE_LIST ${COMPILER_FLAGS_RELEASE})

	# set C++ flags
	set(CMAKE_CXX_FLAGS_LIST ${COMPILER_FLAGS}
		-fno-exceptions
		-fno-rtti
		-std=c++11
		)
	set(CMAKE_CXX_FLAGS_DEBUG_LIST ${COMPILER_FLAGS_DEBUG})
	set(CMAKE_CXX_FLAGS_RELEASE_LIST ${COMPILER_FLAGS_RELEASE})

	set(CMAKE_EXE_LINKER_FLAGS_LIST ${CMAKE_MACHINE_FLAGS})
elseif(MSVC)
	# set common C/C++ flags
	set(COMPILER_FLAGS
		# Preprocessor definitions
		/D_WINDOWS
		/DWIN32

		# General
		/W4			# Warning level 4
		/Zi			# Generate debug program database
		/MP			# Multi-processor compilation
		/WX			# Treat warnings as errors

		# Code generation
		/Gy			# Enable function level linking
		/fp:fast	# Fast floating point model

		# Additional Options
		/d2Zi+
		)

	set(COMPILER_FLAGS_DEBUG
		# Preprocessor definitions
		/D_DEBUG

		# Optimization
		/Od			# Disable optimization
		/Ob0		# Disable inline function expansion

		# Code generation
		/MDd		# Multi-threaded debug DLL runtime library
		/RTC1		# Basic runtime checks
		)

	set(COMPILER_FLAGS_RELEASE
		# Preprocessor definitions
		/DNDEBUG

		# Optimization
		/Ox			# Full optimization
		/Ob2		# Any suitable inline function expansion
		/Oi			# Enable intrinsic functions
		/Ot			# Favor fast code

		# Code generation
		/GF			# Enable string pooling
		/MD			# Multi-threaded debug DLL runtime library
		)

	# set arch flags
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(COMPILER_MACHINE_FLAGS /arch:SSE2)
		set(LINKER_MACHINE_FLAGS /MACHINE:X86)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(COMPILER_MACHINE_FLAGS /D_WIN64)
		set(LINKER_MACHINE_FLAGS /MACHINE:X64)
	endif()

	# set C flags
	set(CMAKE_C_FLAGS_LIST ${COMPILER_FLAGS}
		${COMPILER_MACHINE_FLAGS}
		)
	set(CMAKE_C_FLAGS_DEBUG_LIST ${COMPILER_FLAGS_DEBUG})
	set(CMAKE_C_FLAGS_RELEASE_LIST ${COMPILER_FLAGS_RELEASE})

	# set C++ flags
	set(CMAKE_CXX_FLAGS_LIST ${COMPILER_FLAGS}
		${COMPILER_MACHINE_FLAGS}
		/D_HAS_EXCEPTIONS=0
		/GR-
		/EHs-c-
		)
	set(CMAKE_CXX_FLAGS_DEBUG_LIST ${COMPILER_FLAGS_DEBUG})
	set(CMAKE_CXX_FLAGS_RELEASE_LIST ${COMPILER_FLAGS_RELEASE})

	# set linker flags
	set(CMAKE_EXE_LINKER_FLAGS_LIST
		${LINKER_MACHINE_FLAGS}
		/INCREMENTAL:NO
		/DEBUG
		)
	set(CMAKE_SHARED_LINKER_FLAGS_LIST
		/INCREMENTAL:NO
		/DEBUG
		)
endif()

# Override default CMake compiler flags
string(REPLACE ";" " " CMAKE_C_FLAGS				"${CMAKE_C_FLAGS_LIST}")
string(REPLACE ";" " " CMAKE_C_FLAGS_DEBUG			"${CMAKE_C_FLAGS_DEBUG_LIST}")
string(REPLACE ";" " " CMAKE_C_FLAGS_RELEASE		"${CMAKE_C_FLAGS_RELEASE_LIST}")

string(REPLACE ";" " " CMAKE_CXX_FLAGS				"${CMAKE_CXX_FLAGS_LIST}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG_LIST}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE_LIST}")

# Override default CMake linker flags

# Linker flags (exe)
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS		"${CMAKE_EXE_LINKER_FLAGS_LIST}")
# Linker flags (shared)
string(REPLACE ";" " " CMAKE_SHARED_LINKER_FLAGS	"${CMAKE_SHARED_LINKER_FLAGS_LIST}")

message(STATUS "C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "SHARED_LIBRARY_C_FLAGS: ${CMAKE_SHARED_LIBRARY_C_FLAGS}")
message(STATUS "SHARED_LIBRARY_CXX_FLAGS: ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
message(STATUS "SHARED_LIBRARY_LINK_CXX_FLAGS: ${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS}")
message(STATUS "SHARED_LIBRARY_LINK_C_FLAGS: ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMAKE_EXECUTABLE_SUFFIX: ${CMAKE_EXECUTABLE_SUFFIX}")


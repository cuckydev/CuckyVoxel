cmake_minimum_required(VERSION 3.8)

if((${CMAKE_VERSION} VERSION_EQUAL 3.9) OR (${CMAKE_VERSION} VERSION_GREATER 3.9))
	cmake_policy(SET CMP0069 NEW)
endif()

###########
# Options #
###########
option(COMPILE_CLIENT "Compile a CuckyVoxel client instead of a CuckyVoxel server" ON)
option(BACKEND_SDL2 "Enable the SDL2 backend (does nothing for server binaries)" ON)

option(LTO "Enable link-time optimization" OFF)
option(PKG_CONFIG_STATIC_LIBS "On platforms with pkg-config, static-link the dependencies (good for Windows builds, so you don't need to bundle DLL files)" OFF)
option(MSVC_LINK_STATIC_RUNTIME "Link the static MSVC runtime library (Visual Studio only)" OFF)

#########
# Setup #
#########

# Define project, common source, and includes
project(CuckyVoxel LANGUAGES CXX)

add_executable(CuckyVoxel WIN32
	# Utility
	"src/Common/Util/Error.cpp"
	"src/Common/Util/Error.h"
	"src/Common/Util/Random.cpp"
	"src/Common/Util/Random.h"
	
	# Noise
	"src/Common/Noise/Perlin.cpp"
	"src/Common/Noise/Perlin.h"
	"src/Common/Noise/Simplex.cpp"
	"src/Common/Noise/Simplex.h"
	"src/Common/Noise/Octaves_Perlin.cpp"
	"src/Common/Noise/Octaves_Perlin.h"
	"src/Common/Noise/Octaves_Simplex.cpp"
	"src/Common/Noise/Octaves_Simplex.h"
	
	# World
	"src/Common/World/WorldDef.h"
	"src/Common/World/WorldPosition.cpp"
	"src/Common/World/WorldPosition.h"
	"src/Common/World/ChunkManager.cpp"
	"src/Common/World/ChunkManager.h"
	"src/Common/World/ChunkGenerator.cpp"
	"src/Common/World/ChunkGenerator.h"
	"src/Common/World/Chunk.cpp"
	"src/Common/World/Chunk.h"
)

target_include_directories(CuckyVoxel PRIVATE "src")

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
)

##################
# Compiler flags #
##################

# Strip release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# Statically link other libraries
if(PKG_CONFIG_STATIC_LIBS)
	target_link_options(CuckyVoxel PRIVATE "-static")
endif()

# Enable threading functions
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Use LTO if requested
if(LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result)
	if(result)
		set_target_properties(CuckyVoxel PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

########
# MSVC #
########

# This is messy as hell, and has been replaced by CMAKE_MSVC_RUNTIME_LIBRARY,
# but that's a very recent CMake addition, so we're still doing it this way for now
if(MSVC AND MSVC_LINK_STATIC_RUNTIME)
	# Statically-link the CRT (vcpkg static libs do this)
	foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif()
	endforeach()
endif()

# Do some other MSVC fixes
if(MSVC)
	# Disable warnings that normally fire up on MSVC when using "unsafe" functions instead of using MSVC's "safe" _s functions
	target_compile_definitions(CuckyVoxel PRIVATE _CRT_SECURE_NO_WARNINGS)
	
	# Make it so source files are recognized as UTF-8 by MSVC
	target_compile_options(CuckyVoxel PRIVATE "/utf-8")
	
	# Use `main` instead of `WinMain`
	set_target_properties(CuckyVoxel PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

##########################
# Further compiler setup #
##########################

# Force strict C++17
set_target_properties(CuckyVoxel PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

# Set debug executable name
set_target_properties(CuckyVoxel PROPERTIES DEBUG_OUTPUT_NAME "CuckyVoxel_debug")

# Set output directory
if(COMPILE_CLIENT)
	set(BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin_client")
else()
	set(BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin_server")
endif()

set_target_properties(CuckyVoxel PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIRECTORY}
)

############################
# Compile client or server #
############################

# Find PkgConfig for dependency linking
find_package(PkgConfig QUIET)

if(COMPILE_CLIENT)
	# Compile client source files
	target_sources(CuckyVoxel PRIVATE
		# CuckyVoxel client
		"src/Client/Main.cpp"
		"src/Client/Main.h"
		"src/Client/TextureData.cpp"
		"src/Client/TextureData.h"
		"src/Client/World/World.cpp"
		"src/Client/World/World.h"
		"src/Client/World/ChunkMesh.cpp"
		"src/Client/World/ChunkMesh.h"
		
		# Backend
		"src/Client/Backend/Backend.cpp"
		"src/Client/Backend/Backend.h"
		"src/Client/Backend/Core.h"
		"src/Client/Backend/Render.h"
		"src/Client/Backend/ShaderFile.cpp"
		"src/Client/Backend/ShaderFile.h"
		
		# Dependencies
		"lib/lodepng/lodepng.cpp"
	)
	
	target_include_directories(CuckyVoxel PRIVATE "lib")
	
	# Compile SDL2 backend
	if(BACKEND_SDL2)
		target_compile_definitions(CuckyVoxel PRIVATE COMPILE_SDL2)
		target_sources(CuckyVoxel PRIVATE
			"src/Client/Backend/SDL2/Core.cpp"
			"src/Client/Backend/SDL2/Core.h"
			"src/Client/Backend/SDL2/Render.cpp"
			"src/Client/Backend/SDL2/Render.h"
			"src/Client/Backend/SDL2/Event.cpp"
			"src/Client/Backend/SDL2/Event.h"
		)
		
		# Find SDL2
		find_package(SDL2)
		if(PKG_CONFIG_FOUND)
			pkg_check_modules(sdl2 QUIET IMPORTED_TARGET sdl2)
		endif()
		
		# Link SDL2
		if(TARGET PkgConfig::sdl2)
			if(PKG_CONFIG_STATIC_LIBS)
				# Pkg-Config static
				message(STATUS "Using system SDL2 (pkg-config, static)")
				list(REMOVE_ITEM sdl2_STATIC_CFLAGS "-Dmain=SDL_main")
				list(REMOVE_ITEM sdl2_STATIC_LDFLAGS "-lSDL2main")
				target_compile_options(CuckyVoxel PRIVATE ${sdl2_STATIC_CFLAGS})
				target_link_libraries(CuckyVoxel PRIVATE ${sdl2_STATIC_LDFLAGS})
			else()
				# Pkg-Config dynamic
				message(STATUS "Using system SDL2 (pkg-config, dynamic)")
				list(REMOVE_ITEM sdl2_CFLAGS "-Dmain=SDL_main")
				list(REMOVE_ITEM sdl2_LDFLAGS "-lSDL2main")
				target_compile_options(CuckyVoxel PRIVATE ${sdl2_CFLAGS})
				target_link_libraries(CuckyVoxel PRIVATE ${sdl2_LDFLAGS})
			endif()
		elseif(TARGET SDL2::SDL2)
			# CMake-generated config (Arch, vcpkg, Raspbian)
			message(STATUS "Using system SDL2 (CMake, dynamic)")
			target_link_libraries(CuckyVoxel PRIVATE SDL2::SDL2)
		elseif(TARGET SDL2::SDL2-static)
			# CMake-generated config (Arch, vcpkg, Raspbian)
			message(STATUS "Using system SDL2 (CMake, static)")
			target_link_libraries(CuckyVoxel PRIVATE SDL2::SDL2-static)
		elseif(SDL2_FOUND)
			# Autotools-generated config (MSYS2)
			message(STATUS "Using system SDL2 (Autotools)")
			target_include_directories(CuckyVoxel PRIVATE ${SDL2_INCLUDE_DIRS})
			target_link_libraries(CuckyVoxel PRIVATE ${SDL2_LIBRARIES})
		endif()
	endif()
	
	# Compile OpenGL backend
	if(BACKEND_SDL2)
		target_compile_definitions(CuckyVoxel PRIVATE COMPILE_OPENGL)
		target_sources(CuckyVoxel PRIVATE
			"src/Client/Backend/OpenGL/Render.cpp"
			"src/Client/Backend/OpenGL/Render.h"
			"src/Client/Backend/OpenGL/Texture.cpp"
			"src/Client/Backend/OpenGL/Texture.h"
			"src/Client/Backend/OpenGL/Mesh.cpp"
			"src/Client/Backend/OpenGL/Mesh.h"
			"src/Client/Backend/OpenGL/Shader.cpp"
			"src/Client/Backend/OpenGL/Shader.h"
		)
		
		# Find GLEW
		find_package(GLEW)
		if(PKG_CONFIG_FOUND)
			pkg_check_modules(glew QUIET IMPORTED_TARGET glew)
		endif()
		
		# Link GLEW
		if(TARGET PkgConfig::glew)
			if(PKG_CONFIG_STATIC_LIBS)
				# Pkg-Config static
				message(STATUS "Using system GLEW (pkg-config, static)")
				target_compile_options(CuckyVoxel PRIVATE ${glew_STATIC_CFLAGS})
				target_link_libraries(CuckyVoxel PRIVATE ${glew_STATIC_LDFLAGS})
			else()
				# Pkg-Config dynamic
				message(STATUS "Using system GLEW (pkg-config, dynamic)")
				target_compile_options(CuckyVoxel PRIVATE ${glew_CFLAGS})
				target_link_libraries(CuckyVoxel PRIVATE ${glew_LDFLAGS})
			endif()
		elseif(TARGET GLEW::GLEW)
			# CMake-generated config (Arch, vcpkg, Raspbian)
			message(STATUS "Using system GLEW (CMake, dynamic)")
			target_link_libraries(CuckyVoxel PRIVATE GLEW::GLEW)
		elseif(TARGET GLEW::GLEW-static)
			# CMake-generated config (Arch, vcpkg, Raspbian)
			message(STATUS "Using system GLEW (CMake, static)")
			target_link_libraries(CuckyVoxel PRIVATE GLEW::GLEW-static)
		elseif(GLEW_FOUND)
			# Autotools-generated config (MSYS2)
			message(STATUS "Using system GLEW (Autotools)")
			target_include_directories(CuckyVoxel PRIVATE ${GLEW_INCLUDE_DIRS})
			target_link_libraries(CuckyVoxel PRIVATE ${GLEW_LIBRARIES})
		endif()
		
		# Find and link OpenGL
		if (CMAKE_VERSION GREATER_EQUAL 3.11)
			cmake_policy(SET CMP0072 NEW)
		endif()
		
		find_package(OpenGL REQUIRED)
		target_link_libraries(CuckyVoxel PRIVATE OpenGL::GL)
	endif()
else()
	
endif()


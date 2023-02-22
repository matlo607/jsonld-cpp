#
# Possible Input Settings (defined in environment or set on command line)
#
# JSONLDCPP_PROJECT : Project name. Default: "jsonld-cpp"
#
# JSONLDCPP_BUILD_TESTS : Build test executables [ON OFF]. Default: ON.
# JSONLDCPP_BUILD_EXAMPLES : Build example executables [ON OFF]. Default: ON.
# INSTALL_JSONLDCPP : Enable installation [ON OFF]. Default: ON.
#
# The test executables use googletest and rapidcheck. Other projects that
# embed jsonld-cpp and already use googletest and/or rapidcheck could set
# these options to OFF
# JSONLDCPP_BUILD_GOOGLETEST : Build googletest for testing [ON OFF]. Default: ON if JSONLDCPP_BUILD_TESTS is ON.
# JSONLDCPP_BUILD_RAPIDCHECK : Build rapidcheck for testing [ON OFF]. Default: ON if JSONLDCPP_BUILD_TESTS is ON.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Version settings

set(JSONLDCPP_VERSION_MAJOR 0)
set(JSONLDCPP_VERSION_MINOR 5)
set(JSONLDCPP_VERSION_PATCH 0)
set(JSONLDCPP_VERSION ${JSONLDCPP_VERSION_MAJOR}.${JSONLDCPP_VERSION_MINOR}.${JSONLDCPP_VERSION_PATCH} CACHE STRING "" FORCE)

# General settings

set(JSONLDCPP_PROJECT "jsonld-cpp" CACHE STRING "Project Name")

# Test settings
option(JSONLDCPP_BUILD_TESTS "Build test executables" ON)
# Examples settings
option(JSONLDCPP_BUILD_EXAMPLES "Build example executables" ON)
# Load conanbuildinfo.cmake
option(USE_CONAN "If Conan is installed, use Conan to pull the project dependencies" OFF)

###############
# Conan support
###############

find_program (CONAN_BIN conan)
if(CONAN_BIN AND USE_CONAN)
    message (STATUS "Found conan C++ package manager: ${CONAN_BIN}")
    find_file (CONANFILE NAMES "conanfile.txt" HINTS "${CMAKE_SOURCE_DIR}")
    if (CONANFILE)
        message (STATUS "Found ${CONANFILE}")
        if (NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
            message(STATUS "Trying to execute 'conan install'")
            get_filename_component(CONANFILE_DIR ${CONANFILE} DIRECTORY)
            execute_process(COMMAND conan install ${CONANFILE_DIR} --build=missing)
        endif()
    endif()

    if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        message(STATUS "conan_basic_setup()")
        conan_basic_setup(TARGETS)
    else()
        message(WARNING "Please run 'conan install' if you plan to use conan")
    endif()
endif()

##########################################
# Local CMake scripts and finders (if any)
##########################################

if (EXISTS "${CMAKE_SOURCE_DIR}/cmake/Modules/")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
endif()

###############################
# Check compiler's capabilities
###############################

include (CheckCCompilerFlag)
include (CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set (CMAKE_COMPILER_IS_CLANG true)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set (CMAKE_COMPILER_IS_MSVC true)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    find_program (LINKER_BIN gold)
    if(LINKER_BIN)
        set(LINKER_BIN "gold")
    else()
        find_program (LINKER_BIN ld.gold)
        if(LINKER_BIN)
            set(LINKER_BIN "ld.gold")
        else()
            set(LINKER_BIN "ld")
        endif()
    endif()

    if (CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options(-fdiagnostics-color=always)
    elseif (CMAKE_COMPILER_IS_CLANG)
        add_compile_options(-fcolor-diagnostics)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=address")

    CHECK_CXX_COMPILER_FLAG("-fuse-ld=${LINKER_BIN}" USE_LINKER_LD_GOLD)
    if(USE_LINKER_LD_GOLD)
        set (CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=${LINKER_BIN} ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH OFF)
endif()

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
set (CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")
set (CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
set (CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
message(STATUS "CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")

# Log settings

message(STATUS "---------------- JSONLDCPP OPTIONS ----------------")

message(STATUS "JSONLDCPP_PROJECT            : " ${JSONLDCPP_PROJECT})
message(STATUS "JSONLDCPP_VERSION            : " ${JSONLDCPP_VERSION})

message(STATUS "JSONLDCPP_BUILD_TESTS        : " ${JSONLDCPP_BUILD_TESTS})
message(STATUS "JSONLDCPP_BUILD_EXAMPLES     : " ${JSONLDCPP_BUILD_EXAMPLES})

message(STATUS "CMAKE_BUILD_TYPE            : " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX        : " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_TOOLCHAIN_FILE        : " ${CMAKE_TOOLCHAIN_FILE})

message(STATUS "---------------------------------------------------")

# Project

project(${JSONLDCPP_PROJECT} VERSION ${JSONLDCPP_VERSION}
        DESCRIPTION "Digital Contract Design's C++ implementation of JSON-LD"
        HOMEPAGE_URL "https://github.com/dcdpr/jsonld-cpp"
        LANGUAGES CXX )

add_subdirectory(jsonld-cpp)
if(JSONLDCPP_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(JSONLDCPP_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
else()
  message(STATUS "Skipping unit tests")
endif()
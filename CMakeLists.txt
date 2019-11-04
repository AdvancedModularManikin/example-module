#############################
# CMake - Test Module - root
#############################

cmake_minimum_required(VERSION 3.5.1)

project("AMMExampleModule")

set(VERSION_TYPE "beta" CACHE STRING "version type" FORCE)
set(PROJECT_VERSION_MAJOR, 1)
set(PROJECT_VERSION_MINOR, 0)
set(PROJECT_VERSION_PATCH@, 0)
site_name(VERSION_HOST) # read hostname to VERSION_HOST
set(VERSION_HOST "${VERSION_HOST}" CACHE STRING "host of build" FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
    add_compile_options(/std:c++14 /W3)
    add_definitions(-DNOMINMAX)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DBOOST_ALL_NO_LIB)
    add_definitions(-DBOOST_ALL_DYN_LINK)
    add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)
    add_definitions(-DBOOST_DATE_TIME_STATIC_LIB)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_THREADAPI win32)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_RELEASE_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        set(CMAKE_INSTALL_PREFIX "C:/Program Files/${CMAKE_PROJECT_NAME}")
    else ()
        set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/${CMAKE_PROJECT_NAME}")
    endif ()
else ()
    add_compile_options(-std=c++14)
    add_compile_options(-O0)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
endif ()

if (DEFINED ENV{FASTRTPS_HOME})
    list(APPEND CMAKE_PREFIX_PATH $ENV{FASTRTPS_HOME})
endif ()
if (DEFINED ENV{FASTRTPSHOME})
    list(APPEND CMAKE_PREFIX_PATH $ENV{FASTRTPSHOME})
endif ()

find_package(Boost REQUIRED)
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)
find_package(amm_std REQUIRED)

include_directories(Source)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(Source)

file(COPY Config DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

message(STATUS "")
message(STATUS "    == Final overview for ${PROJECT_NAME} ==")
message(STATUS "Version:              ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} ${VERSION_TYPE} @ ${VERSION_HOST}")
message(STATUS "Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Output:               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Compiler:             ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_BUILD_TYPE:     ${CMAKE_BUILD_TYPE}")
message(STATUS "")

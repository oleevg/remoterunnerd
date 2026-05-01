##############################################
#
# CMake project build suite v1.0.0
# Author: Oleg F., fedorov.ftf@gmail.com
#
##############################################

cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(runnerd C CXX)

include(CMakeLists.var)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_VERBOSE_MAKEFILE "ON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)

find_package(Threads REQUIRED)

enable_testing()

add_definitions(${${project_name}_definitions})

foreach (module ${modules})
  add_subdirectory(${module})
endforeach ()
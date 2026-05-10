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

# Shared source lists for clang-format and clang-tidy targets
file(GLOB_RECURSE ALL_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/core/*.cpp    ${CMAKE_SOURCE_DIR}/core/*.hpp    ${CMAKE_SOURCE_DIR}/core/*.h
  ${CMAKE_SOURCE_DIR}/network/*.cpp ${CMAKE_SOURCE_DIR}/network/*.hpp ${CMAKE_SOURCE_DIR}/network/*.h
  ${CMAKE_SOURCE_DIR}/common/*.cpp  ${CMAKE_SOURCE_DIR}/common/*.hpp  ${CMAKE_SOURCE_DIR}/common/*.h
  ${CMAKE_SOURCE_DIR}/server/*.cpp  ${CMAKE_SOURCE_DIR}/server/*.hpp  ${CMAKE_SOURCE_DIR}/server/*.h
)
# clang-tidy only needs translation units; headers are analyzed transitively
file(GLOB_RECURSE ALL_CPP_FILES
  ${CMAKE_SOURCE_DIR}/core/src/*.cpp
  ${CMAKE_SOURCE_DIR}/network/src/*.cpp
  ${CMAKE_SOURCE_DIR}/common/src/*.cpp
  ${CMAKE_SOURCE_DIR}/common/test/*.cpp
  ${CMAKE_SOURCE_DIR}/server/src/*.cpp
)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
    COMMENT "Formatting source files with clang-format"
    VERBATIM
  )

  add_custom_target(check-format
    COMMAND ${CLANG_FORMAT} --dry-run --Werror ${ALL_SOURCE_FILES}
    COMMENT "Checking source formatting with clang-format"
    VERBATIM
  )
endif()

# clang-tidy — requires compile_commands.json (use Ninja or -DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
find_program(CLANG_TIDY clang-tidy)
find_program(RUN_CLANG_TIDY NAMES run-clang-tidy run-clang-tidy-18 run-clang-tidy-17 run-clang-tidy-16)
if(CLANG_TIDY)
  if(RUN_CLANG_TIDY)
    add_custom_target(tidy
      COMMAND ${RUN_CLANG_TIDY} -p ${CMAKE_BINARY_DIR} -j 4
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Running clang-tidy in parallel (run-clang-tidy)"
      VERBATIM
    )
  else()
    add_custom_target(tidy
      COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${ALL_CPP_FILES}
      COMMENT "Running clang-tidy"
      VERBATIM
    )
  endif()
endif()
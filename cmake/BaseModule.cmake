include("${CMAKE_SOURCE_DIR}/cmake/CommonFunctions.cmake")

## 
set(${project_name}_${module_name}_link_type
    )

message(STATUS "**********************************************************")
message(STATUS "******** Processing ${project_name}_${module_name} *******")
message(STATUS "**********************************************************")


# Variable to cause subproject libraries to be shared or static. If present,
# this will override global BUILD_SHARED_LIBS flag.
if ("${module_type}" STREQUAL "library")
  set(${project_name}_${module_name}_library_type ${library_type}
      CACHE STRING "Type of ${project_name}_${module_name} library to be created (STATIC|SHARED)."
      )
endif ()

# Set a root directory for include files.
set(${project_name}_${module_name}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL "${project_name}_${module_name} headers directory."
    )

message(STATUS "${project_name}_${module_name}_INCLUDE_DIR -> ${CMAKE_CURRENT_SOURCE_DIR}/include")

set(_headers
    )

set(_sources
    )

# Put a list of header files into the cache.
add_prefix(_headers "${CMAKE_CURRENT_SOURCE_DIR}" "${headers}")
message(STATUS "_headers=${_headers}")
set(${project_name}_${module_name}_HEADERS ${_headers}
    CACHE INTERNAL "${project_name}_${module_name} headers."
    )

# Put a list of sources into the cache.
add_prefix(_sources "${CMAKE_CURRENT_SOURCE_DIR}" "${sources}")
message(STATUS "_sources=${_sources}")
set(${project_name}_${module_name}_SOURCES ${_sources}
    CACHE INTERNAL "${project_name}_${module_name} sources"
    )

# Adds custom definitions to the compiler command line. 
add_definitions(${definitions})

#
# Add a library or an executable to the project using all source files.
#
if ("${module_type}" STREQUAL "library")
  add_library(${project_name}_${module_name}
              ${${project_name}_${module_name}_library_type}
              ${${project_name}_${module_name}_HEADERS}
              ${${project_name}_${module_name}_SOURCES}
              )

  set(${project_name}_${module_name}_link_type "PUBLIC"
      CACHE INTERNAL "${project_name}_${module_name} linking type."
      )
elseif ("${module_type}" STREQUAL "executable")

  message(STATUS "${project_name}_${module_name}
        ${${project_name}_${module_name}_HEADERS}
        ${${project_name}_${module_name}_SOURCES}")

  add_executable(${project_name}_${module_name}
                 ${${project_name}_${module_name}_HEADERS}
                 ${${project_name}_${module_name}_SOURCES}
                 )

  set(${project_name}_${module_name}_link_type "PUBLIC"
      CACHE INTERNAL "${project_name}_${module_name} linking type."
      )
elseif ("${module_type}" STREQUAL "interface")
  add_library(${project_name}_${module_name}
              INTERFACE
              )

  target_sources(${project_name}_${module_name}
                 INTERFACE ${${project_name}_${module_name}_HEADERS}
                 )

  set(${project_name}_${module_name}_link_type "INTERFACE"
      CACHE INTERNAL "${project_name}_${module_name} linking type."
      )
else ()
  message(ERROR_FATAL "Unknown module type for ${project_name}")
endif ()

#
# Switch exports to the appropriate state STATIC or SHARED.
#
if (${BUILD_SHARED_LIBS} OR "${${project_name}_${module_name}_library_type}" STREQUAL "SHARED")
  add_definitions(-D${project_name}_${module_name}_SHARED_DEFINE)
endif ()

message(STATUS "${project_name}_${module_name}
    ${${project_name}_${module_name}_link_type} ${${project_name}_${module_name}_INCLUDE_DIR}")

target_include_directories(${project_name}_${module_name}
                           ${${project_name}_${module_name}_link_type} ${${project_name}_${module_name}_INCLUDE_DIR})

## Link a target to the given libraries.
if (WIN32)
  list(APPEND libraries
       ${libraries_WIN32}
       )
  message(STATUS "Adding platform specific libraries: ${${project_name}_${module_name}} -> ${libraries}")
elseif (APPLE)
  list(APPEND libraries
       ${libraries_APPLE}
       )
elseif (UNIX)
  list(APPEND libraries
       ${libraries_UNIX}
       )
endif ()

foreach (dependency ${libraries})
  if (${${project_name}_${module_name}_link_type} STREQUAL "INTERFACE")
    message(STATUS "Adding link:INTERFACE lib:${dependency} for ${project_name}_${module_name}")
    target_link_libraries(${project_name}_${module_name} INTERFACE ${dependency})
  else ()
    message(STATUS "Adding link:${${dependency}_link_type} lib:${dependency} for ${project_name}_${module_name}")
    target_link_libraries(${project_name}_${module_name} ${${dependency}_link_type} ${dependency})
  endif ()
endforeach ()

foreach (dependency ${external_libraries})
  target_link_libraries(${project_name}_${module_name} PUBLIC ${dependency})
endforeach ()

#
# Module tests processing
#

## Put a list of test sources into the cache.
set(${project_name}_${module_name}_TESTS ${tests}
    CACHE INTERNAL "${project_name}_${module_name} tests")

#
# Add test module if tests sources are specified and skip this step overwise.
#
if (${project_name}_${module_name}_TESTS)
  find_package(Boost COMPONENTS system unit_test_framework program_options REQUIRED)
  message(STATUS "Found BOOST placement: Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")

  add_definitions(-DBOOST_TEST_DYN_LINK)
  include_directories(${Boost_INCLUDE_DIR})

  # Add test's executable to the project using the specified source files.
  add_executable(${project_name}_${module_name}_test
                 ${${project_name}_${module_name}_TESTS}
                 )
  # Link a target to the given libraries.
  #add_dependencies(${project_name}_${module_name}_test ${project_name}_${module_name})

  if (WIN32)
    list(APPEND libraries_test
         ${libraries_test_WIN32}
         )
    message(STATUS "Adding platform specific libraries for tests: ${${project_name}_${module_name}} -> ${libraries_test}")
  elseif (APPLE)
    list(APPEND libraries_test
         ${libraries_test_APPLE}
         )
  elseif (UNIX)
    list(APPEND libraries_test
         ${libraries_test_UNIX}
         )
  endif ()

  ## Link a target to the given libraries.
  foreach (dependency ${libraries_test})
    message(STATUS "Adding link:${${dependency}_link_type} lib:${dependency} for ${project_name}_${module_name}_test")
    if (NOT DEFINED ${${dependency}_link_type})
      target_link_libraries(${project_name}_${module_name}_test ${dependency})
    else ()
      target_link_libraries(${project_name}_${module_name}_test ${dependency})
    endif ()
  endforeach ()

  target_link_libraries(${project_name}_${module_name}_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

  add_test(NAME ${project_name}_${module_name} COMMAND ${project_name}_${module_name}_test)
else ()
  message(STATUS "Skipping test module generation for ${project_name}_${module_name}")
endif ()




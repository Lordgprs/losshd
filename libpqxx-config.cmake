########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(libpqxx_FIND_QUIETLY)
    set(libpqxx_MESSAGE_MODE VERBOSE)
else()
    set(libpqxx_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/libpqxxTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${libpqxx_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(libpqxx_VERSION_STRING "6.4.8")
set(libpqxx_INCLUDE_DIRS ${libpqxx_INCLUDE_DIRS_RELEASE} )
set(libpqxx_INCLUDE_DIR ${libpqxx_INCLUDE_DIRS_RELEASE} )
set(libpqxx_LIBRARIES ${libpqxx_LIBRARIES_RELEASE} )
set(libpqxx_DEFINITIONS ${libpqxx_DEFINITIONS_RELEASE} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${libpqxx_BUILD_MODULES_PATHS_RELEASE} )
    message(${libpqxx_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()



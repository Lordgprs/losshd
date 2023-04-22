# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libpqxx_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(libpqxx_FRAMEWORKS_FOUND_RELEASE "${libpqxx_FRAMEWORKS_RELEASE}" "${libpqxx_FRAMEWORK_DIRS_RELEASE}")

set(libpqxx_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libpqxx_DEPS_TARGET)
    add_library(libpqxx_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libpqxx_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${libpqxx_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${libpqxx_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:PostgreSQL::PostgreSQL>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libpqxx_DEPS_TARGET to all of them
conan_package_library_targets("${libpqxx_LIBS_RELEASE}"    # libraries
                              "${libpqxx_LIB_DIRS_RELEASE}" # package_libdir
                              "${libpqxx_BIN_DIRS_RELEASE}" # package_bindir
                              "${libpqxx_LIBRARY_TYPE_RELEASE}"
                              "${libpqxx_IS_HOST_WINDOWS_RELEASE}"
                              libpqxx_DEPS_TARGET
                              libpqxx_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "libpqxx"    # package_name
                              "${libpqxx_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libpqxx_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT libpqxx::pqxx #############

        set(libpqxx_libpqxx_pqxx_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(libpqxx_libpqxx_pqxx_FRAMEWORKS_FOUND_RELEASE "${libpqxx_libpqxx_pqxx_FRAMEWORKS_RELEASE}" "${libpqxx_libpqxx_pqxx_FRAMEWORK_DIRS_RELEASE}")

        set(libpqxx_libpqxx_pqxx_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET libpqxx_libpqxx_pqxx_DEPS_TARGET)
            add_library(libpqxx_libpqxx_pqxx_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET libpqxx_libpqxx_pqxx_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_DEPENDENCIES_RELEASE}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'libpqxx_libpqxx_pqxx_DEPS_TARGET' to all of them
        conan_package_library_targets("${libpqxx_libpqxx_pqxx_LIBS_RELEASE}"
                              "${libpqxx_libpqxx_pqxx_LIB_DIRS_RELEASE}"
                              "${libpqxx_libpqxx_pqxx_BIN_DIRS_RELEASE}" # package_bindir
                              "${libpqxx_libpqxx_pqxx_LIBRARY_TYPE_RELEASE}"
                              "${libpqxx_libpqxx_pqxx_IS_HOST_WINDOWS_RELEASE}"
                              libpqxx_libpqxx_pqxx_DEPS_TARGET
                              libpqxx_libpqxx_pqxx_LIBRARIES_TARGETS
                              "_RELEASE"
                              "libpqxx_libpqxx_pqxx"
                              "${libpqxx_libpqxx_pqxx_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET libpqxx::pqxx
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_LIBRARIES_TARGETS}>
                     APPEND)

        if("${libpqxx_libpqxx_pqxx_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET libpqxx::pqxx
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         libpqxx_libpqxx_pqxx_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_LINKER_FLAGS_RELEASE}> APPEND)
        set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_INCLUDE_DIRS_RELEASE}> APPEND)
        set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_LIB_DIRS_RELEASE}> APPEND)
        set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_COMPILE_DEFINITIONS_RELEASE}> APPEND)
        set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_RELEASE}> APPEND)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET libpqxx::pqxx PROPERTY INTERFACE_LINK_LIBRARIES libpqxx::pqxx APPEND)

########## For the modules (FindXXX)
set(libpqxx_LIBRARIES_RELEASE libpqxx::pqxx)

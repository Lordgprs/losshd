########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND libpqxx_COMPONENT_NAMES libpqxx::pqxx)
list(REMOVE_DUPLICATES libpqxx_COMPONENT_NAMES)
list(APPEND libpqxx_FIND_DEPENDENCY_NAMES PostgreSQL)
list(REMOVE_DUPLICATES libpqxx_FIND_DEPENDENCY_NAMES)
set(PostgreSQL_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(libpqxx_PACKAGE_FOLDER_RELEASE "/root/.conan2/p/libpqf8c624f7c9e38/p")
set(libpqxx_BUILD_MODULES_PATHS_RELEASE )


set(libpqxx_INCLUDE_DIRS_RELEASE "${libpqxx_PACKAGE_FOLDER_RELEASE}/include")
set(libpqxx_RES_DIRS_RELEASE )
set(libpqxx_DEFINITIONS_RELEASE )
set(libpqxx_SHARED_LINK_FLAGS_RELEASE )
set(libpqxx_EXE_LINK_FLAGS_RELEASE )
set(libpqxx_OBJECTS_RELEASE )
set(libpqxx_COMPILE_DEFINITIONS_RELEASE )
set(libpqxx_COMPILE_OPTIONS_C_RELEASE )
set(libpqxx_COMPILE_OPTIONS_CXX_RELEASE )
set(libpqxx_LIB_DIRS_RELEASE "${libpqxx_PACKAGE_FOLDER_RELEASE}/lib")
set(libpqxx_BIN_DIRS_RELEASE )
set(libpqxx_LIBRARY_TYPE_RELEASE STATIC)
set(libpqxx_IS_HOST_WINDOWS_RELEASE 0)
set(libpqxx_LIBS_RELEASE pqxx)
set(libpqxx_SYSTEM_LIBS_RELEASE )
set(libpqxx_FRAMEWORK_DIRS_RELEASE )
set(libpqxx_FRAMEWORKS_RELEASE )
set(libpqxx_BUILD_DIRS_RELEASE )
set(libpqxx_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(libpqxx_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${libpqxx_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${libpqxx_COMPILE_OPTIONS_C_RELEASE}>")
set(libpqxx_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libpqxx_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libpqxx_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libpqxx_EXE_LINK_FLAGS_RELEASE}>")


set(libpqxx_COMPONENTS_RELEASE libpqxx::pqxx)
########### COMPONENT libpqxx::pqxx VARIABLES ############################################

set(libpqxx_libpqxx_pqxx_INCLUDE_DIRS_RELEASE "${libpqxx_PACKAGE_FOLDER_RELEASE}/include")
set(libpqxx_libpqxx_pqxx_LIB_DIRS_RELEASE "${libpqxx_PACKAGE_FOLDER_RELEASE}/lib")
set(libpqxx_libpqxx_pqxx_BIN_DIRS_RELEASE )
set(libpqxx_libpqxx_pqxx_LIBRARY_TYPE_RELEASE STATIC)
set(libpqxx_libpqxx_pqxx_IS_HOST_WINDOWS_RELEASE 0)
set(libpqxx_libpqxx_pqxx_RES_DIRS_RELEASE )
set(libpqxx_libpqxx_pqxx_DEFINITIONS_RELEASE )
set(libpqxx_libpqxx_pqxx_OBJECTS_RELEASE )
set(libpqxx_libpqxx_pqxx_COMPILE_DEFINITIONS_RELEASE )
set(libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_C_RELEASE "")
set(libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_CXX_RELEASE "")
set(libpqxx_libpqxx_pqxx_LIBS_RELEASE pqxx)
set(libpqxx_libpqxx_pqxx_SYSTEM_LIBS_RELEASE )
set(libpqxx_libpqxx_pqxx_FRAMEWORK_DIRS_RELEASE )
set(libpqxx_libpqxx_pqxx_FRAMEWORKS_RELEASE )
set(libpqxx_libpqxx_pqxx_DEPENDENCIES_RELEASE PostgreSQL::PostgreSQL)
set(libpqxx_libpqxx_pqxx_SHARED_LINK_FLAGS_RELEASE )
set(libpqxx_libpqxx_pqxx_EXE_LINK_FLAGS_RELEASE )
set(libpqxx_libpqxx_pqxx_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(libpqxx_libpqxx_pqxx_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libpqxx_libpqxx_pqxx_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libpqxx_libpqxx_pqxx_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libpqxx_libpqxx_pqxx_EXE_LINK_FLAGS_RELEASE}>
)
set(libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${libpqxx_libpqxx_pqxx_COMPILE_OPTIONS_C_RELEASE}>")
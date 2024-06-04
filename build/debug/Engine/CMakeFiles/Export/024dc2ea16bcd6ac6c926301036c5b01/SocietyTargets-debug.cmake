#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Society::Society" for configuration "Debug"
set_property(TARGET Society::Society APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Society::Society PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libSocietyd.a"
  )

list(APPEND _cmake_import_check_targets Society::Society )
list(APPEND _cmake_import_check_files_for_Society::Society "${_IMPORT_PREFIX}/lib/libSocietyd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

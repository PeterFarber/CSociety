#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Society::Society" for configuration "Release"
set_property(TARGET Society::Society APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Society::Society PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSocietyr.a"
  )

list(APPEND _cmake_import_check_targets Society::Society )
list(APPEND _cmake_import_check_files_for_Society::Society "${_IMPORT_PREFIX}/lib/libSocietyr.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

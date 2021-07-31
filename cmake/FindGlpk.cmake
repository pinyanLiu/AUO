# This will define the following variables::
#
#   GLPKFOUND    - True if the system has the glpk library
#   GLPKINCLUDE_DIRS     - location of header files
#   GLPKLIBRARIES        - location of library files


find_path(GLPK_INCLUDE_DIRS glpk.h
    "/usr/include"
    "/usr/local/include"
    )
find_library(GLPK_LIBRARIES libglpk.so
    "/usr/lib"
    "/usr/local/lib"
    )
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Glpk
    FOUND_VAR GLPK_FOUND
    REQUIRED_VARS GLPK_INCLUDE_DIRS GLPK_LIBRARIES
    )

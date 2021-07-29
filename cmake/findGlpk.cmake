# This will define the following variables::
#
#   GLPKFOUND    - True if the system has the glpk library
#   GLPKINCLUDE_DIRS     - location of header files
#   GLPKLIBRARIES        - location of library files


find_path(GLPKINCLUDE_DIRS glpk.h
    "/usr/include"
    "/usr/local/include"
    )
find_library(GLPKLIBRARIES libglpk.so
    "/usr/lib"
    "/usr/local/lib"
    )
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Glpk
    FOUND_VAR GLPKFOUND
    REQUIRED_VARS GLPKINCLUDE_DIRS GLPKLIBRARIES
    )

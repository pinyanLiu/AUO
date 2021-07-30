# This will define the following variables::
#
#   MYSQL++FOUND    - True if the system has the MYSQL++ library
#   MYSQL++INCLUDE_DIRS     - location of header files
#   MYSQL++LIBRARIES        - location of library files


find_path(MYSQLINCLUDE_DIRS mysql++.h
    "/usr/include"
    "/usr/local/include"
    )
find_library(MYSQLLIBRARIES libmysqlpp.so
    "/usr/lib"
    "/usr/local/lib"
    )
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MYSQL++
    FOUND_VAR MYSQL++FOUND
    REQUIRED_VARS MYSQLIN++CLUDE_DIRS MYSQL++LIBRARIES
    )
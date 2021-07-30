# This will define the following variables::
#
#   MYSQLFOUND    - True if the system has the MYSQL library
#   MYSQLINCLUDE_DIRS     - location of header files
#   MYSQLLIBRARIES        - location of library files


find_path(MYSQLINCLUDE_DIRS mysql.h
    "/usr/include"
    "/usr/local/include"
    )
find_library(MYSQLLIBRARIES libmysqlclient.so
    "/usr/lib"
    "/usr/local/lib"
    )
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MYSQL
    FOUND_VAR MYSQLFOUND
    REQUIRED_VARS MYSQLINCLUDE_DIRS MYSQLLIBRARIES
    )
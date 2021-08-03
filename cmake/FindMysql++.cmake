# This will define the following variables::
#
#   MYSQLPP++FOUND    - True if the system has the MYSQL++ library
#   MYSQLPP++INCLUDE_DIRS     - location of header files
#   MYSQLPP++LIBRARIES        - location of library files

find_path(MYSQLPP_INCLUDE_DIRS mysql++.h
    "/usr/include/mysql++"
    "/usr/local/include"
    )
find_library(MYSQLPP_LIBRARIES libmysqlpp.so.3
    "/usr/lib"
    "/usr/local/lib"
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MysqlPP
    FOUND_VAR MysqlPP_FOUND
    REQUIRED_VARS  MYSQLPP_INCLUDE_DIRS MYSQLPP_LIBRARIES 
    )

# This will define the following variables::
#
#   MYSQL++FOUND    - True if the system has the MYSQL++ library
#   MYSQL++INCLUDE_DIRS     - location of header files
#   MYSQL++LIBRARIES        - location of library files

find_path(MYSQL_INCLUDE_DIRS mysql.h
    "/usr/include/mysql"
    "/usr/local/include"
    )
find_library(MYSQLCLIENT_LIBRARIES libmysqlclient.so
    "/usr/lib"
    "/usr/local/lib"
    )



include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Mysql
    FOUND_VAR Mysql_FOUND
    REQUIRED_VARS  MYSQL_INCLUDE_DIRS MYSQLCLIENT_LIBRARIES
    )

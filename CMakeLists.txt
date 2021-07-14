cmake_minimum_required(VERSION 3.10)

 # Disable in-source builds to prevent source tree corruption.
 if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
     message(FATAL_ERROR "\nFATAL: In-source builds are not allowed.\n"
         "You should create a separate directory for build files."
         )
 endif()

 # Set variables
 set(PROJECT_NAME k10_stage1)
 set(PROJECT_LANGUAGE C CXX)

 # Environment variables
 set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory")
 option(BUILD_TEST "Enable building test" OFF)
 option(BUILD_SHARED_LIBS "Build shared library" OFF)

 set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build configure" FORCE)
 if(DEFINED CMAKE_BUILD_TYPE)
     set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
 endif()
 if(NOT CMAKE_BUILD_TYPE)
     set(CMAKE_BUILD_TYPE Release)
 endif()

 # Let ycm can catch compile commands.
 set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

 # Dependences paths
 set(DEPS_PATHS
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/Wheel_Server/lib/wclt
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/3wheel_ctrl/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/conio/src
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/Camera/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/camera_server/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/helper/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/SocketHelper/
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/Socket/
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/kproto/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/tcpmgr/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/args/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/Wheel/src
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/k10_arm_lib/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/dynamixel_lib/lib
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/SimplePID/src
     ${CMAKE_CURRENT_SOURCE_DIR}/deps/karm_server_dynamixel/lib/karm_client


     )

 # Library paths
 set(LIB_PATHS
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/ltproc
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/yoloClient
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/laserClient
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/qtrClient
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/SensorModel
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/PlanModel
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/ActModel
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/Process
     ${CMAKE_CURRENT_SOURCE_DIR}/lib/wheelClient
     )

 # Utility paths
 set(UTIL_PATHS
     ${CMAKE_CURRENT_SOURCE_DIR}/util
     )

 if(${BUILD_TEST})
     set(UTIL_PATHS ${UTIL_PATHS} ${CMAKE_CURRENT_SOURCE_DIR}/test)
 endif()

 # Set project
 project(${PROJECT_NAME} ${PROJECT_LANGUAGE})

 # Include cmake files
 include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps.cmake)
 include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/lib.cmake)
 include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/util.cmake)

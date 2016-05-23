#
# c_abstraction_test,
# a c++ plugin intereface for Score-P plugins.
# Copyright (C) 2015 TU Dresden, ZIH
#

project(scorep_cplus_abstraction_plugin)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Wextra")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/common;${CMAKE_MODULE_PATH}")

find_package(Scorep)
if(SCOREP_FOUND)
    include_directories(${SCOREP_INCLUDE_DIRS})
    link_directories(${SCOREP_LIBRARY_DIRS})
else()
    message(SEND_ERROR "Scorep was not found but is required!")
endif()

include_directories(include)

add_library(async_plugin SHARED scorep_async_test_plugin.cpp)
add_library(async_oid_plugin SHARED scorep_async_object_id_test_plugin.cpp)
add_library(sine_plugin SHARED scorep_sync_test_plugin.cpp)
add_library(sine_strict_plugin SHARED scorep_sync_strict_test_plugin.cpp)

install(TARGETS async_plugin LIBRARY DESTINATION lib)
install(TARGETS async_oid_plugin LIBRARY DESTINATION lib)
install(TARGETS sine_plugin LIBRARY DESTINATION lib)
install(TARGETS sine_strict_plugin LIBRARY DESTINATION lib)

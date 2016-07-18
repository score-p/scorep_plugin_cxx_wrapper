#
# Copyright (c) 2015-2016, Technische Universität Dresden, Germany
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions
#    and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of
#    conditions and the following disclaimer in the documentation and/or other materials provided
#    with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may be used to
#    endorse or promote products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
# WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

project(scorep_cplus_abstraction_plugin)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Wextra")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/common;${CMAKE_MODULE_PATH}")

# Intialize git submodules if not done already
file(GLOB SUBMODULE_FILES "${CMAKE_SOURCE_DIR}/common/*")
list(LENGTH SUBMODULE_FILES COUNT_COMMON)
if(${COUNT_COMMON} EQUAL 0)
    message(STATUS "Initializing git submodule")
    execute_process(COMMAND "git" "submodule" "init" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    execute_process(COMMAND "git" "submodule" "update" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

include(common/DefaultBuildType.cmake)

include(cmake/NitroLog.cmake)

find_package(Scorep REQUIRED)
if(SCOREP_FOUND)
    include_directories(${SCOREP_INCLUDE_DIRS})
    link_directories(${SCOREP_LIBRARY_DIRS})
endif()

include_directories(include)

add_library(async_plugin SHARED scorep_async_test_plugin.cpp)
add_library(async_oid_plugin SHARED scorep_async_object_id_test_plugin.cpp)
add_library(sine_plugin SHARED scorep_sync_test_plugin.cpp)
add_library(sine_strict_plugin SHARED scorep_sync_strict_test_plugin.cpp)

include(CTest)

add_executable(matcher_test tests/matcher_test.cpp)
add_test(matcher_test matcher_test)

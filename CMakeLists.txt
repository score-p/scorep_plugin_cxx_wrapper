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

cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common")

# Intialize git submodules if not done already
include(cmake/GitSubmoduleUpdate.cmake)
git_submodule_update()

include(common/DefaultBuildType.cmake)

set(NITRO_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(lib/nitro)

find_package(Scorep REQUIRED)

add_library(scorep-plugin-cxx INTERFACE)
target_include_directories(scorep-plugin-cxx INTERFACE include)
target_link_libraries(scorep-plugin-cxx INTERFACE Nitro::nitro-log Nitro::nitro-env Scorep::Plugin)
target_compile_features(scorep-plugin-cxx INTERFACE cxx_std_14)

add_library(Scorep::scorep-plugin-cxx ALIAS scorep-plugin-cxx)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    target_compile_options(scorep-plugin-cxx INTERFACE -Wall -pedantic -Wextra)

    add_library(async_plugin MODULE scorep_async_test_plugin.cpp)
    target_link_libraries(async_plugin PRIVATE Scorep::scorep-plugin-cxx)

    add_library(async_oid_plugin MODULE scorep_async_object_id_test_plugin.cpp)
    target_link_libraries(async_oid_plugin PRIVATE Scorep::scorep-plugin-cxx)

    add_library(sine_plugin MODULE scorep_sync_test_plugin.cpp)
    target_link_libraries(sine_plugin PRIVATE Scorep::scorep-plugin-cxx)

    add_library(sine_strict_plugin MODULE scorep_sync_strict_test_plugin.cpp)
    target_link_libraries(sine_strict_plugin PRIVATE Scorep::scorep-plugin-cxx)

    include(CTest)
    add_executable(matcher_test tests/matcher_test.cpp)
    target_link_libraries(matcher_test Scorep::scorep-plugin-cxx)
    add_test(matcher_test matcher_test)
endif()

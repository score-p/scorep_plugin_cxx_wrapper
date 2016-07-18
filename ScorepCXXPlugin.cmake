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

#
# CMake magic to easily use the Score-P CXX plugin wrapper in your plugin.
#

#
# Usage:
#
# git submodule add git@github.com:score-p/scorep_plugin_cxx_wrapper.git scorep
#
# In your CMakeLists.txt
# include(scorep/ScorepCXXPlugin.cmake)
#

# Intialize git submodules if not done already
file(GLOB SUBMODULE_FILES "${CMAKE_CURRENT_LIST_DIR}/common/*")
list(LENGTH SUBMODULE_FILES COUNT_COMMON)
if(${COUNT_COMMON} EQUAL 0)
    message(STATUS "Initializing git submodule")
    execute_process(COMMAND "git" "submodule" "init" WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
    execute_process(COMMAND "git" "submodule" "update" WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
endif()


# inlcude build settings for nitro log
include(${CMAKE_CURRENT_LIST_DIR}/cmake/NitroLog.cmake)

# Note: The CMake Magic requires, that Score-P was found with a FindScorep script.
if(SCOREP_FOUND)
    include_directories(SYSTEM ${SCOREP_INCLUDE_DIRS})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/include)

    message(STATUS "Score-P CXX plugin wrapper found.")
else()
    message(STATUS "Score-P was not found but is required!")
    message(SEND_ERROR "Scorep CXX plugin wrapper needs Score-P!")
endif()

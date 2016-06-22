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

include(${CMAKE_CURRENT_LIST_DIR}/../common/UnsetIfUpdated.cmake)

UnsetIfUpdated(MIN_LOG_LEVEL CMAKE_BUILD_TYPE)

if (NOT MIN_LOG_LEVEL)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(MIN_LOG_LEVEL "trace" CACHE STRING
            "The minimum required severity level of log messages to be compiled into the binary.")
    else()
        set(MIN_LOG_LEVEL "debug" CACHE STRING
            "The minimum required severity level of log messages to be compiled into the binary.")
    endif()

    set_property(CACHE MIN_LOG_LEVEL PROPERTY "STRINGS" "fatal" "error" "warn" "info" "debug"
        "trace")
endif()

add_definitions(-DNITRO_LOG_MIN_SEVERITY=${MIN_LOG_LEVEL})

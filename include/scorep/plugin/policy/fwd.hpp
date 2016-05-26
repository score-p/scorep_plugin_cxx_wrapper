/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename T, typename Policies>
        class post_mortem;

        template <typename T, typename Policies>
        class async;
        template <typename T, typename Policies>
        class sync;
        template <typename T, typename Policies>
        class sync_strict;

        template <typename T, typename Policies>
        class with_initialize;
        template <typename T, typename Policies>
        class scorep_clock;
        template <typename Handle, typename T, typename Policies>
        class object_id;

        template <typename T, typename Policies>
        class once;
        template <typename T, typename Policies>
        class per_host;
        template <typename T, typename Policies>
        class per_process;
        template <typename T, typename Policies>
        class per_thread;

        template <typename T, typename Policies>
        class synchronize;
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP

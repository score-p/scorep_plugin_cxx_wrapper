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

#include <scorep/plugin/util/matcher.hpp>

#include <cassert>
#include <string>

int main()
{
    const std::string chan("artemis/s0/watts");
    assert(scorep::plugin::util::matcher("*")(chan));
    assert(scorep::plugin::util::matcher("**")(chan));
    assert(scorep::plugin::util::matcher("*******")(chan));
    assert(scorep::plugin::util::matcher("*/watts")(chan));
    assert(scorep::plugin::util::matcher("artemis/*")(chan));
    assert(scorep::plugin::util::matcher("*/s0/*")(chan));
    assert(scorep::plugin::util::matcher("*/*")(chan));
    assert(scorep::plugin::util::matcher("*s*s*s*")(chan));
    assert(scorep::plugin::util::matcher("*a*r*t*e*m*i*s*/*s*0*/*w*a*t*t*s*")(chan));
    assert(scorep::plugin::util::matcher(chan)(chan));
    assert(scorep::plugin::util::matcher("*" + chan)(chan));
    assert(scorep::plugin::util::matcher(chan + "*")(chan));
    assert(scorep::plugin::util::matcher("*" + chan + "*")(chan));

    assert(!scorep::plugin::util::matcher("*/s1/*")(chan));
    assert(!scorep::plugin::util::matcher("*/s1/watts")(chan));
    assert(!scorep::plugin::util::matcher("artemis/s1/*")(chan));
    assert(!scorep::plugin::util::matcher("artemis/s1/watts")(chan));
    assert(!scorep::plugin::util::matcher("*artemis/s1/watts*")(chan));
    assert(!scorep::plugin::util::matcher("/watts")(chan));
    assert(!scorep::plugin::util::matcher("artemis")(chan));

    return 0;
}

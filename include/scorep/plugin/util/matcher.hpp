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

#ifndef SCOREP_PLUGIN_UTIL_MATCHER_HPP
#define SCOREP_PLUGIN_UTIL_MATCHER_HPP

#include <sstream>
#include <string>
#include <vector>

namespace scorep
{
namespace plugin
{
    namespace util
    {

        // Very simple class that takes a wild/*/card/n*me@filter:whatever
        class matcher
        {
        public:
            matcher(const std::string& pattern)
            {
                std::string pattern_upper(pattern);
                std::transform(pattern_upper.begin(), pattern_upper.end(), pattern_upper.begin(),
                               ::toupper);

                // We need another delimiter because otherwise getline will *not* return an empty
                // string at the end of line because the * is already consumed by the previous
                // getline, and getline will set the failbit if no character is consumed
                std::stringstream ss(pattern_upper + "*");
                std::string part;
                while (std::getline(ss, part, '*'))
                {
                    if (part.empty())
                    {
                        if (pattern_parts.empty())
                        {
                            prefix_wildcard = true;
                        }
                        postfix_wildcard = true;
                    }
                    else
                    {
                        postfix_wildcard = false;
                        pattern_parts.push_back(part);
                    }
                }
            }

            bool operator()(std::string name)
            {
                std::string name_upper(name);
                std::transform(name_upper.begin(), name_upper.end(), name_upper.begin(), ::toupper);

                size_t start_pos = 0;
                for (auto part : pattern_parts)
                {
                    auto pos = name_upper.find(part, start_pos);
                    if (pos == std::string::npos ||
                        (start_pos == 0 && pos != 0 && !prefix_wildcard))
                    {
                        return false;
                    }
                    start_pos = pos + part.length();
                }
                return postfix_wildcard || start_pos == name_upper.length();
            }

        protected:
            std::vector<std::string> pattern_parts;
            bool prefix_wildcard = false;
            bool postfix_wildcard = false;
        };
    }
}
}

#endif // SCOREP_PLUGIN_UTIL_MATCHER_HPP

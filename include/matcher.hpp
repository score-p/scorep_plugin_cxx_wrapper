/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
//
// Created by tilsche on 6/30/15.
//

#ifndef SCOREP_NI_PLUGIN_MATCHER_HPP
#define SCOREP_NI_PLUGIN_MATCHER_HPP

#include <sstream>
#include <vector>
#include <string>

// Very simple class that takes a wild/*/card/n*me@filter:whatever
class ChannelMatcher
{
public:
    ChannelMatcher(const std::string &pattern)
    {
        // We need another delimiter because otherwise getline will *not* return an empty string at the end of line
        // because the * is already consumed by the previous getline, and getline will set the failbit if no
        // character is consumed
        std::string pattern_upper(pattern);
        std::transform(pattern_upper.begin() ,pattern_upper.end() ,pattern_upper.begin(), ::toupper);

        std::stringstream ss(pattern_upper + "*");
        std::string part;
        while (std::getline(ss, part, '*'))
        {
            if (part.empty()) {
                if (pattern_parts.empty()) {
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
            if (pos == std::string::npos || (start_pos == 0 && pos != 0 && !prefix_wildcard)) {
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

#endif //SCOREP_NI_PLUGIN_MATCHER_HPP

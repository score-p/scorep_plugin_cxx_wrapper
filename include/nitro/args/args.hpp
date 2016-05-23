/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_ARGS_HPP
#define INCLUDE_NITRO_ARGS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace nitro { namespace args {
    
    class parser
    {
    public:
        parser(int argc, char ** argv) : argc(argc), argv(argv)
        {
            bool only_unnamed = false;
            
            for(int i=1; i< argc; i++)
            {
                std::string arg = argv[i];
                            
                if(is_double_dash(arg))
                {
                    only_unnamed = true;
                    continue;
                }
                
                if(is_unnamed(arg) || only_unnamed)
                {
                    unnamed.push_back(arg);
                    continue;
                }
            
                if(is_short_named(arg))
                {
                    for(int j=1; j < arg.size(); j++)
                    {
                        named[std::string(&arg[j], &arg[j+1])] = "";
                    }
                }
            
            }
        }
        
        static bool is_unnamed(const std::string& option)
        {
            return !(is_short_named(option) || is_long_named(option));
        }
        
        static bool is_short_named(const std::string& arg)
        {
            return arg[0] == '-' && arg[1] != '-';
        }
        
        static bool is_long_named(const std::string& arg)
        {
            return arg[0] == '-' && arg[1] == '-';
        }
        
        static bool is_double_dash(const std::string& arg)
        {
            return arg.size() == 2 && is_long_named(arg);
        }
        
        const std::string& operator[](std::size_t i) const
        {
            return unnamed[i];
        }
        
        const std::string& operator[](const std::string& name) const
        {
            return named[name];
        }
        
    private:
        int argc;
        char** argv;
        
        std::vector<std::string> unnamed;
        std::map<std::string, std::string> named;
    };
    
    
}} // namespace nitr::args

#endif //INCLUDE_NITRO_ARGS_HPP

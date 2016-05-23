/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_DL_EXCEPTION_HPP
#define INCLUDE_NITRO_DL_EXCEPTION_HPP

#include <stdexcept>

namespace nitro { namespace dl {
    
    
    class exception : public std::runtime_error
    {
    public:
        explicit exception(const std::string& what, const std::string& dlerror) : std::runtime_error(what), dlerror_(dlerror) {}
        
        const std::string& dlerror() const
        {
            return dlerror_;
        }
        
    private:
        std::string dlerror_;
    };
    
    
}} // namespace nitr::args

#endif //INCLUDE_NITRO_DL_EXCEPTION_HPP


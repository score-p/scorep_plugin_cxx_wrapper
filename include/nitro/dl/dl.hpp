/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_DL_DL_HPP
#define INCLUDE_NITRO_DL_DL_HPP

#include <nitro/dl/symbol.hpp>
#include <nitro/dl/exception.hpp>

#include <dlfcn.h>

#include <string>
#include <sstream>
#include <memory>

namespace nitro { namespace dl {
    
    /**
     * \brief Class for dynamically loading libraries
     * 
     * This class uses libdl to load libraries dynamically.
     * 
     */
    class dl
    {
    public:
        /**
         * \brief construct with the name of the library
         * 
         * 
         * 
         * \param [in] filename the filename of the library {
         * libdl tries to find a library with the given name in LD_LIBRARY_PATH.
         * }
         * 
         * \throws nitro::dl::exception {
         * throws if the library could not be opened. 
         * }
         */
        dl(const std::string& filename) : handle(dlopen(filename.c_str(), RTLD_NOW), [](void * handle){
            dlclose(handle);
        }
        )
        {
            if(handle == nullptr)
            {
                std::stringstream msg;
                msg << "Couldn't open library '" << filename << '\'';
                
                throw nitro::dl::exception(msg.str(), dlerror());
            }
        }
        
        template <typename T>
        nitro::dl::symbol<T> load(const std::string& name)
        {
            return nitro::dl::symbol<T>(handle, name);
        }
        
        std::shared_ptr<void> get() const
        {
            return handle;
        }
        
    private:
        std::shared_ptr<void> handle;
    };
    
    
}} // namespace nitro::dl

#endif //INCLUDE_NITRO_DL_DL_HPP


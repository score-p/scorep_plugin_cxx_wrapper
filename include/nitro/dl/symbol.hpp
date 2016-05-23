/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_DL_SYMBOL_HPP
#define INCLUDE_NITRO_DL_SYMBOL_HPP

#include <nitro/dl/exception.hpp>

#include <dlfcn.h>

#include <string>
#include <sstream>
#include <memory>

namespace nitro
{
namespace dl
{

    /**
     * \brief Class for holding and calling a handler to a dynamically loaded
     *symbol in a typesafe way.
     *
     *
     * \internal
     *
     * This nice trick to get return and argument types out of the instantiation
     * with 'symbol<Ret(Args...)>' is copied from std::function :-)
     *
     */
    template <typename T>
    class symbol;

    /**
     * \brief Class for holding and calling a handler to a dynamically loaded
     *symbol in a typesafe way.
     *
     * \attention Don't let the parental object of type \ref nitro::dl::dl go
     *out of
     * scope. This will fuck up everything.
     */
    template <typename Ret, typename... Args>
    class symbol<Ret(Args...)>
    {
    public:
        /**
         * \brief Constructor for a symbol
         *
         * This constructor gets called by the parental dl instance. You should
         * not call it yourself.
         *
         * \internal
         *
         * \param library the pointer to the \ref nitro::dl::dl instance
         * \param name the name of the symbol which is to be called
         *
         */
        symbol(std::shared_ptr<void> library, const std::string& name)
        : library(library)
        {
            // clear previous errors
            dlerror();

            // dirty cast copied from http://linux.die.net/man/3/dlopen
            *(void**)(&this->handle) = dlsym(library.get(), name.c_str());

            // read out dlerror and check it against NULL, as it's legal for
            // dlsym to return NULL
            char* error = dlerror();
            if (error != nullptr)
            {
                std::stringstream msg;
                msg << "Couldn't open symbol '" << name << '\'';

                throw nitro::dl::exception(msg.str(), error);
            }
        }

        /**
         * \brief Typesafe call of dynamic loaded function
         *
         *
         */
        Ret operator()(Args... args)
        {
            return (*handle)(args...);
        }

    private:
        Ret (*handle)(Args...);

        std::shared_ptr<void> library;
    };
}
} // namespace nitr::args

#endif // INCLUDE_NITRO_DL_SYMBOL_HPP

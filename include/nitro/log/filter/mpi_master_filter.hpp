/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_FILTERL_MPI_MASTER_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTERL_MPI_MASTER_FILTER_HPP

namespace nitro { namespace log { namespace filter {
    
    template <typename Record>
    class mpi_master_filter
    {
    public:
        typedef Record record_type;
        
        bool filter(Record& r) const
        {
            return r.mpi_rank() == 0;
        }
    };
    
}}} // namespace nitro::log::filter

#endif //INCLUDE_NITRO_LOG_FILTERL_MPI_MASTER_FILTER_HPP

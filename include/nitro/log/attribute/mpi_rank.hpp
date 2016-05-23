/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_MPI_RANK_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_MPI_RANK_ATTRIBUTE_HPP

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/intercommunicator.hpp>

namespace nitro { namespace log {
    
    class mpi_rank_attribute
    {
        int my_mpi_rank;
        
    public:
        
        mpi_rank_attribute() : my_mpi_rank(0)
        {
            if(boost::mpi::environment::initialized())
            {
                // lets try, if there is a parent communicator
                MPI_Comm pcomm;
                int ret = MPI_Comm_get_parent(&pcomm);
                if(ret == MPI_SUCCESS && pcomm != MPI_COMM_NULL)
                {
                    boost::mpi::intercommunicator comm(pcomm, boost::mpi::comm_attach);
                    
                    
                    
                    my_mpi_rank = comm.rank() + comm.remote_size();
                }
                else
                {
                    boost::mpi::communicator comm;
                    my_mpi_rank = comm.rank();
                }
            }
        }

        int mpi_rank() const
        {
            return my_mpi_rank;
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_THREAD_ID_ATTRIBUTE_HPP

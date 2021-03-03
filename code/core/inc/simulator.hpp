/*
 * simulator_interface.hpp
 *
 * \date 4 mars 2014
 *      Author: cec
 */

#ifndef SIMULATOR_INTERFACE_HPP_
#define SIMULATOR_INTERFACE_HPP_

#include "ssc/data_source/DataSource.hpp"

namespace ssc
{
    namespace solver
    {
        ssc::data_source::DataSource make_ds(const std::string& yaml);
        void integrate(ssc::data_source::DataSource& ds, const double t0, const double t1);
    }
}

#endif /* SIMULATOR_INTERFACE_HPP_ */

/*
 * YamlTimeSeries.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef YAMLTIMESERIES_HPP_
#define YAMLTIMESERIES_HPP_

#include <map>
#include <string>
#include <vector>

/** \brief Stores the commands read from the command parameter file (for controlled forces)
 */
struct YamlTimeSeries
{
    YamlTimeSeries();
    std::string name;                                    //!< Name of the controlled force
    std::vector<double> t;                               //!< Time instants at which the commands change
    std::map<std::string,std::vector<double> > values;   //!< List of command values at each instant
};


#endif /* YAMLTIMESERIES_HPP_ */

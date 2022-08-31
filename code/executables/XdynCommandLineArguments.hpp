/*
 * XdynCommandLineArguments.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef XDYNCOMMANDLINEARGUMENTS_HPP_
#define XDYNCOMMANDLINEARGUMENTS_HPP_

#include <string>
#include <vector>

struct XdynCommandLineArguments
{
    XdynCommandLineArguments();
    std::vector<std::string> yaml_filenames;
    std::string solver;
    std::string output_filename;
    std::string wave_output;
    double initial_timestep;
    double tstart;
    double tend;
    bool catch_exceptions;
    bool empty() const;
};

#endif /* XDYNCOMMANDLINEARGUMENTS_HPP_ */

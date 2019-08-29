/*
 * XdynForCSCommandLineArguments.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */

#ifndef XDYNFORCSCOMMANDLINEARGUMENTS_HPP_
#define XDYNFORCSCOMMANDLINEARGUMENTS_HPP_

#include <string>
#include <vector>

struct XdynForCSCommandLineArguments
{
    XdynForCSCommandLineArguments();
    bool empty() const;
    std::vector<std::string> yaml_filenames;
    std::string solver;
    double initial_timestep;
    bool catch_exceptions;
    short unsigned int port;
    bool verbose;
    bool show_help;
    bool show_websocket_debug_information;
    bool grpc;
};

#endif /* XDYNFORCSCOMMANDLINEARGUMENTS_HPP_ */

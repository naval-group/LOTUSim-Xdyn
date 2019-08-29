/*
 * XdynForMECommandLineArguments.hpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#ifndef XDYNFORMECOMMANDLINEARGUMENTS_HPP_
#define XDYNFORMECOMMANDLINEARGUMENTS_HPP_

#include <string>
#include <vector>

struct XdynForMECommandLineArguments
{
    XdynForMECommandLineArguments();
    std::vector<std::string> yaml_filenames;
    bool catch_exceptions;
    short unsigned int port;
    bool empty() const;
    bool verbose;
    bool show_help;
    bool show_websocket_debug_information;
    bool grpc;
};

#endif /* XDYNFORMECOMMANDLINEARGUMENTS_HPP_ */

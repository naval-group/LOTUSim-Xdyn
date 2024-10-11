/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 *  Modified on: Oct 11, 2024
 *      Author: julien.prodhon
 */

#include "XdynForMECommandLineArguments.hpp"

XdynForMECommandLineArguments::XdynForMECommandLineArguments() : yaml_filenames(),
                         catch_exceptions(false),
                         address({"127.0.0.1"}),
                         port(0),
                         verbose(false),
                         show_help(false),
                         show_websocket_debug_information(false),
                         grpc(false)
{
}

bool XdynForMECommandLineArguments::empty() const
{
    return yaml_filenames.empty() and (port == 0);
}



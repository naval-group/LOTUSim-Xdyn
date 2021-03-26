/*
 * YamlController.hpp
 *
 *  Created on: Fev, 10 2021
 *      Author: lincker
 */

#ifndef YAMLCONTROLLER_HPP_
#define YAMLCONTROLLER_HPP_

#include <map>
#include <string>
#include <vector>

/** \brief Stores the controller parameters used to generate a command for a controlled force
 */
struct YamlController
{
    YamlController();
    std::string name;                      //!< Name of the controlled force
    std::string output;                    //!< Name of the command outputed by the controller
    std::string type;                      //!< Type of the controller (PID, gRPC)
    double dt;                             //!< Time step of the discrete system
    std::string input;                     //!< Name of the input needed by the controller as setpoint
    std::map<std::string, double> states;  //!< Continuous states used by the controller (e.g. {{'u',1},{'v',-3}} for 'u - 3v')
    std::string rest_of_the_yaml;          //!< All other fields that are spectific to the controller type
};


#endif /* YAMLCONTROLLER_HPP_ */

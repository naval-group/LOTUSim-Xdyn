/*
 * YamlController.hpp
 *
 *  Created on: Fev, 10 2021
 *      Author: lincker
 */

#ifndef YAMLCONTROLLER_HPP_
#define YAMLCONTROLLER_HPP_

#include <string>

/** \brief Stores the controller parameters used to generate a command for a controlled force
 */
struct YamlController
{
    YamlController();
    std::string type;             //!< Type of the controller (PID, gRPC, CSV)
    double dt;                    //!< Time step of the discrete system
    std::string name;             //!< Name of the controller instance (so we can reference it in the outputs section)
    std::string rest_of_the_yaml; //!< All other fields that are spectific to the controller type
};


#endif /* YAMLCONTROLLER_HPP_ */

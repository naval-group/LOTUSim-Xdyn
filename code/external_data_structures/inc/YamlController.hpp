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
    std::string type;                             //!< Type of the controller (PID, gRPC)
    double dt;                                    //!< Time step of the discrete system
    std::map<std::string, double> state_weights;  //!< Weights associated to each state, used to compute the controller's measured input,
                                                  //   with the convention that any missing state has weight 0.
                                                  //   For example "u - 2v" -> { {"u", 1}, {"v", -2} }
    std::string rest_of_the_yaml;                 //!< All other fields that are spectific to the controller type
};


#endif /* YAMLCONTROLLER_HPP_ */

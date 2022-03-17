/*
 * CSVController.cpp
 *
 *  Created on: Mar, 16 2021
 *      Author: cady
 */

#include "InvalidInputException.hpp"
#include "CSVController.hpp"
#include "check_input_yaml.hpp"
#include "yaml.h"

CSVController::CSVController(const double tstart, const double dt, const std::string& yaml_)
    : Controller(tstart, dt)
    , yaml(yaml_)
{
}

CSVController::Yaml::Yaml(const std::string& )
{

}

    std::vector<std::string> CSVController::get_command_names() const
{
    return {};
}

std::string CSVController::get_name() const
{
    return "";
}

void CSVController::update_discrete_states(const double ,
                                           ssc::solver::ContinuousSystem* )
{
    
}

double CSVController::get_measured_value(const ssc::solver::ContinuousSystem* ) const
{
    return 0;
}

double CSVController::compute_command(const double , const double ,
                                      const double )
{
    return 0;
}
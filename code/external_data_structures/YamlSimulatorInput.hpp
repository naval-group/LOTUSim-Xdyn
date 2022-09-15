/*
 * YamlSimulatorInput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLSIMULATORINPUT_HPP_
#define YAMLSIMULATORINPUT_HPP_

#include "external_data_structures/YamlRotation.hpp"
#include "external_data_structures/YamlModel.hpp"
#include "external_data_structures/YamlBody.hpp"
#include "external_data_structures/YamlEnvironmentalConstants.hpp"
#include "external_data_structures/YamlController.hpp"
#include "external_data_structures/YamlTimeSeries.hpp"

struct YamlSimulatorInput
{
    YamlSimulatorInput();
    YamlRotation rotations;
    YamlEnvironmentalConstants environmental_constants;
    std::vector<YamlModel> environment;
    std::vector<YamlBody> bodies;
    std::vector<YamlTimeSeries> setpoints;
    std::vector<YamlController> controllers;
    std::vector<YamlTimeSeries> commands;
};

#endif /* YAMLSIMULATORINPUT_HPP_ */

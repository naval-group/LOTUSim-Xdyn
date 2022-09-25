/*
 * YamlSimulatorInput.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLSIMULATORINPUT_HPP_
#define YAMLSIMULATORINPUT_HPP_

#include "YamlRotation.hpp"
#include "YamlModel.hpp"
#include "YamlBody.hpp"
#include "YamlEnvironmentalConstants.hpp"
#include "YamlController.hpp"
#include "YamlTimeSeries.hpp"

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

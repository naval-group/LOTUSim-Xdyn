/*
 * parse_time_series.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef PARSE_TIME_SERIES_HPP_
#define PARSE_TIME_SERIES_HPP_

#include "YamlTimeSeries.hpp"
#include "yaml.h"

/**  \brief Parses the commands for use by controlled forces.
  *  \snippet simulator_yaml_parser/unit_tests/src/parse_time_seriesTest.cpp parse_command_yaml example
  */
std::vector<YamlTimeSeries> parse_command_yaml(const std::string& yaml //!< YAML data
                                            );

/**  \brief Parses the setpoints for use by the controllers (for controlled forces).
  *  \snippet simulator_yaml_parser/unit_tests/src/parse_setpointsTest.cpp parse_setpoint_yaml example
  */
std::vector<YamlTimeSeries> parse_setpoint_yaml(const std::string& yaml //!< YAML data
                                           );

void operator >> (const YAML::Node& node, YamlTimeSeries& c);

#endif /* PARSE_TIME_SERIES_HPP_ */

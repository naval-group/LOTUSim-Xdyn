/*
 * listeners.hpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#ifndef LISTENERS_HPP_
#define LISTENERS_HPP_

#include <ssc/data_source.hpp>
#include "PIDController.hpp"
#include "Scheduler.hpp"
#include "YamlTimeSeries.hpp"
#include "YamlController.hpp"

/**  \brief Reads data from YAML & builds an interpolation table per command.
  *  \returns DataSource used to retrieve the commands of the controlled forces models at each instant
  *  \snippet listeners_and_controllers/unit_tests/src/listenersTest.cpp listenersTest listen_to_file_example
  */
ssc::data_source::DataSource make_command_listener(const std::vector<YamlTimeSeries>& commands);

/**  \brief Reads data from YAML & builds an interpolation table per setpoint.
  *  \returns DataSource used to retrieve the setpoints of the controlled forces models at each instant
  *  \snippet listeners_and_controllers/unit_tests/src/listenersTest.cpp listenersTest listen_to_file_example
  */
void add_setpoints_listener(ssc::data_source::DataSource& ds, const std::vector<YamlTimeSeries>& setpoints);

/**  \brief Reads data from YAML & returns the corresponding controllers.
  *  \snippet listeners_and_controllers/unit_tests/src/controllersTest.cpp controllersTest listen_to_file_example
  */
std::vector<PIDController> get_pid_controllers(const double tstart,
                                               const std::vector<YamlController>& yaml_controllers,
                                               const std::vector<YamlTimeSeries>& yaml_commands
                                               );

/**  \brief Initializes the given controllers commands in Sim datasource and
  * adds their callbacks to the scheduler.
  *  \snippet observers_and_api/unit_tests/src/PIDControllerTest.cpp controllersTest initialize_controllers
  */
void initialize_controllers(std::vector<PIDController>& controllers,
                            ssc::solver::Scheduler& scheduler,
                            Sim* system
                            );

#endif /* LISTENERS_HPP_ */

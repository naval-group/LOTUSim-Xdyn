/*
 * listeners.hpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#ifndef LISTENERS_HPP_
#define LISTENERS_HPP_

#include <ssc/data_source.hpp>
#include <ssc/solver.hpp>

class Sim;
struct YamlController;
struct YamlTimeSeries;

/**  \brief Reads data from YAML & builds an interpolation table per command.
  *  \returns DataSource used to retrieve the commands of the controlled forces models at each instant
  *  \snippet listeners_and_controllers/unit_tests/listenersTest.cpp listenersTest listen_to_file_example
  */
ssc::data_source::DataSource make_command_listener(const std::vector<YamlTimeSeries>& commands);

/**  \brief Reads data from YAML & builds an interpolation table per setpoint.
  *  \returns DataSource used to retrieve the setpoints of the controlled forces models at each instant
  *  \snippet listeners_and_controllers/unit_tests/listenersTest.cpp listenersTest listen_to_file_example
  */
void add_setpoints_listener(ssc::data_source::DataSource& ds, const std::vector<YamlTimeSeries>& setpoints);

class Controller;
Controller* build_controller(const double tstart, const YamlController& yaml_controller, Sim& sys);
void check_no_controller_outputs_are_defined_in_a_command(const Controller* controller, const std::vector<YamlTimeSeries>& yaml_commands);

/**  \brief Reads data from YAML & returns the corresponding controllers.
  *  \snippet listeners_and_controllers/unit_tests/controllersTest.cpp controllersTest listen_to_file_example
  */
std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> > get_initialized_controllers(
    const double tstart,
    const std::vector<YamlController>& yaml_controllers,
    const std::vector<YamlTimeSeries>& yaml_commands,
    ssc::solver::Scheduler& scheduler,
    Sim& sys
    );

std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> > build_controllers(
    const double tstart,
    const std::vector<YamlController>& yaml_controllers,
    const std::vector<YamlTimeSeries>& yaml_commands,
    Sim& sys
    );

#endif /* LISTENERS_HPP_ */

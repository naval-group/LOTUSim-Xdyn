/*
 * listeners.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "check_input_yaml.hpp"
#include "InterpolationModule.hpp"
#include "InvalidInputException.hpp"
#include "listeners.hpp"
#include "YamlTimeSeries.hpp"

#include <ssc/macros.hpp>
#include TR1INC(memory)

void add_interpolation_table(const std::string& x_name, const std::vector<double>& x, const std::string& y_name, const std::vector<double>& y, ssc::data_source::DataSource& ds);
void add_interpolation_table(const std::string& x_name, const std::vector<double>& x, const std::string& y_name, const std::vector<double>& y, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    TR1(shared_ptr)<ssc::interpolation::LinearInterpolationVariableStep> I(new ssc::interpolation::LinearInterpolationVariableStep(x, y));

    const std::string module_name = x_name + "->" + y_name;
    InterpolationModule module(&ds, module_name, x_name, y_name, I);
    ds.add(module);
    ds.check_out();
}

std::string namify(const std::string& command_name, const std::string& model_name);
std::string namify(const std::string& command_name, const std::string& model_name)
{
    return model_name + "(" + command_name + ")";
}

void add(std::vector<YamlTimeSeries>::const_iterator& that_command, ssc::data_source::DataSource& ds);
void add(std::vector<YamlTimeSeries>::const_iterator& that_command, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    const auto t = that_command->t;
    if (t.size() == 1)
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            ds.set<double>(namify(it->first, that_command->name), it->second.front());
        }
    }
    else
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            try
            {
                add_interpolation_table("t", t, namify(it->first, that_command->name), it->second, ds);
            }
            catch(const ssc::interpolation::PiecewiseConstantVariableStepException& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to build interpolation table between 't' and '" << it->first << "' for force model '" << that_command->name << "': " << e.get_message());
            }
        }
    }
    ds.check_out();
}

ssc::data_source::DataSource make_command_listener(const std::vector<YamlTimeSeries>& commands //!< Parsed YAML commands
                       )
{
    ssc::data_source::DataSource ds;
    ds.check_in(__PRETTY_FUNCTION__);
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        add(that_command, ds);
    }
    ds.check_out();
    return ds;
}


void add_setpoints_listener(ssc::data_source::DataSource& ds,
                           const std::vector<YamlTimeSeries>& setpoints //!< Parsed YAML setpoints
                           )
{
    ds.check_in(__PRETTY_FUNCTION__);
    for (auto that_setpoint = setpoints.begin() ; that_setpoint != setpoints.end() ; ++that_setpoint)
    {
        add(that_setpoint, ds);
    }
    ds.check_out();
}


std::vector<PIDController> get_pid_controllers(const std::vector<YamlController>& yaml_controllers, //!< Parsed YAML controllers
                                               const std::vector<YamlTimeSeries>& yaml_commands //!< Parsed YAML commands
                                               )
{
    std::vector<PIDController> controllers;
    for (YamlController yaml_controller : yaml_controllers)
    {
        if (yaml_controller.type == "PID")
        {
            const PIDController controller(yaml_controller.dt,
                                           yaml_controller.state_weights,
                                           yaml_controller.rest_of_the_yaml
                                           );
            check_controller_output_is_not_defined_in_a_command(controller.yaml.command_name, yaml_commands);
            controllers.push_back(controller);
        }
    }

    return controllers;
}


/*
 * Tests concerning 'initialize_controllers' are found in `observers_and_api/unit_tests/src/PIDControllerTest.cpp`
 * because these tests need a Sim instance, which requires the observers_and_api include directory.
 * To avoid cross-dependencies, the corresponding unit tests are moved to observers_and_api/unit_tests.
 */

void initialize_controllers(std::vector<PIDController>& controllers,
                            ssc::solver::Scheduler& scheduler,
                            Sim* system
                            )
{
    system->set_discrete_state("t", scheduler.get_time());
    for (PIDController &controller:controllers) {
        controller.initialize(scheduler, system);
    }
}

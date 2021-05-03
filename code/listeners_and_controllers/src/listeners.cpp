/*
 * listeners.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include <boost/algorithm/string.hpp> // For boost::to_upper

#include "check_input_yaml.hpp"
#include "InterpolationModule.hpp"
#include "InvalidInputException.hpp"
#include "listeners.hpp"
#include "PIDController.hpp"
#include "GrpcController.hpp"
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
    if (model_name == "")
    {
        return command_name;
    }
    return model_name + "(" + command_name + ")";
}

void add(std::vector<YamlTimeSeries>::const_iterator& that_command, ssc::data_source::DataSource& ds);
void add(std::vector<YamlTimeSeries>::const_iterator& that_command, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    const auto t = that_command->t;
    if (t.size() == 1)
    {
        for (auto it = that_command->values.begin() ; it != that_command->values.end() ; ++it)
        {
            ds.set<double>(namify(it->first, that_command->name), it->second.front());
        }
    }
    else
    {
        for (auto it = that_command->values.begin() ; it != that_command->values.end() ; ++it)
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
    check_command_names(commands);
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

Controller* build_controller(const double tstart, const YamlController& yaml_controller)
{
    if (yaml_controller.type == "PID")
    {
        return new PIDController (tstart, yaml_controller.dt, yaml_controller.rest_of_the_yaml);
    }
    if (yaml_controller.type == "GRPC")
    {
        return GrpcController::build(tstart, yaml_controller.rest_of_the_yaml);
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Controller type '" << yaml_controller.type << "' is unknown. Known controller types are: PID, GRPC");
    return NULL;
}

void check_no_controller_outputs_are_defined_in_a_command(const Controller* controller, const std::vector<YamlTimeSeries>& yaml_commands)
{
    const auto command_names = controller->get_command_names();
    for (const auto command_name:command_names)
    {
        check_controller_output_is_not_defined_in_a_command(command_name, yaml_commands);
    }
}

std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> > get_controllers(const double tstart,
                                               const std::vector<YamlController>& yaml_controllers, //!< Parsed YAML controllers
                                               const std::vector<YamlTimeSeries>& yaml_commands //!< Parsed YAML commands
                                               )
{
    std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> > controllers;
    for (YamlController yaml_controller : yaml_controllers)
    {
        boost::to_upper(yaml_controller.type);
        const auto controller = build_controller(tstart, yaml_controller);
        if (controller)
        {
            check_no_controller_outputs_are_defined_in_a_command(controller, yaml_commands);
            controllers.push_back(std::shared_ptr<ssc::solver::DiscreteSystem> (controller));
        }
    }

    return controllers;
}


/*
 * Tests concerning 'initialize_controllers' are found in `observers_and_api/unit_tests/src/PIDControllerTest.cpp`
 * because these tests need a Sim instance, which requires the observers_and_api include directory.
 * To avoid cross-dependencies, the corresponding unit tests are moved to observers_and_api/unit_tests.
 */

void initialize_controllers(const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& controllers,
                            ssc::solver::Scheduler& scheduler,
                            Sim* system
                            )
{
    system->set_discrete_state("t", scheduler.get_time());
    for (auto controller:controllers)
    {
        controller->initialize(scheduler, system);
    }
}

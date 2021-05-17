#include "GrpcController.hpp"

GrpcController* GrpcController::build(const double tstart, const std::string& yaml)
{
    const auto parsed_yaml = GrpcControllerInterface::parse(yaml);
    const auto grpc = GrpcControllerInterface::build(parsed_yaml, tstart);
    return new GrpcController(tstart, grpc, parsed_yaml.name);
}

std::vector<std::string> GrpcController::get_command_names() const
{
    return grpc->get_command_names();
}

std::string GrpcController::get_name() const
{
    return name;
}

GrpcController::GrpcController (const double tstart, const std::shared_ptr<GrpcControllerInterface>& grpc_, const std::string& name_) :
                    Controller(tstart, grpc_->get_dt())
                    , grpc(grpc_)
                    , name(name_)
{}


void GrpcController::update_discrete_states (const double time,
                                 ssc::solver::ContinuousSystem *sys)
{
    if (sys)
    {
        const auto setpoint_names = grpc->get_setpoint_names();
        std::vector<double> setpoints(setpoint_names.size(), 0);

        for (size_t i = 0 ; i < setpoint_names.size() ; ++i)
        {
            setpoints[i] = Controller::get_setpoint(sys, setpoint_names[i]);
        }
        auto dx_dt = sys->get_latest_dx_dt();
        if (dx_dt.empty())
        {
            dx_dt = std::vector<double>(13, 0);
        }
        const auto response = grpc->get_commands(time, sys->state, dx_dt, setpoints);
        const auto commands = response.commands;
        for (auto command : commands)
        {
            Controller::set_discrete_state(sys, command.first, command.second);
        }
    }
}
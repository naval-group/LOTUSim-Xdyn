#include "GrpcController.hpp"

GrpcController* GrpcController::build(const double tstart, const std::string& yaml)
{
    const auto grpc = GrpcControllerInterface::build(GrpcControllerInterface::parse(yaml), tstart);
    return new GrpcController(tstart, grpc);
}

std::vector<std::string> GrpcController::get_command_names() const
{
    return grpc->get_command_names();
}

GrpcController::GrpcController (const double tstart, const std::shared_ptr<GrpcControllerInterface>& grpc_) :
                    Controller(tstart, grpc->get_dt())
                    , grpc(grpc_)
{}


void GrpcController::update_discrete_states (const double time,
                                 ssc::solver::ContinuousSystem *system)
{
    if (system)
    {
        const auto setpoint_names = grpc->get_setpoint_names();
        std::vector<double> setpoints(setpoint_names.size(), 0);

        for (size_t i = 0 ; i < setpoint_names.size() ; ++i)
        {
            setpoints[i] = Controller::get_setpoint(system, setpoint_names[i]);
        }
        const auto response = grpc->get_commands(time, system->state, system->get_latest_dx_dt(), setpoints);
        const auto commands = response.commands;
        for (auto command : commands)
        {
            Controller::set_discrete_state(system, command.first, command.second);
        }
    }
}
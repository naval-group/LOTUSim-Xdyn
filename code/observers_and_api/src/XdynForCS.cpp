#include <functional>

#include "XdynForCS.hpp"
#include "JSONSerializer.hpp"
#include "SimServerInputs.hpp"
#include "simulator_api.hpp"
#include "CoSimulationObserver.hpp"

XdynForCS::XdynForCS(const std::string& yaml_model, const std::string& solver, const double dt):
        builder(yaml_model),
        dt(dt),
        sim(builder.sim),
        solver(solver)
{
}

XdynForCS::XdynForCS(const std::string& yaml_model,
                  const VectorOfVectorOfPoints& mesh,
                  const std::string& solver,
                  const double dt):
        builder(yaml_model, mesh),
        dt(dt),
        sim(builder.sim),
        solver(solver)
{
}

std::vector<YamlState> XdynForCS::handle(const YamlSimServerInputs& request)
{
    return handle(SimServerInputs(request, builder.Tmax));
}

std::vector<YamlState> XdynForCS::handle(const SimServerInputs& request)
{
    return play_one_step(request);
}

std::vector<YamlState> XdynForCS::play_one_step(const SimServerInputs& request)
{
    if (request.Dt <= 0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Dt should be greater than 0 but got Dt = " << request.Dt);
    }
    const double tstart = request.t;
    const double Dt = request.Dt;
    sim.reset_history();
    sim.set_bodystates(request.full_state_history);
    sim.set_command_listener(request.commands);
    CoSimulationObserver observer(request.requested_output, sim.get_bodies().at(0)->get_name());
    if(solver == "euler")
    {
        simulate<ssc::solver::EulerStepper>(sim, tstart, tstart+Dt, dt, observer);
    }
    else if (solver == "rk4")
    {
        simulate<ssc::solver::RK4Stepper>(sim, tstart, tstart+Dt, dt, observer);
    }
    else if (solver == "rkck")
    {
        simulate<ssc::solver::RKCK>(sim, tstart, tstart+Dt, dt, observer);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "unknown solver");
    }
    return observer.get();
}

double XdynForCS::get_Tmax() const
{
    return builder.Tmax;
}

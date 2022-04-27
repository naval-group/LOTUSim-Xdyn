/*
 * XdynForME.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#include <tuple>

#include "InternalErrorException.hpp"
#include "XdynForME.hpp"
#include "SimulationServerObserver.hpp"

XdynForME::XdynForME(const std::string& yaml_model) :builder(yaml_model)
{
}

double XdynForME::get_Tmax() const
{
    return builder.Tmax;
}

YamlState XdynForME::handle(const YamlSimServerInputs& request)
{
    return handle(SimServerInputs(request, get_Tmax()));
}

std::tuple<double, double, double> get_euler_derivative(const StateType& state);
std::tuple<double, double, double> get_euler_derivative(const StateType& state)
{
    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention = {"z", "y'", "x''"};
    const ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(state[9],state[10],state[11],state[12]).matrix();
    const ssc::kinematics::EulerAngles angles = BodyStates::convert(R, rot);
    const double cos_phi = std::cos(angles.phi);
    const double sin_phi = std::sin(angles.phi);
    const double cos_theta = std::cos(angles.theta);
    const double tan_theta = std::tan(angles.theta);
    const double p = state[6];
    const double q = state[7];
    const double r = state[8];
    const double dphi_dt = p + sin_phi*tan_theta *q + cos_phi*tan_theta*r;
    const double dtheta_dt = cos_phi*q - sin_phi*r;
    const double dpsi_dt = sin_phi/cos_theta*q + cos_phi/cos_theta*r;
    return std::tuple<double, double, double>(dphi_dt, dtheta_dt, dpsi_dt);
}

YamlState XdynForME::handle(const SimServerInputs& request)
{
    const double t = request.t;
    builder.sim.set_bodystates(request.state_history_except_last_point);
    builder.sim.set_command_listener(request.commands);

    StateType dx_dt(13, 0);
    // Here we use a CoSimulationObserver, but only for the requested extra observations.
    SimulationServerObserver observer(request.requested_output);
    observer.observe_before_solver_step(builder.sim, request.t, std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >());
    builder.sim.dx_dt(request.state_at_t, dx_dt, t);
    observer.observe_after_solver_step(builder.sim, request.t, std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >());
    observer.flush();

    YamlState state_derivatives;
    state_derivatives.t = request.t;
    if (dx_dt.size() != 13)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Size of state derivatives is invalid: expected a state vector of size 13 (x, y, z, u, v, w, p, q, r, qr, qi, qj, qk) but got a vector of size " << dx_dt.size());
    }
    state_derivatives.x = dx_dt[0];
    state_derivatives.y = dx_dt[1];
    state_derivatives.z = dx_dt[2];
    state_derivatives.u = dx_dt[3];
    state_derivatives.v = dx_dt[4];
    state_derivatives.w = dx_dt[5];
    state_derivatives.p = dx_dt[6];
    state_derivatives.q = dx_dt[7];
    state_derivatives.r = dx_dt[8];
    state_derivatives.qr = dx_dt[9];
    state_derivatives.qi = dx_dt[10];
    state_derivatives.qj = dx_dt[11];
    state_derivatives.qk = dx_dt[12];
    auto angles = get_euler_derivative(dx_dt);
    state_derivatives.phi = std::get<0>(angles);
    state_derivatives.theta = std::get<1>(angles);
    state_derivatives.psi = std::get<2>(angles);
    if (not(observer.get().empty()))
    {
        state_derivatives.extra_observations = observer.get().at(0).extra_observations;
    }

    return state_derivatives;
}

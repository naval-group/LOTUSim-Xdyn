/*
 * XdynForCSgrpc.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */
#include <cmath>
#include <sstream>
#include <tuple>
#include "BodyStates.hpp"
#include "ModelExchangeServiceImpl.hpp"
#include "SimServerInputs.hpp"
#include "YamlSimServerInputs.hpp"
#include "ErrorOutputter.hpp"

ModelExchangeServiceImpl::ModelExchangeServiceImpl(const XdynForME& xdyn_):
simserver(xdyn_),
error()
{}

YamlSimServerInputs from_grpc(grpc::ServerContext* , const ModelExchangeRequestEuler* request)
{
    YamlSimServerInputs server_inputs;
    ssc::kinematics::EulerAngles angles;
    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention = {"z", "y'", "x''"};
    if (request)
    {

        server_inputs.Dt = 0; // Not used by XdynForME anyway
        std::map<std::string, double> commands(request->commands().begin(),
                request->commands().end());
        server_inputs.commands = commands;
        const size_t n = request->states().t_size();
        server_inputs.states.resize(n);
        for (size_t i = 0 ; i < n ; ++i)
        {
            server_inputs.states[i].x = request->states().x(i);
            server_inputs.states[i].y = request->states().y(i);
            server_inputs.states[i].z = request->states().z(i);
            server_inputs.states[i].u = request->states().u(i);
            server_inputs.states[i].v = request->states().v(i);
            server_inputs.states[i].w = request->states().w(i);
            server_inputs.states[i].p = request->states().p(i);
            server_inputs.states[i].q = request->states().q(i);
            server_inputs.states[i].r = request->states().r(i);
            angles.phi = request->states().phi(i);
            angles.theta = request->states().theta(i);
            angles.psi = request->states().psi(i);
            const auto quaternion = BodyStates::convert(angles, rot);
            server_inputs.states[i].qr = std::get<0>(quaternion);
            server_inputs.states[i].qi = std::get<1>(quaternion);
            server_inputs.states[i].qj = std::get<2>(quaternion);
            server_inputs.states[i].qk = std::get<3>(quaternion);
        }
        server_inputs.requested_output = std::vector<std::string>(request->requested_output().begin(),
                                                                  request->requested_output().end());
    }
    return server_inputs;
}

YamlSimServerInputs from_grpc(grpc::ServerContext* , const ModelExchangeRequestQuaternion* request)
{
    YamlSimServerInputs server_inputs;
    if (request)
    {

        server_inputs.Dt = 0; // Not used by XdynForME anyway
        std::map<std::string, double> commands(request->commands().begin(),
                request->commands().end());
        server_inputs.commands = commands;
        const size_t n = request->states().t_size();
        server_inputs.states.resize(n);
        for (size_t i = 0 ; i < n ; ++i)
        {
            server_inputs.states[i].x = request->states().x(i);
            server_inputs.states[i].y = request->states().y(i);
            server_inputs.states[i].z = request->states().z(i);
            server_inputs.states[i].u = request->states().u(i);
            server_inputs.states[i].v = request->states().v(i);
            server_inputs.states[i].w = request->states().w(i);
            server_inputs.states[i].p = request->states().p(i);
            server_inputs.states[i].q = request->states().q(i);
            server_inputs.states[i].r = request->states().r(i);
            server_inputs.states[i].qr = request->states().qr(i);
            server_inputs.states[i].qi = request->states().qi(i);
            server_inputs.states[i].qj = request->states().qj(i);
            server_inputs.states[i].qk = request->states().qk(i);
        }
        server_inputs.requested_output = std::vector<std::string>(request->requested_output().begin(),
                                                                  request->requested_output().end());
    }
    return server_inputs;
}

grpc::Status to_grpc(grpc::ServerContext* , const YamlState& state_derivatives, ModelExchangeResponse* response)
{
    ModelExchangeStateDerivatives* d_dt = new ModelExchangeStateDerivatives();
    d_dt->set_x(state_derivatives.x);
    d_dt->set_y(state_derivatives.y);
    d_dt->set_z(state_derivatives.z);
    d_dt->set_u(state_derivatives.u);
    d_dt->set_v(state_derivatives.v);
    d_dt->set_w(state_derivatives.w);
    d_dt->set_p(state_derivatives.p);
    d_dt->set_q(state_derivatives.q);
    d_dt->set_r(state_derivatives.r);
    d_dt->set_qr(state_derivatives.qr);
    d_dt->set_qi(state_derivatives.qi);
    d_dt->set_qj(state_derivatives.qj);
    d_dt->set_qk(state_derivatives.qk);
    d_dt->set_phi(state_derivatives.phi);
    d_dt->set_theta(state_derivatives.theta);
    d_dt->set_psi(state_derivatives.psi);
    d_dt->set_t(state_derivatives.t);
    response->set_allocated_d_dt(d_dt);
    for(const auto& obs:state_derivatives.extra_observations)
    {
        response->mutable_extra_observations()->operator[](obs.first) = obs.second;
    }
    return grpc::Status::OK;
}

grpc::Status ModelExchangeServiceImpl::dx_dt_euler_321(
        grpc::ServerContext* context,
        const ModelExchangeRequestEuler* request,
        ModelExchangeResponse* response)
{
    return dx_dt(error, context, request, response);
}

grpc::Status ModelExchangeServiceImpl::dx_dt_quaternion(
        grpc::ServerContext* context,
        const ModelExchangeRequestQuaternion* request,
        ModelExchangeResponse* response)
{
    return dx_dt(error, context, request, response);
}

template <> grpc::Status check_states_size<ModelExchangeRequestEuler>(ErrorOutputter& error, const ModelExchangeRequestEuler* request)
{
    return check_euler_states_size(error, request);
}

template <> grpc::Status check_states_size<ModelExchangeRequestQuaternion>(ErrorOutputter& error, const ModelExchangeRequestQuaternion* request)
{
    return check_quaternion_states_size(error, request);
}
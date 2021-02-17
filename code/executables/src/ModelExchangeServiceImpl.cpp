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

ModelExchangeServiceImpl::ModelExchangeServiceImpl(const XdynForME& xdyn_):
xdyn(xdyn_)
{}

#define SIZE size()
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define STATE_SIZE(state) EVALUATOR(state, SIZE)
#define CHECK_SIZE(state) if (states.STATE_SIZE(state) != states.t_size())\
{\
    const size_t n1 = states.STATE_SIZE(state);\
    const size_t n2 = states.t_size();\
    msg << "State '" << #state << "' has size " << n1 << ", whereas 't' has size " << n2 << ": this is a problem in the client code (caller of xdyn's gRPC server), not a problem with xdyn. Please ensure that '" << #state << "' and 't' have the same size in CosimulationRequest's 'States' type." << std::endl;\
}

grpc::Status check_states_size(const ModelExchangeRequestEuler* request);
grpc::Status check_states_size(const ModelExchangeRequestEuler* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const ModelExchangeStatesEuler states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(phi);
        CHECK_SIZE(theta);
        CHECK_SIZE(psi);
    }
    if (msg.str().empty())
    {
        return grpc::Status::OK;
    }
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, msg.str());
}

grpc::Status check_states_size(const ModelExchangeRequestQuaternion* request);
grpc::Status check_states_size(const ModelExchangeRequestQuaternion* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const ModelExchangeStatesQuaternion states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(qr);
        CHECK_SIZE(qi);
        CHECK_SIZE(qj);
        CHECK_SIZE(qk);
    }
    if (msg.str().empty())
    {
        return grpc::Status::OK;
    }
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, msg.str());
}

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestEuler* request);
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
        server_inputs.requested_output.clear();
        for (int i = 0 ; i < request->requested_output_size() ; i++)
        {
            server_inputs.requested_output.push_back(request->requested_output(i));
        }
    }
    return server_inputs;
}

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestQuaternion* request);
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
        server_inputs.requested_output.clear();
        for (int i = 0 ; i < request->requested_output_size() ; i++)
        {
            server_inputs.requested_output.push_back(request->requested_output(i));
        }
    }
    return server_inputs;
}

grpc::Status to_grpc(grpc::ServerContext* context, const YamlState& state_derivatives, ModelExchangeResponse* response);
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
    for(auto obs:state_derivatives.extra_observations)
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
    const grpc::Status precond = check_states_size(request);
    if (not precond.ok())
    {
        return precond;
    }
    const SimServerInputs inputs(from_grpc(context, request), xdyn.get_Tmax());
    const YamlState output = xdyn.handle(inputs);
    const grpc::Status postcond = to_grpc(context, output, response);
    return postcond;
}

grpc::Status ModelExchangeServiceImpl::dx_dt_quaternion(
        grpc::ServerContext* context,
        const ModelExchangeRequestQuaternion* request,
        ModelExchangeResponse* response)
{
    const grpc::Status precond = check_states_size(request);
    if (not precond.ok())
    {
        return precond;
    }
    const YamlSimServerInputs inputs = from_grpc(context, request);
    if (inputs.states.empty())
    {
        return grpc::Status(grpc::StatusCode::INTERNAL, "We didn't get any states as input (inputs.states is empty): we need at least one to set the initial conditions. This error was detected in ModelExchangeServiceImpl::dx_dt_quaternion");
    }
    const YamlState output = xdyn.handle(SimServerInputs(inputs, xdyn.get_Tmax()));
    const grpc::Status postcond = to_grpc(context, output, response);
    return postcond;
}

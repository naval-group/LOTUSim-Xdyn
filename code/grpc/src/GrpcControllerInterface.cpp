#include "GrpcControllerInterface.hpp"
#include "InvalidInputException.hpp"
#include "yaml.h"
#include <grpcpp/grpcpp.h>
#include "controller.pb.h"
#include "controller.grpc.pb.h"
#include <ssc/kinematics.hpp>

GrpcControllerInterface::Input::Input () : url (), name (), yaml () {}

GrpcSetParametersResponse::GrpcSetParametersResponse ()
    : date_of_first_callback (), setpoint_names (), angle_representation (),
      has_extra_observations ()
{
}

GrpcControllerResponse::GrpcControllerResponse () : commands (), next_call ()
{
}

GrpcControllerResponse convert_controller_response(const ControllerResponse& response);
GrpcControllerResponse convert_controller_response(const ControllerResponse& response)
{
    GrpcControllerResponse ret;
    ret.next_call = response.next_call();
    ret.commands = std::map<std::string, double>(response.commands().begin(),response.commands().end());
    return ret;
}


SetParametersRequest build_set_parameters_request(const std::string& yaml, const double t0);
SetParametersRequest build_set_parameters_request(const std::string& yaml, const double t0)
{
    SetParametersRequest ret;
    ret.set_parameters(yaml);
    ret.set_t0(t0);
    return ret;
}


ControllerStatesQuaternion* build_quaternion_state(const double t, const std::vector<double>& states);
ControllerStatesQuaternion* build_quaternion_state(const double t, const std::vector<double>& states)
{
    ControllerStatesQuaternion* ret = new ControllerStatesQuaternion();
    ret->set_t(t);
    ret->set_x(states[0]);
    ret->set_y(states[1]);
    ret->set_z(states[2]);
    ret->set_u(states[3]);
    ret->set_v(states[4]);
    ret->set_w(states[5]);
    ret->set_p(states[6]);
    ret->set_q(states[7]);
    ret->set_r(states[8]);
    ret->set_qr(states[9]);
    ret->set_qi(states[10]);
    ret->set_qj(states[11]);
    ret->set_qk(states[12]);
    return ret;
}

ControllerRequestQuaternion build_quaternion_request(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints);
ControllerRequestQuaternion build_quaternion_request(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
{
    ControllerRequestQuaternion ret;
    ret.set_allocated_states(build_quaternion_state(t, states));
    ret.set_allocated_dstates_dt(build_quaternion_state(t, dstates_dt));
    *ret.mutable_setpoints() = {setpoints.begin(), setpoints.end()};
    return ret;
}

ControllerStatesEuler* build_euler321_state(const double t, const std::vector<double>& states);
ControllerStatesEuler* build_euler321_state(const double t, const std::vector<double>& states)
{
    using namespace ssc::kinematics;
    ControllerStatesEuler* ret = new ControllerStatesEuler();
    ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(states[9],states[10],states[11],states[12]).matrix();
    const ssc::kinematics::EulerAngles angles = euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
    ret->set_t(t);
    ret->set_x(states[0]);
    ret->set_y(states[1]);
    ret->set_z(states[2]);
    ret->set_u(states[3]);
    ret->set_v(states[4]);
    ret->set_w(states[5]);
    ret->set_p(states[6]);
    ret->set_q(states[7]);
    ret->set_r(states[8]);
    ret->set_phi(angles.phi);
    ret->set_theta(angles.theta);
    ret->set_psi(angles.psi);
    return ret;
}

ControllerRequestEuler build_euler321_request(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints);
ControllerRequestEuler build_euler321_request(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
{
    ControllerRequestEuler ret;
    ret.set_allocated_states(build_euler321_state(t, states));
    ret.set_allocated_dstates_dt(build_euler321_state(t, dstates_dt));
    *ret.mutable_setpoints() = {setpoints.begin(), setpoints.end()};
    return ret;
}


struct GrpcControllerInterface::Impl
{
    Impl(const GrpcControllerInterface::Input& input)
    : stub(Controller::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
    , yaml(input.yaml)
    {}

    GrpcSetParametersResponse set_parameters(const double t0)
    {
        SetParametersResponse response;
        grpc::ClientContext context;
        const grpc::Status status = stub->set_parameters(&context, build_set_parameters_request(yaml, t0), &response);
        GrpcSetParametersResponse ret;
        switch(response.angle_representation())
        {
            case SetParametersResponse_AngleRepresentation::SetParametersResponse_AngleRepresentation_EULER_321:
                ret.angle_representation = AngleRepresentation::EULER_321;
                break;
            case SetParametersResponse_AngleRepresentation::SetParametersResponse_AngleRepresentation_QUATERNION:
                ret.angle_representation = AngleRepresentation::QUATERNION;
                break;
            case SetParametersResponse_AngleRepresentation::SetParametersResponse_AngleRepresentation_SetParametersResponse_AngleRepresentation_INT_MIN_SENTINEL_DO_NOT_USE_:
                break;
            case SetParametersResponse_AngleRepresentation::SetParametersResponse_AngleRepresentation_SetParametersResponse_AngleRepresentation_INT_MAX_SENTINEL_DO_NOT_USE_:
                break;
        }
        ret.date_of_first_callback = response.date_of_first_callback();
        ret.has_extra_observations = response.has_extra_observations();
        ret.setpoint_names.reserve(response.setpoint_names_size());
        std::copy(response.setpoint_names().begin(), response.setpoint_names().end(), std::back_inserter(ret.setpoint_names));
        ret.dt = response.dt();
        return ret;
    }

    GrpcControllerResponse get_commands_quaternion(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
    {
        ControllerResponse response;
        grpc::ClientContext context;
        const ControllerRequestQuaternion request = build_quaternion_request(t, states, dstates_dt, setpoints);
        const grpc::Status status = stub->get_commands_quaternion(&context, request, &response);
        return convert_controller_response(response);
    }

    GrpcControllerResponse get_commands_euler321(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
    {
        ControllerResponse response;
        grpc::ClientContext context;
        const ControllerRequestEuler request = build_euler321_request(t, states, dstates_dt, setpoints);
        const grpc::Status status = stub->get_commands_euler_321(&context, request, &response);
        return convert_controller_response(response);
    }

    std::map<std::string, double> get_extra_observations()
    {
        ExtraObservationsResponse response;
        grpc::ClientContext context;
        const grpc::Status status = stub->get_extra_observations(&context, ExtraObservationsRequest(), &response);
        return std::map<std::string, double>(response.extra_observations().begin(), response.extra_observations().end());
    }


    private:
        std::unique_ptr<Controller::Stub> stub;
        std::string yaml;
};


GrpcControllerInterface::GrpcControllerInterface(const std::shared_ptr<GrpcControllerInterface::Impl>& pimpl_, const GrpcSetParametersResponse& set_parameters_response_) :
pimpl(pimpl_),
set_parameters_response(set_parameters_response_)
{}

GrpcControllerInterface::~GrpcControllerInterface()
{}

double GrpcControllerInterface::get_dt() const
{
    return set_parameters_response.dt;
}

bool GrpcControllerInterface::has_extra_observations() const
{
    return set_parameters_response.has_extra_observations;
}

std::vector<std::string> GrpcControllerInterface::get_setpoint_names() const
{
    return set_parameters_response.setpoint_names;
}

std::shared_ptr<GrpcControllerInterface> GrpcControllerInterface::build(const GrpcControllerInterface::Input& input, const double t0)
{
    std::shared_ptr<GrpcControllerInterface::Impl> pimpl(new GrpcControllerInterface::Impl(input));
    const GrpcSetParametersResponse res = pimpl->set_parameters(t0);
    switch(res.angle_representation)
    {
        case AngleRepresentation::EULER_321:
            return std::shared_ptr<GrpcControllerInterface>(new Euler321GrpcController(pimpl, res));
            break;
        case AngleRepresentation::QUATERNION:
            return std::shared_ptr<GrpcControllerInterface>(new QuaternionGrpcController(pimpl, res));
            break;
        default:
            return std::shared_ptr<GrpcControllerInterface>(new Euler321GrpcController(pimpl, res));
            break;
    }
    return std::shared_ptr<GrpcControllerInterface>(new Euler321GrpcController(pimpl, res));
}

GrpcControllerInterface::Input
GrpcControllerInterface::parse (const std::string &yaml)
{
    std::stringstream stream (yaml);
    std::stringstream ss;
    YAML::Parser parser (stream);
    YAML::Node node;
    parser.GetNextDocument (node);
    GrpcControllerInterface::Input ret;
    try
    {
        node["url"] >> ret.url;
        node["name"] >> ret.name;
        YAML::Emitter out;
        out << node;
        ret.yaml = out.c_str();
    }
    catch (YAML::Exception &e)
    {
        THROW (__PRETTY_FUNCTION__, InvalidInputException,
               "Unable to parse YAML data for a gRPC controller:"
                   << std::endl
                   << e.what () << std::endl
                   << "The offending YAML block was:" << std::endl
                   << yaml);
    }
    return ret;
}


GrpcControllerResponse QuaternionGrpcController::get_commands(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
{
    return pimpl->get_commands_quaternion(t, states, dstates_dt, setpoints);
}

GrpcControllerResponse Euler321GrpcController::get_commands(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
{
    return pimpl->get_commands_euler321(t, states, dstates_dt, setpoints);
}

Euler321GrpcController::Euler321GrpcController(const std::shared_ptr<Impl>& pimpl_, const GrpcSetParametersResponse& set_parameters_response) : GrpcControllerInterface(pimpl_, set_parameters_response)
{}

QuaternionGrpcController::QuaternionGrpcController(const std::shared_ptr<Impl>& pimpl_, const GrpcSetParametersResponse& set_parameters_response) : GrpcControllerInterface(pimpl_, set_parameters_response)
{}

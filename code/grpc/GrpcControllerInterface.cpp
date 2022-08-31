#include "GrpcControllerInterface.hpp"
#include "InvalidInputException.hpp"
#include "yaml.h"
#include <grpcpp/grpcpp.h>
#include "controller.pb.h"
#include "controller.grpc.pb.h"
#include <ssc/kinematics.hpp>
#include "grpc_error_outputter.hpp"
#include "InternalErrorException.hpp"

template <> std::string get_type_of_service<GrpcControllerInterface>()
{
    return "controller";
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
    try
    {
        ret->set_t(t);
        ret->set_x(states.at(0));
        ret->set_y(states.at(1));
        ret->set_z(states.at(2));
        ret->set_u(states.at(3));
        ret->set_v(states.at(4));
        ret->set_w(states.at(5));
        ret->set_p(states.at(6));
        ret->set_q(states.at(7));
        ret->set_r(states.at(8));
        ret->set_qr(states.at(9));
        ret->set_qi(states.at(10));
        ret->set_qj(states.at(11));
        ret->set_qk(states.at(12));
    }
    catch(const std::out_of_range& e)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "When trying to convert (quaternion) states before sending to gRPC controller: " << e.what());
    }
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
    try
    {
        ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(states.at(9),states.at(10),states.at(11),states.at(12)).matrix();
        const ssc::kinematics::EulerAngles angles = euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
        ret->set_t(t);
        ret->set_x(states.at(0));
        ret->set_y(states.at(1));
        ret->set_z(states.at(2));
        ret->set_u(states.at(3));
        ret->set_v(states.at(4));
        ret->set_w(states.at(5));
        ret->set_p(states.at(6));
        ret->set_q(states.at(7));
        ret->set_r(states.at(8));
        ret->set_phi(angles.phi);
        ret->set_theta(angles.theta);
        ret->set_psi(angles.psi);
    }
    catch(const std::out_of_range& e)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "When trying to convert (euler321) states before sending to gRPC controller: " << e.what());
    }
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

/*
GrpcSetParametersResponse::GrpcSetParametersResponse():
    date_of_first_callback(0.0),
    angle_representation(AngleRepresentation::QUATERNION),
    dt(0.0),
    command_names()
{}
*/
GrpcControllerResponse::GrpcControllerResponse(): commands(), next_call(0.0){}

GrpcControllerInterface::Input::Input ():url(), name(), yaml(){}

struct GrpcControllerInterface::Impl
{
    Impl(const GrpcControllerInterface::Input& input_)
    : input(input_)
    , stub(Controller::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
    {}

    void set_connection_timeout(grpc::ClientContext& context)
    {
        // Timeout in seconds
        const int timeout = 5;
        // Calculate deadline from connection timeout in seconds
        std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(timeout);
        context.set_deadline(deadline);
    }

    GrpcSetParametersResponse set_parameters(const double t0)
    {
        SetParametersResponse response;
        grpc::ClientContext context;
        set_connection_timeout(context);
        const grpc::Status status = stub->set_parameters(&context, build_set_parameters_request(input.yaml, t0), &response);
        throw_if_invalid_status<Input,GrpcControllerInterface>(input, "set_parameters", status);
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
        ret.setpoint_names.reserve(static_cast<size_t>(response.setpoint_names_size()));
        std::copy(response.setpoint_names().begin(), response.setpoint_names().end(), std::back_inserter(ret.setpoint_names));
        ret.command_names.reserve(static_cast<size_t>(response.command_names_size()));
        std::copy(response.command_names().begin(), response.command_names().end(), std::back_inserter(ret.command_names));
        ret.dt = response.dt();
        if (ret.dt <= 0)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "When calling the gRPC controller '" << input.name << "', the controller gave us a timestep dt of " << ret.dt << ": we expect a strictly positive value here. To fix this, ensure the gRPC controller's set_parameters method returns a strictly positive value for dt.");
        }
        return ret;
    }

    GrpcControllerResponse get_commands_quaternion(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
    {
        ControllerResponse response;
        grpc::ClientContext context;
        const ControllerRequestQuaternion request = build_quaternion_request(t, states, dstates_dt, setpoints);
        const grpc::Status status = stub->get_commands_quaternion(&context, request, &response);
        throw_if_invalid_status<Input,GrpcControllerInterface>(input, "get_commands_quaternion", status);
        return convert_controller_response(response);
    }

    GrpcControllerResponse get_commands_euler321(const double t, const std::vector<double>& states, const std::vector<double>& dstates_dt, const std::vector<double>& setpoints)
    {
        ControllerResponse response;
        grpc::ClientContext context;
        const ControllerRequestEuler request = build_euler321_request(t, states, dstates_dt, setpoints);
        const grpc::Status status = stub->get_commands_euler_321(&context, request, &response);
        throw_if_invalid_status<Input,GrpcControllerInterface>(input, "get_commands_euler_321", status);
        return convert_controller_response(response);
    }

    private:
        GrpcControllerInterface::Input input;
        std::unique_ptr<Controller::Stub> stub;
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

std::vector<std::string> GrpcControllerInterface::get_setpoint_names() const
{
    return set_parameters_response.setpoint_names;
}

std::vector<std::string> GrpcControllerInterface::get_command_names() const
{
    return set_parameters_response.command_names;
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

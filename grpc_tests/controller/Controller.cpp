#include <cmath>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include "controller.grpc.pb.h"
#include "controller.pb.h"
#include "Controller.hpp"
#include "FromGrpc.hpp"
#include "ToGrpc.hpp"


Controller_::Controller_(const double dt_) : dt(dt_), t0(0)
{}

EulerController_::EulerController_(const double dt_) : Controller_(dt_)
{}

QuaternionController_::QuaternionController_(const double dt_) : Controller_(dt_)
{}

double Controller_::get_dt() const
{
    return dt;
}

double Controller_::get_t0() const
{
    return t0;
}

double Controller_::get_date_of_next_call(const double t) const
{
    const int current_index = (int)std::round((t - t0) / dt);
    return t0 + (current_index + 1) * dt;
}

std::map<std::string, double> Controller_::get_extra_observations() const
{
    return std::map<std::string, double>();
}

void Controller_::set_t0(const double t0_)
{
    t0 = t0_;
}

std::map<std::string,double> EulerController_::get_commands_quaternion(const QuaternionStates_&, const QuaternionStates_&, const std::vector<double>&)
{
    return std::map<std::string, double>();
}

AngleRepresentation_ EulerController_::get_angle_representation() const
{
    return AngleRepresentation_::EULER_321;
}

AngleRepresentation_ QuaternionController_::get_angle_representation() const
{
    return AngleRepresentation_::QUATERNION;
}

std::map<std::string,double> QuaternionController_::get_commands_euler_321(const EulerStates_&, const EulerStates_&, const std::vector<double>&)
{
    return std::map<std::string, double>();
}

class ControllerServiceImpl final : public Controller::Service
{
    public:
        ControllerServiceImpl() = delete;
        ControllerServiceImpl(Controller_* controller_) : controller(controller_), to_grpc(), from_grpc()
        {}
        grpc::Status set_parameters(grpc::ServerContext* , const SetParametersRequest* request, SetParametersResponse* reply) override
        {
            SetParametersResponse_ response = controller->set_parameters(request->parameters());
            controller->set_t0(request->t0());
            reply->set_date_of_first_callback(controller->get_date_of_next_call(request->t0()));
            to_grpc.copy_from_string_vector(response.setpoint_names, reply->mutable_setpoint_names());
            reply->set_has_extra_observations(not(controller->get_extra_observations().empty()));
            return grpc::Status::OK;
        }

        grpc::Status get_commands_quaternion(grpc::ServerContext* , const ControllerRequestQuaternion* request, ControllerResponse* reply) override
        {
            const QuaternionStates_ states = from_grpc.to_quaternion_states(request->states());
            const QuaternionStates_ dstates_dt = from_grpc.to_quaternion_states(request->dstates_dt());
            std::vector<double> setpoints;
            std::copy(request->setpoints().begin(), request->setpoints().end(), std::back_inserter(setpoints));
            const std::map<std::string,double> commands = controller->get_commands_quaternion(states, dstates_dt, setpoints);
            reply->mutable_commands()->insert(commands.begin(), commands.end());
            reply->set_next_call(controller->get_date_of_next_call(states.t));
            return grpc::Status::OK;
        }

        grpc::Status get_commands_euler_321(grpc::ServerContext* , const ControllerRequestEuler* request, ControllerResponse* reply) override
        {
            const EulerStates_ states = from_grpc.to_euler_states(request->states());
            const EulerStates_ dstates_dt = from_grpc.to_euler_states(request->dstates_dt());
            std::vector<double> setpoints;
            std::copy(request->setpoints().begin(), request->setpoints().end(), std::back_inserter(setpoints));
            const std::map<std::string,double> commands = controller->get_commands_euler_321(states, dstates_dt, setpoints);
            reply->mutable_commands()->insert(commands.begin(), commands.end());
            reply->set_next_call(controller->get_date_of_next_call(states.t));
            return grpc::Status::OK;
        }

        grpc::Status get_extra_observations(grpc::ServerContext* , const ExtraObservationsRequest* , ExtraObservationsResponse* reply) override
        {
            const std::map<std::string, double> extra_observations = controller->get_extra_observations();
            reply->mutable_extra_observations()->insert(extra_observations.begin(), extra_observations.end());
            return grpc::Status::OK;
        }

    private:
        Controller_* controller;
        ToGrpc to_grpc;
        FromGrpc from_grpc;
};



void run(Controller_& controller)
{
    std::string server_address("0.0.0.0:9002");
    ControllerServiceImpl service(&controller);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}


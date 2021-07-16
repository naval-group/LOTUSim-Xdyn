/*
 * GRPCForceModel.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#include <memory> // std::make_shared
#include <vector>
#include "grpc_error_outputter.hpp"
#include "force.pb.h"
#include "force.grpc.pb.h"

#include "yaml.h"
#include <ssc/macros.hpp>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>


#include "Body.hpp"
#include "GRPCForceModel.hpp"
#include "GRPCTypes.hpp"
#include "ToGRPC.hpp"
#include "FromGRPC.hpp"

template <> std::string get_type_of_service<GRPCForceModel>()
{
    return "force model";
}

class GRPCForceModel::Impl
{
    public:
        Impl(const GRPCForceModel::Input& input_, const std::vector<std::string>& rotation_convention_, const std::string& body_name)
            : input(input_)
            , stub(Force::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
            , extra_observations()
            , max_history_length()
            , needs_wave_outputs()
            , rotation_convention(rotation_convention_)
            , to_grpc(ToGRPC(input))
            , from_grpc(FromGRPC())
            , commands()
            , force_frame()
            , needs_results_from_potential_theory()
        {
            set_parameters(input.yaml, body_name, input.name);
            if (needs_results_from_potential_theory)
            {

            }
        }

        GRPCForceModel::Input get_input() const
        {
            return input;
        }

        void set_parameters(const std::string& yaml, const std::string& body_name, const std::string& instance_name)
        {
            SetForceParameterResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->set_parameters(&context, to_grpc.from_yaml(yaml, body_name, instance_name), &response);
            throw_if_invalid_status<Input,GRPCForceModel>(input, "set_parameters", status);
            needs_wave_outputs = response.needs_wave_outputs();
            max_history_length = response.max_history_length();
            commands.reserve(response.commands_size());
            std::copy(response.commands().begin(), response.commands().end(), std::back_inserter(commands));
            force_frame.frame = response.frame();
            force_frame.angle.phi = response.phi();
            force_frame.angle.theta = response.theta();
            force_frame.angle.psi = response.psi();
            force_frame.coordinates.x = response.x();
            force_frame.coordinates.y = response.y();
            force_frame.coordinates.z = response.z();
        }

        WaveRequest required_wave_information(const double t, const double x, const double y, const double z) const
        {
            const RequiredWaveInformationRequest required_wave_information_request = to_grpc.from_required_wave_information(t, x, y, z, input.name);
            RequiredWaveInformationResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->required_wave_information(&context, required_wave_information_request, &response);
            throw_if_invalid_status<Input,GRPCForceModel>(input, "required_wave_information", status);
            return from_grpc.to_wave_request(response);
        }

        ssc::kinematics::Vector6d force(const double t, const BodyStates& state, const std::map<std::string,double>& commands, const EnvironmentAndFrames& env, const std::string& instance_name)
        {
            ForceResponse response;
            grpc::ClientContext context;
            const auto states = to_grpc.from_state(state, max_history_length, env);
            const auto wave_information = get_wave_information(t, state.x(0), state.y(0), state.z(0), env);
            const grpc::Status status = stub->force(&context, to_grpc.from_force_request(states, commands, wave_information, instance_name), &response);
            throw_if_invalid_status<Input,GRPCForceModel>(input, "force", status);
            extra_observations = std::map<std::string,double>(response.extra_observations().begin(),response.extra_observations().end());
            return from_grpc.to_force(response);
        }

        double get_Tmax() const
        {
            return max_history_length;
        }

        std::map<std::string,double> get_extra_observations() const
        {
            return extra_observations;
        }

        std::vector<std::string> get_commands() const
        {
            return commands;
        }

        YamlPosition get_transformation_to_model_frame() const
        {
            return force_frame;
        }

    private:
        Impl(); // Disabled
        WaveInformation* get_wave_information(const double t, const double x, const double y, const double z, const EnvironmentAndFrames& env) const
        {
            if (needs_wave_outputs)
            {
                const WaveRequest wave_request = required_wave_information(t, x, y, z);
                return to_grpc.from_wave_information(wave_request, t, env);
            }
            return new WaveInformation();
        }
        GRPCForceModel::Input input;
        std::unique_ptr<Force::Stub> stub;
        std::map<std::string,double> extra_observations;
        double max_history_length;
        bool needs_wave_outputs;
        std::vector<std::string> rotation_convention;
        ToGRPC to_grpc;
        FromGRPC from_grpc;
        std::vector<std::string> commands;
        YamlPosition force_frame;
        bool needs_results_from_potential_theory;
};

std::string GRPCForceModel::model_name() {return "grpc";}


GRPCForceModel::Input GRPCForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    GRPCForceModel::Input ret;
    node["url"] >> ret.url;
    node["name"] >> ret.name;
    YAML::Emitter out;
    out << node;
    ret.yaml = out.c_str();
    return ret;
}

GRPCForceModel::GRPCForceModel(const GRPCForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& env) :
        GRPCForceModel(TR1(shared_ptr)<GRPCForceModel::Impl>(new GRPCForceModel::Impl(input, env.rot.convention, body_name_)), body_name_, env)
{
}

GRPCForceModel::GRPCForceModel(const TR1(shared_ptr)<Impl>& pimpl_, const std::string& body_name_, const EnvironmentAndFrames& env) :
        ForceModel(pimpl_->get_input().name, pimpl_->get_commands(), pimpl_->get_transformation_to_model_frame(), body_name_, env),
        pimpl(pimpl_)

{
}

Wrench GRPCForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    const auto ret = Wrench(ssc::kinematics::Point(name,0,0,0), name, pimpl->force(t, states, commands, env, get_name()));
    return ret;
}

void GRPCForceModel::extra_observations(Observer& observer) const
{
    const auto extra_observations = pimpl->get_extra_observations();
    for (const auto observation : extra_observations)
    {
        observer.write(observation.second, DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),observation.first},observation.first + std::string("(") + get_body_name() + ")"));
    }
}

double GRPCForceModel::get_Tmax() const
{
    return pimpl->get_Tmax();
}

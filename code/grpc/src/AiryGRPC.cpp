#include "AiryGRPC.hpp"
// #include "ErrorReporter.hpp"
// #include "gRPCChecks.hpp"
#include "wave_types.pb.h"
#include "wave_types.grpc.pb.h"
#include "wave_grpc.grpc.pb.h"
#include "wave_grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <string>
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;

// Forward declaration
// `get_environment` will parse the input YAML data and build the EnvironmentAndFrames object
EnvironmentAndFrames get_environment(const std::string& yaml_data);
// Because run_and_report_errors_as_gRPC_status is not present in libxdyn
grpc::Status run_and_report_errors_as_gRPC_status(const std::function<void(void)>& f);

class WavesImpl final : public Waves::Service {
    public:
        explicit WavesImpl(const EnvironmentAndFrames& _env): env(_env) {}

        // OK rpc set_parameters(SetParameterRequest)                    returns (SetParameterResponse);
        // OK rpc elevations(XYTGrid)                                    returns (XYZTGrid);
        // OK rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
        // OK rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
        // OK rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
        // OK rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
        // OK rpc directions_for_rao(DirectionsRequest)                  returns (DirectionsResponse);

        // rpc elevations(XYTGrid) returns (XYZTGrid);
        Status set_parameters(ServerContext* /*context*/, const SetParameterRequest* request, SetParameterResponse* reply) override
        {
            const std::function<void()> f = [this, &request]()
            {
                const std::string yaml_data(request->parameters());
                std::cout <<yaml_data<<std::endl;
                this->env = get_environment(yaml_data);
            };
            reply->clear_error_message();
            return run_and_report_errors_as_gRPC_status(f);
        }

        // rpc elevations(XYTGrid) returns (XYZTGrid);
        Status elevations(ServerContext* /*context*/, const XYTGrid* request, XYZTGrid* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(res_elevations[index]);
            }
            return Status::OK;
        }

        // rpc dynamic_pressures(XYZTGrid) returns (DynamicPressuresResponse);
        Status dynamic_pressures(ServerContext* /*context*/, const XYZTGrid* request, DynamicPressuresResponse* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->clear_pdyn();
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> vec_z(request->z().begin(), request->z().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            const std::vector<double> res_pdyn = env.w->get_and_check_dynamic_pressure(
                env.rho, env.g, vec_x, vec_y, vec_z, res_elevations, request->t());
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(request->z(index));
                reply->add_pdyn(res_pdyn[index]);
            }
            return Status::OK;
        }

        // rpc orbital_velocities(XYZTGrid) returns (OrbitalVelocitiesResponse);
        Status orbital_velocities(ServerContext* /*context*/, const XYZTGrid* request, OrbitalVelocitiesResponse* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->clear_vx();
            reply->clear_vy();
            reply->clear_vz();
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> vec_z(request->z().begin(), request->z().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            const ssc::kinematics::PointMatrix res_velocities = env.w->get_and_check_orbital_velocity(
                env.g, vec_x, vec_y, vec_z, request->t(), res_elevations);
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(request->z(index));
                reply->add_vx(res_velocities.m(0, index));
                reply->add_vy(res_velocities.m(1, index));
                reply->add_vz(res_velocities.m(2, index));
            }
            return Status::OK;
        }

        // rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
        Status angular_frequencies_for_rao(ServerContext* /*context*/, const AngularFrequenciesRequest* /*request*/, AngularFrequenciesResponse* reply) override
        {
            reply->clear_angular_frequencies();
            std::vector<std::vector<double> > res = env.w->get_wave_angular_frequency_for_each_model();
            for (size_t index = 0; index < res.size(); ++index)
            {
                AngularFrequencies * g = reply->add_angular_frequencies();
                google::protobuf::RepeatedField<double> field{res[index].begin(), res[index].end()};
                g->mutable_omegas()->Swap(&field);

            }
            return Status::OK;
        }

        // rpc directions_for_rao(DirectionsRequest) returns (DirectionsResponse);
        Status directions_for_rao(ServerContext* /*context*/, const DirectionsRequest* /*request*/, DirectionsResponse* reply) override
        {
            reply->clear_directions();
            const std::vector<std::vector<double> > res = env.w->get_wave_directions_for_each_model();
            for (size_t index = 0; index < res.size(); ++index)
            {
                Directions * g = reply->add_directions();
                google::protobuf::RepeatedField<double> field{res[index].begin(), res[index].end()};
                g->mutable_psis()->Swap(&field);
            }
            return Status::OK;
        }

        // rpc spectrum(SpectrumRequest) returns (SpectrumResponse);
        Status spectrum(ServerContext* /*context*/, const SpectrumRequest* request, SpectrumResponse* spectrum_response) override
        {
            spectrum_response->clear_spectrum();
            const std::vector<DiscreteDirectionalWaveSpectrum> spectra = env.w->get_directional_spectra(request->x(), request->y(), request->t());
            for (const auto& spectrum:spectra)
            {
                const auto s = spectrum_response->add_spectrum();
                for (const auto& Si:spectrum.Si)
                {
                    s->add_si(Si);
                }
                for (const auto& Dj:spectrum.Dj)
                {
                    s->add_dj(Dj);
                }
                for (const auto& omega:spectrum.omega)
                {
                    s->add_omega(omega);
                }
                for (const auto& psi:spectrum.psi)
                {
                    s->add_psi(psi);
                }
                for (const auto& k:spectrum.k)
                {
                    s->add_k(k);
                }
                for (const auto& phases:spectrum.phase)
                {
                    const auto p = s->add_phase();
                    for (const auto& phase:phases)
                    {
                        p->add_phase(phase);
                    }
                }
            }
            return Status::OK;
        }
    private:
        EnvironmentAndFrames env;
};

void run_xdyn_airy_server(const EnvironmentAndFrames& env)
{
    const std::string server_address("0.0.0.0:50051");
    WavesImpl service(env);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

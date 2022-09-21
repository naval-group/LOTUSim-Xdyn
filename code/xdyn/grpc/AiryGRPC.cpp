#include "AiryGRPC.hpp"
#include "ToGRPCCommon.hpp"
#include "wave_types.pb.h"
#include "wave_types.grpc.pb.h"
#include "wave_grpc.grpc.pb.h"
#include "wave_grpc.pb.h"
#include "xdyn/core/EnvironmentAndFrames.hpp"
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
// `get_environment_for_wave_queries` will parse the input YAML data and build the EnvironmentAndFrames object
EnvironmentAndFrames get_environment_for_wave_queries(const std::string& yaml_data);
// Because run_and_report_errors_as_gRPC_status is not present in libxdyn
grpc::Status run_and_report_errors_as_gRPC_status(const std::function<void(void)>& f);

class WavesImpl final : public Waves::Service {
    public:
        explicit WavesImpl(const EnvironmentAndFrames& _env): env(_env) {}

        // rpc set_parameters(SetParameterRequest) returns (SetParameterResponse);
        Status set_parameters(ServerContext* /*context*/, const SetParameterRequest* request, SetParameterResponse* /*reply*/) override
        {
            const std::function<void()> f = [this, &request]()
            {
                const std::string yaml_data(request->parameters());
                std::cout << "Receiving this input YAML:" << std::endl;
                std::cout << yaml_data << std::endl;
                this->env = get_environment_for_wave_queries(yaml_data);
            };
            return run_and_report_errors_as_gRPC_status(f);
        }

        // rpc elevations(XYTGrid) returns (XYZTGrid);
        Status elevations(ServerContext* /*context*/, const XYTGrid* request, XYZTGrid* reply) override
        {
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            for (int index = 0; index < request->x_size(); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(res_elevations.at(static_cast<size_t>(index)));
            }
            return Status::OK;
        }

        // rpc dynamic_pressures(XYZTGrid) returns (DynamicPressuresResponse);
        Status dynamic_pressures(ServerContext* /*context*/, const XYZTGrid* request, DynamicPressuresResponse* reply) override
        {
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> vec_z(request->z().begin(), request->z().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            const std::vector<double> res_pdyn = env.w->get_and_check_dynamic_pressure(
                env.rho, env.g, vec_x, vec_y, vec_z, res_elevations, request->t());
            for (int index = 0; index < request->x_size(); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(request->z(index));
                reply->add_pdyn(res_pdyn.at(static_cast<size_t>(index)));
            }
            return Status::OK;
        }

        // rpc orbital_velocities(XYZTGrid) returns (OrbitalVelocitiesResponse);
        Status orbital_velocities(ServerContext* /*context*/, const XYZTGrid* request, OrbitalVelocitiesResponse* reply) override
        {
            reply->set_t(request->t());
            const std::vector<double> vec_x(request->x().begin(), request->x().end());
            const std::vector<double> vec_y(request->y().begin(), request->y().end());
            const std::vector<double> vec_z(request->z().begin(), request->z().end());
            const std::vector<double> res_elevations = env.w->get_and_check_wave_height(vec_x, vec_y, request->t());
            const ssc::kinematics::PointMatrix res_velocities = env.w->get_and_check_orbital_velocity(
                env.g, vec_x, vec_y, vec_z, request->t(), res_elevations);
            for (int index = 0; index < request->x_size(); ++index)
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

        // rpc spectrum(SpectrumRequest) returns (SpectrumResponse);
        Status spectrum(ServerContext* /*context*/, const SpectrumRequest* request, SpectrumResponse* spectrum_response) override
        {
            const std::vector<FlatDiscreteDirectionalWaveSpectrum> spectra = env.w->get_flat_directional_spectra(request->x(), request->y(), request->t());
            flat_spectrum_response_from_vector_of_flat_discrete_directional_wave_spectra(spectra, spectrum_response);
            return Status::OK;
        }
    private:
        EnvironmentAndFrames env;
};

void run_xdyn_airy_server(const EnvironmentAndFrames& env, const short unsigned int port_number)
{
    const std::string server_address("0.0.0.0:"+std::to_string(port_number));
    WavesImpl service(env);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

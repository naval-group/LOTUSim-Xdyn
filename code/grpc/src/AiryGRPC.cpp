#include "AiryGRPC.hpp"
#include "wave_types.pb.h"
#include "wave_types.grpc.pb.h"
#include "wave_grpc.grpc.pb.h"
#include "wave_grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define PI (4.0 * std::atan(1.0))
#define G 9.81

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
/*
struct XdynAiryCommandLineArguments
{
    XdynAiryCommandLineArguments();
    std::vector<std::string> yaml_filenames;
    std::string output_filename;
    std::string wave_output;
    bool catch_exceptions;
    bool empty() const;
};


int run(const XdynAiryCommandLineArguments& input_data, ErrorReporter& error_outputter);
int run(const XdynAiryCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    if (not(input_data.empty())) run_simulation(input_data, error_outputter);
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
*/
/*
service Waves
{
    rpc set_parameters(SetParameterRequest)                    returns (SetParameterResponse);
    rpc elevations(XYTGrid)                                    returns (XYZTGrid);
    rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
    rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
    rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
    rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
    rpc directions_for_rao(DirectionsRequest)                  returns (DirectionsResponse);
}
*/

// using wave::Point;
// using wave::ElevationRequest;
// using wave::ElevationResponse;
// using wave::ElevationPoint;
// using wave::ElevationRequestRepeated;
// using wave::ElevationResponseRepeated;
// using wave::Waves;
// using wave::Airy;
// using wave::WaveSpectrumLine;

class WavesImpl final : public Waves::Service {
    public:
        explicit WavesImpl(const EnvironmentAndFrames& _env): env(_env) {}

        // KO rpc set_parameters(SetParameterRequest)                    returns (SetParameterResponse);
        // OK rpc elevations(XYTGrid)                                    returns (XYZTGrid);
        // OK rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
        // OK rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
        // KO rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
        // OK rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
        // OK rpc directions_for_rao(DirectionsRequest)                  returns (DirectionsResponse);

        // rpc elevations(XYTGrid)                                    returns (XYZTGrid);
        Status elevations(ServerContext* /*context*/, const XYTGrid* request, XYZTGrid* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->set_t(request->t());
            std::vector<double> vec_x(request->x_size()); std::copy(request->x().begin(), request->x().end(), std::back_inserter(vec_x));
            std::vector<double> vec_y(request->y_size()); std::copy(request->y().begin(), request->y().end(), std::back_inserter(vec_y));
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
            std::vector<double> vec_x(request->x_size()); std::copy(request->x().begin(), request->x().end(), std::back_inserter(vec_x));
            std::vector<double> vec_y(request->y_size()); std::copy(request->y().begin(), request->y().end(), std::back_inserter(vec_y));
            std::vector<double> vec_z(request->z_size()); std::copy(request->z().begin(), request->z().end(), std::back_inserter(vec_z));
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
            std::vector<double> vec_x(request->x_size()); std::copy(request->x().begin(), request->x().end(), std::back_inserter(vec_x));
            std::vector<double> vec_y(request->y_size()); std::copy(request->y().begin(), request->y().end(), std::back_inserter(vec_y));
            std::vector<double> vec_z(request->z_size()); std::copy(request->z().begin(), request->z().end(), std::back_inserter(vec_z));
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
                g->mutable_omegas()->CopyFrom(field);

            }
            return Status::OK;
        }

        // rpc directions_for_rao(DirectionsRequest) returns (DirectionsResponse);
        Status directions_for_rao(ServerContext* /*context*/, const DirectionsRequest* /*request*/, DirectionsResponse* reply) override
        {
            reply->clear_directions();
            std::vector<std::vector<double> > res = env.w->get_wave_directions_for_each_model();
            for (size_t index = 0; index < res.size(); ++index)
            {
                Directions * g = reply->add_directions();
                google::protobuf::RepeatedField<double> field{res[index].begin(), res[index].end()};
                g->mutable_psis()->CopyFrom(field);
            }
            return Status::OK;
        }


        // get_wave_directions_for_each_model
        // get_directional_spectra
        // get_wave_angular_frequency_for_each_model

        /*
            message PhasesForEachFrequency
            {
                repeated double phase = 1; //!< Random phase (should have the same size as psi in SpectrumResponse. In radian.
            }

            message Spectrum
            {
                repeated double Si = 1;                    // Discretized spectral density for each omega (should therefore be the same size as omega) (in s m^2/rad)
                repeated double Dj = 2;                    // Spatial spreading for each psi (should therefore be the same size as psi) (in 1/rad)
                repeated double omega = 3;                 // Angular frequencies the spectrum was discretized at (in rad/s).
                repeated double psi = 4;                   // Directions between 0 & 2pi the spatial spreading was discretized at (in rad)
                repeated double k = 5;                     // Discretized wave number for each frequency (should therefore be the same size as omega) in rad/m.
                repeated PhasesForEachFrequency phase = 6; // Random phases, for each [frequency i][direction j] couple (but time invariant), should have the same size as omega. In radian.
            }

        */
        // rpc spectrum(SpectrumRequest) returns (SpectrumResponse);
        Status spectrum(ServerContext* /*context*/, const SpectrumRequest* /*request*/, SpectrumResponse* reply) override
        {
            reply->clear_spectrum();
            // Spectrum
            // SpectrumResponse
            // reply->clear_si();
            // reply->clear_dj();
            // reply->clear_omega();
            // reply->clear_psi();
            // reply->clear_k();
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

/*
int main(int argc, char** argv)
{
    args::ArgumentParser parser("This is a test grpc server demo program.", "Enjoy.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<std::string> input_use_full_spectrum(parser, "spectrum", "'y' if you wish to use a 128 line discrete wave spectrum, anything else if you want a 1 line one.", {'s', "spectrum"});
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (const args::ValidationError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An internal error has occurred: " << e.what() << std::endl;
        return -1;
    }

    bool use_full_spectrum(false);
    if (input_use_full_spectrum)
    {
      use_full_spectrum = args::get(input_use_full_spectrum) == "y";
      std::cout << "use full wave spectrum: " << (use_full_spectrum ? "yes" : "no") << std::endl;
    }

    Airy wave_spectrum;
    compute_wave_spectrum(wave_spectrum, use_full_spectrum);

    run_server(wave_spectrum);

    return 0;
}
*/
#include "AiryGRPC.hpp"
#include "Airy.hpp"
#include "DiracSpectralDensity.hpp" // To be removed later // xdyn/code/environment_models/inc/DiracSpectralDensity.hpp
#include "DiracDirectionalSpreading.hpp" // To be removed later // xdyn/code/environment_models/inc/DiracDirectionalSpreading.hpp
#include "DiscreteDirectionalWaveSpectrum.hpp" // To be removed later // xdyn/code/environment_models/inc/DiscreteDirectionalWaveSpectrum.hpp
#include "Stretching.hpp" // To be removed later // xdyn/code/environment_models/inc/Stretching.hpp
#include "YamlWaveModelInput.hpp" // To be removed late // xdyn/code/external_data_structures/inc/YamlWaveModelInput.hpp
#include "discretize.hpp" // To be removed late // xdyn/code/environment_models/inc/discretize.hpp
//#include "args.hxx"
#include "wave_types.pb.h"
#include "wave_types.grpc.pb.h"
#include "wave_grpc.grpc.pb.h"
#include "wave_grpc.pb.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <grpcpp/grpcpp.h>

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

/*
double compute_elevation(const double x, const double y, const double t, const Airy& wave_spectrum);
double compute_elevation(const double x, const double y, const double t, const Airy& wave_spectrum)
{
    double result = 0;
    for (const WaveSpectrumLine& spectrum_line : wave_spectrum.spectrum_lines())
    {
        result += - spectrum_line.a() * sin(
                                        spectrum_line.k() * (x * cos(spectrum_line.psi()) + y * sin(spectrum_line.psi()))
                                        - spectrum_line.omega() * t
                                        + spectrum_line.phase()
                                    );
    }
    return result;
}
*/

class WavesImpl final : public Waves::Service {
    public:
        explicit WavesImpl(const Airy& _wave_spectrum): wave_spectrum(_wave_spectrum) {}
        // rpc elevations(XYTGrid)                                    returns (XYZTGrid);
        // rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
        // rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
        // rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
        // rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
        // rpc directions_for_rao(DirectionsRequest)                  returns (DirectionsResponse);

        // rpc elevations(XYTGrid)                                    returns (XYZTGrid);
        Status elevations(ServerContext* /*context*/, const XYTGrid* request, XYZTGrid* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->set_t(request->t());
            std::vector<double> vec_x;
            vec_x.reserve(request->x_size());
            std::copy(request->x().begin(), request->x().end(), std::back_inserter(vec_x));
            std::vector<double> vec_y;
            vec_y.reserve(request->y_size());
            std::copy(request->y().begin(), request->y().end(), std::back_inserter(vec_y));
            const std::vector<double> res ;//= wave_spectrum.elevation(vec_x, vec_y, request->t());
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(res[index]);
            }
            return Status::OK;
        }

        /*
        message DynamicPressuresResponse
        {
            repeated double x = 1; // Positions (in meters) at which the dynamic pressure was computed. Projected on the X-axis of the Earth-centered, Earth-fixed North-East-Down reference frame. Should be the same size as y, z and pdyn.
            repeated double y = 2; // Positions (in meters) at which the dynamic pressure was computed. Projected on the Y-axis of the Earth-centered, Earth-fixed North-East-Down reference frame. Should be the same size as x, z and pdyn.
            repeated double z = 3; // Positions (in meters) at which the dynamic pressure was computed. Projected on the Z-axis of the Earth-centered, Earth-fixed North-East-Down reference frame. Should be the same size as x, y and pdyn.
            double t = 4;          // Simulation time (in seconds). All values were calculated at that instant. The documentation of each waves model should describe how the phases are defined.
            repeated double pdyn = 5; // Dynamic pressure (in Pascal) for each (x,y,z) value at t. Should be the same size as x, y and z.
        }
        */

        // rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
        Status dynamic_pressures(ServerContext* /*context*/, const XYZTGrid* request, DynamicPressuresResponse* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->clear_pdyn();
            reply->set_t(request->t());
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(request->z(index));
                reply->add_pdyn(request->x(index) + request->y(index));
            }
            return Status::OK;
        }

        // rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
        Status orbital_velocities(ServerContext* /*context*/, const XYZTGrid* request, OrbitalVelocitiesResponse* reply) override
        {
            reply->clear_x();
            reply->clear_y();
            reply->clear_z();
            reply->clear_vx();
            reply->clear_vy();
            reply->clear_vz();
            reply->set_t(request->t());
            for (size_t index = 0; index < size_t(request->x_size()); ++index)
            {
                reply->add_x(request->x(index));
                reply->add_y(request->y(index));
                reply->add_z(request->z(index));
                reply->add_vx(request->x(index) + request->y(index));
                reply->add_vy(request->x(index) + request->y(index));
                reply->add_vz(request->x(index) + request->y(index));
            }
            return Status::OK;
        }


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
        // rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
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

        // rpc GetElevation (ElevationRequest) returns (ElevationResponse) {}
        /*
        Status GetElevation(ServerContext* context, const ElevationRequest* request,
                            ElevationResponse* reply) override
        {
            reply->clear_elevation_points();
            reply->set_t(request->t());
            for (const Point& point : request->points())
            {
                ElevationPoint* added_elevation_point = reply->add_elevation_points();
                added_elevation_point->set_x(point.x());
                added_elevation_point->set_y(point.y());
                added_elevation_point->set_z(point.x() + point.y());
            }

            return Status::OK;
        }
        */
        /*
        Status GetElevationInputRepeated(ServerContext* context, const ElevationRequestRepeated* request,
                            ElevationResponse* reply) override
        {
            reply->clear_elevation_points();
            reply->set_t(request->t());
            for (size_t index = 0; index < request->x_size(); ++index)
            {
                ElevationPoint* added_elevation_point = reply->add_elevation_points();
                added_elevation_point->set_x(request->x(index));
                added_elevation_point->set_y(request->y(index));
                added_elevation_point->set_z(request->x(index) + request->y(index));
            }

            return Status::OK;
        }
        */
        /*
        Status GetElevationOutputRepeated(ServerContext* context, const ElevationRequest* request,
                            ElevationResponseRepeated* reply) override
        {
            reply->clear_z();
            reply->clear_x(); reply->clear_y();
            reply->set_t(request->t());
            for (const Point& point : request->points())
            {
                reply->add_x(point.x());
                reply->add_y(point.y());
                reply->add_z(point.x() + point.y());
            }

            return Status::OK;
        }
        */
        /*
        Status GetElevations(ServerContext* context, const ElevationRequest* request,
                            ServerWriter<ElevationResponse>* writer) override
        {
            ElevationResponse elevation;
            if (request->dt() > 0 && request->t_end() - request->t_start() > 0)
            {
                const double count = (request->t_end() - request->t_start()) / request->dt();
                for (size_t index = 0; index <= count; ++index)
                {
                    const double t = request->t_start() + index * request->dt();
                    elevation.clear_elevation_points();
                    elevation.set_t(t);
                    for (const Point& point : request->points())
                    {
                        ElevationPoint* added_elevation_point = elevation.add_elevation_points();
                        added_elevation_point->set_x(point.x());
                        added_elevation_point->set_y(point.y());
                        added_elevation_point->set_z(compute_elevation(point.x(), point.y(), t, wave_spectrum));
                    }
                    writer->Write(elevation);
                }
            }
            return Status::OK;
        }
        */
    private:
        Airy wave_spectrum;
};

/*
void compute_wave_spectrum(Airy& wave_spectrum, const bool& use_full_spectrum);
void compute_wave_spectrum(Airy& wave_spectrum, const bool& use_full_spectrum)
{
    std::vector<double> a;
    std::vector<double> omega;
    std::vector<double> psi;
    std::vector<double> k;
    std::vector<double> phase;

    if (use_full_spectrum)
    {
        // Discrete wave spectrum with 128 lines
        a = { 1.25611168095e-82, 1.218727309e-17, 8.24204446012e-06, 0.0147898632456, 0.186499099895, 0.472293130906, 0.67062611788, 0.749629574614, 0.668165953251, 0.578307224415, 0.502829600214, 0.434356393279, 0.374554788998, 0.32364924998, 0.280777888626, 0.244778348913, 0.214513149444, 0.188982344911, 0.167347231674, 0.148919731506, 0.133140920707, 0.119558321358, 0.107805507971, 0.0975850016428, 0.0886543889715, 0.0808152438523, 0.0739043557766, 0.0677868043606, 0.0623504907151, 0.0575018104905, 0.05316221936, 0.0492654962304, 0.0457555529803, 0.0425846735611, 0.039712091654, 0.0371028363743, 0.0347267911238, 0.0325579226994, 0.0305736470113, 0.028754304914, 0.0270827271874, 0.0255438720171, 0.0241245216835, 0.0228130278139, 0.0215990966228, 0.0204736072156, 0.0194284573323, 0.0184564319492, 0.0175510909925, 0.0167066730842, 0.0159180127851, 0.0151804692364, 0.0144898644579, 0.0138424298523, 0.0132347597045, 0.0126637706576, 0.0121266663143, 0.0116209062406, 0.0111441787637, 0.0106943770466, 0.0102695780012, 0.00986802366285, 0.00948810470778, 0.009128345838, 0.00878739279772, 0.0084640008184, 0.00815702431685, 0.00786540769488, 0.00758817710897, 0.0073244330958, 0.00707334395417, 0.00683413979682, 0.00660610719614, 0.0063885843578, 0.00618095676378, 0.00598265323414, 0.00579314236211, 0.00561192928336, 0.00543855274398, 0.00527258243663, 0.00511361657714, 0.00496127969747, 0.00481522063326, 0.00467511068684, 0.00454064194855, 0.00441152576111, 0.00428749131329, 0.00416828435089, 0.00405366599377, 0.00394341164949, 0.00383731001438, 0.00373516215446, 0.00363678065882, 0.00354198885915, 0.00345062010972, 0.0033625171223, 0.00327753135164, 0.00319552242694, 0.00311635762555, 0.00303991138544, 0.0029660648531, 0.00289470546412, 0.00282572655366, 0.00275902699456, 0.00269451086078, 0.00263208711423, 0.00257166931316, 0.00251317534043, 0.00245652715014, 0.00240165053125, 0.00234847488691, 0.00229693302823, 0.00224696098161, 0.00219849780848, 0.00215148543659, 0.00210586850205, 0.0020615942013, 0.00201861215239, 0.00197687426481, 0.00193633461734, 0.00189694934336, 0.00185867652309, 0.00182147608229, 0.00178530969706, 0.00175014070413, 0.00171593401654, 0.00168265604415, 0.0016502746188 };
        omega = { 0.1, 0.146456692913, 0.192913385827, 0.23937007874, 0.285826771654, 0.332283464567, 0.37874015748, 0.425196850394, 0.471653543307, 0.51811023622, 0.564566929134, 0.611023622047, 0.657480314961, 0.703937007874, 0.750393700787, 0.796850393701, 0.843307086614, 0.889763779528, 0.936220472441, 0.982677165354, 1.02913385827, 1.07559055118, 1.12204724409, 1.16850393701, 1.21496062992, 1.26141732283, 1.30787401575, 1.35433070866, 1.40078740157, 1.44724409449, 1.4937007874, 1.54015748031, 1.58661417323, 1.63307086614, 1.67952755906, 1.72598425197, 1.77244094488, 1.8188976378, 1.86535433071, 1.91181102362, 1.95826771654, 2.00472440945, 2.05118110236, 2.09763779528, 2.14409448819, 2.1905511811, 2.23700787402, 2.28346456693, 2.32992125984, 2.37637795276, 2.42283464567, 2.46929133858, 2.5157480315, 2.56220472441, 2.60866141732, 2.65511811024, 2.70157480315, 2.74803149606, 2.79448818898, 2.84094488189, 2.8874015748, 2.93385826772, 2.98031496063, 3.02677165354, 3.07322834646, 3.11968503937, 3.16614173228, 3.2125984252, 3.25905511811, 3.30551181102, 3.35196850394, 3.39842519685, 3.44488188976, 3.49133858268, 3.53779527559, 3.5842519685, 3.63070866142, 3.67716535433, 3.72362204724, 3.77007874016, 3.81653543307, 3.86299212598, 3.9094488189, 3.95590551181, 4.00236220472, 4.04881889764, 4.09527559055, 4.14173228346, 4.18818897638, 4.23464566929, 4.2811023622, 4.32755905512, 4.37401574803, 4.42047244094, 4.46692913386, 4.51338582677, 4.55984251969, 4.6062992126, 4.65275590551, 4.69921259843, 4.74566929134, 4.79212598425, 4.83858267717, 4.88503937008, 4.93149606299, 4.97795275591, 5.02440944882, 5.07086614173, 5.11732283465, 5.16377952756, 5.21023622047, 5.25669291339, 5.3031496063, 5.34960629921, 5.39606299213, 5.44251968504, 5.48897637795, 5.53543307087, 5.58188976378, 5.62834645669, 5.67480314961, 5.72125984252, 5.76771653543, 5.81417322835, 5.86062992126, 5.90708661417, 5.95354330709, 6.0 };
        psi = { 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679, 1.57079632679 };
        k = { 0.00101936799185, 0.00218649978584, 0.00379363653732, 0.00584077824629, 0.00832792491273, 0.0112550765367, 0.0146222331181, 0.018429394657, 0.0226765611533, 0.0273637326072, 0.0324909090185, 0.0380580903873, 0.0440652767136, 0.0505124679974, 0.0573996642387, 0.0647268654374, 0.0724940715936, 0.0807012827074, 0.0893484987785, 0.0984357198072, 0.107962945793, 0.117930176737, 0.128337412638, 0.139184653497, 0.150471899313, 0.162199150086, 0.174366405817, 0.186973666506, 0.200020932152, 0.213508202755, 0.227435478316, 0.241802758835, 0.256610044311, 0.271857334744, 0.287544630135, 0.303671930484, 0.320239235789, 0.337246546053, 0.354693861274, 0.372581181452, 0.390908506588, 0.409675836681, 0.428883171732, 0.44853051174, 0.468617856706, 0.489145206629, 0.51011256151, 0.531519921348, 0.553367286143, 0.575654655896, 0.598382030607, 0.621549410275, 0.645156794901, 0.669204184484, 0.693691579024, 0.718618978522, 0.743986382978, 0.769793792391, 0.796041206761, 0.822728626089, 0.849856050375, 0.877423479618, 0.905430913818, 0.933878352976, 0.962765797091, 0.992093246164, 1.02186070019, 1.05206815918, 1.08271562313, 1.11380309203, 1.14533056589, 1.17729804471, 1.20970552848, 1.24255301722, 1.27584051091, 1.30956800955, 1.34373551316, 1.37834302172, 1.41339053524, 1.44887805372, 1.48480557715, 1.52117310555, 1.5579806389, 1.5952281772, 1.63291572047, 1.67104326869, 1.70961082187, 1.74861838001, 1.7880659431, 1.82795351116, 1.86828108417, 1.90904866213, 1.95025624506, 1.99190383294, 2.03399142578, 2.07651902358, 2.11948662633, 2.16289423405, 2.20674184672, 2.25102946434, 2.29575708693, 2.34092471447, 2.38653234697, 2.43257998443, 2.47906762684, 2.52599527421, 2.57336292654, 2.62117058383, 2.66941824607, 2.71810591328, 2.76723358544, 2.81680126255, 2.86680894463, 2.91725663166, 2.96814432365, 3.0194720206, 3.0712397225, 3.12344742936, 3.17609514118, 3.22918285796, 3.28271057969, 3.33667830638, 3.39108603803, 3.44593377464, 3.5012215162, 3.55694926273, 3.61311701421, 3.66972477064 };
        phase = { 3.44829693406, 3.72495258314, 4.49366730984, 5.30467811862, 3.78727395246, 5.390631313, 3.42360197426, 5.32343966702, 2.6619015945, 3.9179662553, 4.05827241394, 2.41514150146, 2.74944152834, 1.86946506012, 5.60317502198, 0.356338135805, 6.0548717202, 1.71315002362, 2.40923413292, 3.00125841131, 4.97455509736, 5.10300660907, 3.32314480001, 3.01578551114, 3.56912925681, 2.46793964128, 5.81569516627, 5.25323783595, 0.446332720372, 2.11992260546, 0.547449518533, 4.07258401131, 0.127035948289, 2.31372981705, 5.231504763, 6.01398320231, 4.88930310157, 0.881849942555, 5.46644752814, 5.46691943187, 6.14884039341, 2.9757670784, 5.02126134641, 5.03227067247, 2.8995603519, 3.27025645812, 4.90420944828, 4.26552591125, 0.74314012158, 4.52786848833, 4.02074233171, 3.65693820081, 0.900715257194, 3.3764155721, 5.93552986959, 4.76652252011, 3.27886974819, 0.665437116618, 2.60539781588, 2.97571921758, 1.66225193057, 1.17076064949, 4.86465372337, 4.63019346811, 2.86607708876, 1.36062601954, 3.57157583223, 0.849600839525, 0.118059782852, 2.03663799456, 3.88071830413, 0.940434905003, 3.84591084997, 1.39688646296, 3.87631063583, 2.42838186829, 5.92974404029, 5.67119345366, 4.28400327757, 2.82711916338, 2.25885476804, 3.85199133584, 2.74595274375, 5.66962332856, 4.3833460769, 0.623796846088, 0.378407766225, 6.0934900906, 4.18941880362, 4.10379985384, 4.21374203447, 1.07385660043, 1.32187262034, 2.25033645353, 0.810067823491, 4.71670011804, 1.98189477202, 3.81911271429, 2.28526218755, 2.04233196431, 3.58265200487, 0.241434069132, 2.75581458466, 3.98526141151, 6.21013593825, 6.02525595895, 0.641166430858, 4.10160254064, 1.31241138053, 3.99019260553, 1.01353755048, 6.25365160686, 4.10360061317, 3.65587345831, 1.59147806782, 2.60355462395, 2.92991699631, 2.98261238955, 1.53577126356, 3.9176295369, 0.998835370074, 2.12376449605, 0.693507452672, 4.2395938772, 4.12384044907, 1.99303734212, 0.868229078049, 4.89048889469 };
    }
    else {
        // Discrete wave spectrum with 1 line
        a = { 2.0 };
        omega = { 2 * PI / 12 };
        psi = { PI / 4 };
        k = { omega[0] * omega[0] / G };
        phase = { 0.0 };
    }

    wave_spectrum.clear_spectrum_lines();
    for(size_t index = 0; index < a.size(); ++index)
    {
        WaveSpectrumLine* added_spectrum_line = wave_spectrum.add_spectrum_lines();
        added_spectrum_line->set_a(a[index]);
        added_spectrum_line->set_omega(omega[index]);
        added_spectrum_line->set_psi(psi[index]);
        added_spectrum_line->set_k(k[index]);
        added_spectrum_line->set_phase(phase[index]);
    }
}
*/

Airy create_default_spectrum()
{
    const double Hs = 3;
    const double Tp = 5;
    const double psi = 0;
    const double omega0 = 2*PI/Tp;
    // const double g = 9.81;
    // const double rho = 1000;

    // ssc::random_data_generator::DataGenerator a(123);
    const double omega_min = 0.1;
    const double omega_max = 10;
    const size_t nfreq = 100;

    YamlStretching ys;
    ys.h = 20;//10;
    ys.delta = 1;//0;
    const double h = 20;
    const Stretching stretching(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq, h, stretching, false);
    const double random_phase = 0.123456; /*a.random<double>().between(-PI,PI);*/
    return Airy(A, random_phase);
}

void run_xdyn_airy_server(const Airy& wave_spectrum)
{
    const std::string server_address("0.0.0.0:50051");
    WavesImpl service(wave_spectrum);
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
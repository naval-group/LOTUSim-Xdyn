#include "observers_and_api/simulator_api.hpp"
#include "listeners.hpp"
#include "xdyn/core/DefaultSurfaceElevation.hpp"
#include "xdyn/core/SimulatorBuilder.hpp"
#include "xdyn/core/SurfaceElevationFromWaves.hpp"
#include "xdyn/environment_models/Airy.hpp"
#include "xdyn/environment_models/BretschneiderSpectrum.hpp"
#include "xdyn/environment_models/Cos2sDirectionalSpreading.hpp"
#include "xdyn/environment_models/DefaultWindModel.hpp"
#include "xdyn/environment_models/DiracDirectionalSpreading.hpp"
#include "xdyn/environment_models/DiracSpectralDensity.hpp"
#include "xdyn/environment_models/LogWindVelocityProfile.hpp"
#include "xdyn/environment_models/PiersonMoskowitzSpectrum.hpp"
#include "xdyn/environment_models/PowerLawWindVelocityProfile.hpp"
#include "xdyn/environment_models/JonswapSpectrum.hpp"
#include "xdyn/environment_models/UniformWindVelocityProfile.hpp"
#include "xdyn/external_file_formats/stl_reader.hpp"
#include "xdyn/force_models/AeroPolarForceModel.hpp"
#include "xdyn/force_models/ConstantForceModel.hpp"
#include "xdyn/force_models/DiffractionForceModel.hpp"
#include "xdyn/force_models/ExactHydrostaticForceModel.hpp"
#include "xdyn/force_models/FastHydrostaticForceModel.hpp"
#include "xdyn/force_models/FlettnerRotorForceModel.hpp"
#include "xdyn/force_models/FroudeKrylovForceModel.hpp"
#include "xdyn/force_models/GMForceModel.hpp"
#include "xdyn/force_models/GravityForceModel.hpp"
#include "xdyn/force_models/HoltropMennenForceModel.hpp"
#include "xdyn/force_models/HydroPolarForceModel.hpp"
#include "xdyn/force_models/KtKqForceModel.hpp"
#include "xdyn/force_models/LinearDampingForceModel.hpp"
#include "xdyn/force_models/LinearFroudeKrylovForceModel.hpp"
#include "xdyn/force_models/LinearHydrostaticForceModel.hpp"
#include "xdyn/force_models/LinearStiffnessForceModel.hpp"
#include "xdyn/force_models/ManeuveringForceModel.hpp"
#include "xdyn/force_models/MMGManeuveringForceModel.hpp"
#include "xdyn/force_models/QuadraticDampingForceModel.hpp"
#include "xdyn/force_models/RadiationDampingForceModel.hpp"
#include "xdyn/force_models/ResistanceCurveForceModel.hpp"
#include "xdyn/force_models/RudderForceModel.hpp"
#include "xdyn/force_models/SimpleHeadingKeepingController.hpp"
#include "xdyn/force_models/SimpleStationKeepingController.hpp"
#include "xdyn/force_models/WageningenControlledForceModel.hpp"
#include "grpc/GRPCForceModel.hpp"
#include "grpc/SurfaceElevationFromGRPC.hpp"

SimulatorBuilder get_builder(const YamlSimulatorInput& input, const double t0, const ssc::data_source::DataSource& command_listener = ssc::data_source::DataSource());
SimulatorBuilder get_builder(const YamlSimulatorInput& input, const double t0, const ssc::data_source::DataSource& command_listener)
{
    SimulatorBuilder builder(input, t0, command_listener);
    builder.can_parse<DefaultSurfaceElevation>()
           .can_parse<BretschneiderSpectrum>()
           .can_parse<JonswapSpectrum>()
           .can_parse<PiersonMoskowitzSpectrum>()
           .can_parse<DiracSpectralDensity>()
           .can_parse<DiracDirectionalSpreading>()
           .can_parse<Cos2sDirectionalSpreading>()
           .can_parse<SurfaceElevationFromWaves>()
           .can_parse<Airy>()
           .can_parse<SurfaceElevationFromGRPC>()
           .can_parse<WageningenControlledForceModel>()
           .can_parse<GravityForceModel>()
           .can_parse<ExactHydrostaticForceModel>()
           .can_parse<FastHydrostaticForceModel>()
           .can_parse<FroudeKrylovForceModel>()
           .can_parse<LinearDampingForceModel>()
           .can_parse<ResistanceCurveForceModel>()
           .can_parse<DiffractionForceModel>()
           .can_parse<RadiationDampingForceModel>()
           .can_parse<QuadraticDampingForceModel>()
           .can_parse<SimpleHeadingKeepingController>()
           .can_parse<ManeuveringForceModel>()
           .can_parse<SimpleStationKeepingController>()
           .can_parse<RudderForceModel>()
           .can_parse<GMForceModel>()
           .can_parse<KtKqForceModel>()
           .can_parse<ConstantForceModel>()
           .can_parse<LinearHydrostaticForceModel>()
           .can_parse<GRPCForceModel>()
           .can_parse<DefaultWindModel>()
           .can_parse<UniformWindVelocityProfile>()
           .can_parse<PowerLawWindVelocityProfile>()
           .can_parse<LogWindVelocityProfile>()
           .can_parse<HoltropMennenForceModel>()
           .can_parse<AeroPolarForceModel>()
           .can_parse<HydroPolarForceModel>()
           .can_parse<LinearFroudeKrylovForceModel>()
           .can_parse<MMGManeuveringForceModel>()
           .can_parse<LinearStiffnessForceModel>()
           .can_parse<FlettnerRotorForceModel>();
    return builder;
}

Sim get_system(const YamlSimulatorInput& input, const double t0)
{
    check_input_yaml(input);
    ssc::data_source::DataSource command_listener = make_command_listener(input.commands);
    add_setpoints_listener(command_listener, input.setpoints);
    return get_builder(input, t0, command_listener).build();
}

Sim get_system(const YamlSimulatorInput& input, const std::string& mesh, const double t0)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return get_system(input, meshes, t0);
}

Sim get_system(const std::string& yaml, const std::string& mesh, const double t0, ssc::data_source::DataSource& commands)
{
    return get_system(yaml, read_stl(mesh), t0, commands);
}

Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_system(input, meshes, t0);
}

Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& commands)
{
    return get_system(SimulatorYamlParser(yaml).parse(), mesh, t0, commands);
}

Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& command_listener)
{
    const auto name = input.bodies.empty() ? "" : input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_builder(input, t0, command_listener).build(meshes);
}

Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    const auto name = input.bodies.empty() ? "" : input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_system(input, meshes, t0);
}

Sim get_system(const YamlSimulatorInput& input, const MeshMap& meshes, const double t0)
{
    ssc::data_source::DataSource command_listener = make_command_listener(input.commands);
    add_setpoints_listener(command_listener, input.setpoints);
    return get_builder(input, t0, command_listener).build(meshes);
}

Sim get_system(const std::string& yaml, const double t0)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(input, t0);
}

Sim get_system(const std::string& yaml, const std::string& mesh, const double t0)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), mesh, t0);
}

Sim get_system(const std::string& yaml, const MeshMap& meshes, const double t0)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), meshes, t0);
}

EnvironmentAndFrames get_environment_for_wave_queries(const std::string& yaml_data)
{
    EnvironmentAndFrames env(get_system(yaml_data, 0.0).get_env());
    if (env.w == nullptr)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
        "No wave environment model is defined");
    }
    if (env.g == 0.0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
        "Gravity constant g can not be zero");
    }
    if (env.rho == 0.0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
        "Water density rho can not be zero");
    }
    return env;
}

MeshMap make_mesh_map(const YamlSimulatorInput& input, const std::string& mesh)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return meshes;
}

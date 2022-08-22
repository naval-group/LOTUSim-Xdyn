#include "py_xdyn_force.hpp"
#include "py_pybind_additions.hpp"
#include "core/inc/ForceModel.hpp"
#include "core/inc/ImmersedSurfaceForceModel.hpp"
#include "external_data_structures/inc/GeometricTypes3d.hpp"
#include "force_models/inc/AbstractWageningen.hpp"
#include "force_models/inc/AbstractRaoForceModel.hpp"
#include "force_models/inc/AeroPolarForceModel.hpp"
#include "force_models/inc/DampingForceModel.hpp"
#include "force_models/inc/DiffractionForceModel.hpp"
#include "force_models/inc/ExactHydrostaticForceModel.hpp"
#include "force_models/inc/FastHydrostaticForceModel.hpp"
#include "force_models/inc/KtKqForceModel.hpp"
#include "force_models/inc/ConstantForceModel.hpp"
#include "force_models/inc/FlettnerRotorForceModel.hpp"
#include "force_models/inc/HoltropMennenForceModel.hpp"
#include "force_models/inc/HydroPolarForceModel.hpp"
#include "force_models/inc/GravityForceModel.hpp"
#include "force_models/inc/LinearDampingForceModel.hpp"
#include "force_models/inc/LinearHydrostaticForceModel.hpp"
#include "force_models/inc/LinearFroudeKrylovForceModel.hpp"
#include "force_models/inc/LinearStiffnessForceModel.hpp"
#include "force_models/inc/MMGManeuveringForceModel.hpp"
#include "force_models/inc/ManeuveringForceModel.hpp"
#include "force_models/inc/QuadraticDampingForceModel.hpp"
#include "force_models/inc/RadiationDampingForceModel.hpp"
#include "force_models/inc/ResistanceCurveForceModel.hpp"
#include "force_models/inc/RudderForceModel.hpp"
#include "force_models/inc/SimpleHeadingKeepingController.hpp"
#include "force_models/inc/SimpleStationKeepingController.hpp"
#include "force_models/inc/WageningenControlledForceModel.hpp"
#include "force_models/unit_tests/HDBParserForTests.hpp"
#include "hdb_interpolators/inc/HDBParser.hpp"
#include "ssc/ssc/data_source/DataSource.hpp"
#include "ssc/ssc/kinematics/Point.hpp"
#include "ssc/ssc/kinematics/Wrench.hpp"
#include "ssc/ssc/macros/tr1_macros.hpp"
#include <string>
#include TR1INC(memory)

namespace py = pybind11;

void py_add_module_xdyn_force(py::module& m0)
{
    py::module m = m0.def_submodule("force");

    py::class_<ForceModel>(m, "ForceModel", "Base class from which all force models derived")
        .def("get_name", &ForceModel::get_name)
        .def("get_body_name", &ForceModel::get_body_name)
        .def("__call__",
            static_cast<ssc::kinematics::Wrench (ForceModel::*)(
                const BodyStates& /*states*/,
                const double /*t*/,
                const EnvironmentAndFrames& /*env*/,
                ssc::data_source::DataSource& /*command_listener*/)>(&ForceModel::operator()),
            py::arg("BodyStates"),
            py::arg("t"),
            py::arg("env"),
            py::arg("command_listener") = ssc::data_source::DataSource())
        ;

    py::class_<ConstantForceModel::Input>(m, "ConstantForceModelInput",
        "Input for ConstantForceModel")
        .def(py::init<
            const std::string& /*frame*/,
            const double /*x*/,
            const double /*y*/,
            const double /*z*/,
            const double /*X*/,
            const double /*Y*/,
            const double /*Z*/,
            const double /*K*/,
            const double /*M*/,
            const double /*N*/>(),
            py::arg("frame") = "",
            py::arg("x") = 0.0,
            py::arg("y") = 0.0,
            py::arg("z") = 0.0,
            py::arg("X") = 0.0,
            py::arg("Y") = 0.0,
            py::arg("Z") = 0.0,
            py::arg("K") = 0.0,
            py::arg("M") = 0.0,
            py::arg("N") = 0.0
        )
        .def_readwrite("frame", &ConstantForceModel::Input::frame)
        .def_readwrite("x", &ConstantForceModel::Input::x)
        .def_readwrite("y", &ConstantForceModel::Input::y)
        .def_readwrite("z", &ConstantForceModel::Input::z)
        .def_readwrite("X", &ConstantForceModel::Input::X)
        .def_readwrite("Y", &ConstantForceModel::Input::Y)
        .def_readwrite("Z", &ConstantForceModel::Input::Z)
        .def_readwrite("K", &ConstantForceModel::Input::K)
        .def_readwrite("M", &ConstantForceModel::Input::M)
        .def_readwrite("N", &ConstantForceModel::Input::N)
        ;

    py::class_<ConstantForceModel, ForceModel>(m, "ConstantForceModel", "Constant force model")
        .def(py::init<const ConstantForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def("get_name", &ConstantForceModel::get_name)
        .def("get_body_name", &ConstantForceModel::get_body_name)
        .def("get_force", &ConstantForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>()
            )
        .def_static("model_name", &ConstantForceModel::model_name,
            "Returns model name \"constant force\"")
        .def_static("parse", &ConstantForceModel::parse, py::arg("yaml"))
        ;

    py::class_<GravityForceModel, ForceModel>(m, "GravityForceModel",
        "Gravity force model, a creating a vertical down force along Z-NED axis")
        .def(py::init<const std::string&, const EnvironmentAndFrames&>())
        .def_static("model_name", &GravityForceModel::model_name,
            "Returns model name \"gravity\"")
        .def("potential_energy", &GravityForceModel::potential_energy)
        .def("get_force", &GravityForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>())
        ;

    py::class_<ResistanceCurveForceModel::Yaml>(m, "ResistanceCurveForceModelInput")
        .def(py::init<
            const std::vector<double>& /*Va*/,
            const std::vector<double>& /*R*/
            >(),
            py::arg("Va") = std::vector<double>(),
            py::arg("R") = std::vector<double>()
        )
        .def_readwrite("Va", &ResistanceCurveForceModel::Yaml::Va)
        .def_readwrite("R", &ResistanceCurveForceModel::Yaml::R)
        ;

    py::class_<ResistanceCurveForceModel, ForceModel>(m, "ResistanceCurveForceModel", "Resistance curve given by interpolation table")
        .def(py::init<const ResistanceCurveForceModel::Yaml& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &ResistanceCurveForceModel::model_name,
            "Returns model name \"resistance curve\"")
        .def_static("parse", &ResistanceCurveForceModel::parse, py::arg("yaml"))
        .def("get_force", &ResistanceCurveForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>(),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        ;

    py::class_<DampingForceModel, ForceModel>(m, "DampingForceModel", "Abstract damping force model")
        //.def(py::init<const std::string& /*name*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        //.def("get_force", &DampingForceModel::get_force)
        ;

    py::class_<LinearDampingForceModel, DampingForceModel, ForceModel>(m, "LinearDampingForceModel",
        "Linear damping force model")
        .def(py::init<const LinearDampingForceModel::Input& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("parse", &LinearDampingForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &LinearDampingForceModel::model_name,
            "Returns model name \"linear damping\"")
        .def("get_force", &LinearDampingForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>())
        ;

    py::class_<QuadraticDampingForceModel, DampingForceModel, ForceModel>(m, "QuadraticDampingForceModel",
        "Quadratic damping force model")
        .def(py::init<const QuadraticDampingForceModel::Input& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("parse", &QuadraticDampingForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &QuadraticDampingForceModel::model_name,
            "Returns model name \"quadratic damping\"")
        ;

    py::class_<LinearStiffnessForceModel::Input>(m, "LinearStiffnessForceModelInput")
        .def(py::init<
            const std::string& /*name*/,
            const Eigen::Matrix<double,6,6>& /*K*/,
            const boost::optional<YamlPosition>& /*equilibrium_position*/
        >(),
            py::arg("name") = "",
            py::arg("K") = Eigen::Matrix<double,6,6>::Zero(),
            py::arg("equilibrium_position") = boost::optional<YamlPosition>()
        )
        .def_readwrite("name", &LinearStiffnessForceModel::Input::name)
        .def_readwrite("K", &LinearStiffnessForceModel::Input::K)
        .def_readwrite("equilibrium_position", &LinearStiffnessForceModel::Input::equilibrium_position)
        ;

    py::class_<LinearStiffnessForceModel, ForceModel>(m, "LinearStiffnessForceModel",
        "Linear stiffness force model")
        .def(py::init<const LinearStiffnessForceModel::Input& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("parse", &LinearStiffnessForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &LinearStiffnessForceModel::model_name,
            "Returns model name \"linear stiffness\"")
        .def("get_force", &LinearStiffnessForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>())
        ;

    py::class_<MMGManeuveringForceModel::Input>(m, "MMGManeuveringForceModelInput",
        "Input for MMGManeuveringForceModel")
        .def(py::init<
            const YamlCoordinates& /*application_point*/,
            const double /*Lpp*/,
            const double /*T*/,
            const double /*Xvv*/,
            const double /*Xrr*/,
            const double /*Xvr*/,
            const double /*Xvvvv*/,
            const double /*Yv*/,
            const double /*Yr*/,
            const double /*Yvvv*/,
            const double /*Yrvv*/,
            const double /*Yvrr*/,
            const double /*Yrrr*/,
            const double /*Nv*/,
            const double /*Nr*/,
            const double /*Nvvv*/,
            const double /*Nrvv*/,
            const double /*Nvrr*/,
            const double /*Nrr*/
            >(),
            py::arg("application_point") = YamlCoordinates(),
            py::arg("Lpp") = 0.0,
            py::arg("T") = 0.0,
            py::arg("Xvv") = 0.0,
            py::arg("Xrr") = 0.0,
            py::arg("Xvr") = 0.0,
            py::arg("Xvvvv") = 0.0,
            py::arg("Yv") = 0.0,
            py::arg("Yr") = 0.0,
            py::arg("Yvvv") = 0.0,
            py::arg("Yrvv") = 0.0,
            py::arg("Yvrr") = 0.0,
            py::arg("Yrrr") = 0.0,
            py::arg("Nv") = 0.0,
            py::arg("Nr") = 0.0,
            py::arg("Nvvv") = 0.0,
            py::arg("Nrvv") = 0.0,
            py::arg("Nvrr") = 0.0,
            py::arg("Nrrr") = 0.0
            )
        .def_readwrite("application_point", &MMGManeuveringForceModel::Input::application_point)
        .def_readwrite("Lpp", &MMGManeuveringForceModel::Input::Lpp)
        .def_readwrite("T", &MMGManeuveringForceModel::Input::T)
        .def_readwrite("Xvv", &MMGManeuveringForceModel::Input::Xvv)
        .def_readwrite("Xrr", &MMGManeuveringForceModel::Input::Xrr)
        .def_readwrite("Xvr", &MMGManeuveringForceModel::Input::Xvr)
        .def_readwrite("Xvvvv", &MMGManeuveringForceModel::Input::Xvvvv)
        .def_readwrite("Yv", &MMGManeuveringForceModel::Input::Yv)
        .def_readwrite("Yr", &MMGManeuveringForceModel::Input::Yr)
        .def_readwrite("Yvvv", &MMGManeuveringForceModel::Input::Yvvv)
        .def_readwrite("Yrvv", &MMGManeuveringForceModel::Input::Yrvv)
        .def_readwrite("Yvrr", &MMGManeuveringForceModel::Input::Yvrr)
        .def_readwrite("Yrrr", &MMGManeuveringForceModel::Input::Yrrr)
        .def_readwrite("Nv", &MMGManeuveringForceModel::Input::Nv)
        .def_readwrite("Nr", &MMGManeuveringForceModel::Input::Nr)
        .def_readwrite("Nvvv", &MMGManeuveringForceModel::Input::Nvvv)
        .def_readwrite("Nrvv", &MMGManeuveringForceModel::Input::Nrvv)
        .def_readwrite("Nvrr", &MMGManeuveringForceModel::Input::Nvrr)
        .def_readwrite("Nrrr", &MMGManeuveringForceModel::Input::Nrrr)
    ;

    py::class_<MMGManeuveringForceModel, ForceModel>(m, "MMGManeuveringForceModel",
        "Mathematical Model Group manoeuvring ship force model")
        .def(py::init<const MMGManeuveringForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("parse", &MMGManeuveringForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &MMGManeuveringForceModel::model_name,
            "Returns model name \"MMG maneuvering\"")
        .def("get_force", &MMGManeuveringForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>())
        ;

    py::class_<HydroPolarForceModel::Input>(m, "HydroPolarForceModelInput",
        "Input for HydroPolarForceModel")
        .def(py::init<
            const std::string& /*name*/,
            const YamlPosition& /*internal_frame*/,
            const std::vector<double>& /*angle_of_attack*/,
            const std::vector<double>& /*lift_coefficient*/,
            const std::vector<double>& /*drag_coefficient*/,
            const boost::optional<std::vector<double>>& /*moment_coefficient*/,
            const double /*reference_area*/,
            const boost::optional<double>& /*chord_length*/,
            const bool /*use_waves_velocity*/,
            const boost::optional<std::string>& /*angle_command*/>(),
            py::arg("name") = "",
            py::arg("internal_frame") = YamlPosition(),
            py::arg("angle_of_attack") = std::vector<double>(),
            py::arg("lift_coefficient") = std::vector<double>(),
            py::arg("drag_coefficient") = std::vector<double>(),
            py::arg("moment_coefficient") = boost::optional<std::vector<double>>(),
            py::arg("reference_area") = 0.0,
            py::arg("chord_length") = boost::optional<double>(),
            py::arg("use_waves_velocity") = false,
            py::arg("angle_command") = boost::optional<std::string>()
            )
        .def_readwrite("name", &HydroPolarForceModel::Input::name)
        .def_readwrite("internal_frame", &HydroPolarForceModel::Input::internal_frame)
        .def_readwrite("angle_of_attack", &HydroPolarForceModel::Input::angle_of_attack)
        .def_readwrite("lift_coefficient", &HydroPolarForceModel::Input::lift_coefficient)
        .def_readwrite("drag_coefficient", &HydroPolarForceModel::Input::drag_coefficient)
        .def_readwrite("moment_coefficient", &HydroPolarForceModel::Input::moment_coefficient)
        .def_readwrite("reference_area", &HydroPolarForceModel::Input::reference_area)
        .def_readwrite("chord_length", &HydroPolarForceModel::Input::chord_length)
        .def_readwrite("use_waves_velocity", &HydroPolarForceModel::Input::use_waves_velocity)
        .def_readwrite("angle_command", &HydroPolarForceModel::Input::angle_command)
        ;

    py::class_<HydroPolarForceModel, ForceModel>(m, "HydroPolarForceModel",
        "Hydrodynamic polar force model")
        .def(py::init<const HydroPolarForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>()
            )
        .def_static("parse", &HydroPolarForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &HydroPolarForceModel::model_name,
            "Returns model name \"hydrodynamic polar\"")
        .def("get_force", &HydroPolarForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>(),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        ;

    py::class_<AeroPolarForceModel::Input>(m, "AeroPolarForceModelInput",
        "Input for AeroPolarForceModel")
        .def(py::init<
            const std::string& /*name*/,
            const YamlCoordinates& /*calculation_point_in_body_frame*/,
            const std::vector<double>& /*apparent_wind_angle*/,
            const std::vector<double>& /*lift_coefficient*/,
            const std::vector<double>& /*drag_coefficient*/,
            const double /*reference_area*/,
            const boost::optional<std::string>& /*angle_command*/
            >(),
            py::arg("name") = "",
            py::arg("calculation_point_in_body_frame") = YamlCoordinates(),
            py::arg("apparent_wind_angle") = std::vector<double>(),
            py::arg("lift_coefficient") = std::vector<double>(),
            py::arg("drag_coefficient") = std::vector<double>(),
            py::arg("reference_area") = 0.0,
            py::arg("angle_command") = boost::optional<std::string>()
            )
        .def_readwrite("name", &AeroPolarForceModel::Input::name)
        .def_readwrite("calculation_point_in_body_frame", &AeroPolarForceModel::Input::calculation_point_in_body_frame)
        .def_readwrite("apparent_wind_angle", &AeroPolarForceModel::Input::apparent_wind_angle)
        .def_readwrite("lift_coefficient", &AeroPolarForceModel::Input::lift_coefficient)
        .def_readwrite("drag_coefficient", &AeroPolarForceModel::Input::drag_coefficient)
        .def_readwrite("reference_area", &AeroPolarForceModel::Input::reference_area,
            "Reference area (in square metres) of the wing, for lift and drag normalization.")
        .def_readwrite("angle_command", &AeroPolarForceModel::Input::angle_command)
        ;

    py::class_<AeroPolarForceModel, ForceModel>(m, "AeroPolarForceModel",
        "Aerodynamic polar force model")
        .def(py::init<const AeroPolarForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>()
            )
        .def_static("parse", &AeroPolarForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &AeroPolarForceModel::model_name,
            "Returns model name \"aerodynamic polar\"")
        .def("get_force", &AeroPolarForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>()
            )
        ;

    py::class_<FlettnerRotorForceModel::Input>(m, "FlettnerRotorForceModelInput",
        "Input for FlettnerRotorForceModel")
        .def(py::init<
            const std::string& /*name*/,
            const YamlCoordinates& /*calculation_point_in_body_frame*/,
            double /*diameter*/,
            double /*length*/,
            std::vector<double> /*spin_ratio*/,
            std::vector<double> /*lift_coefficient*/,
            std::vector<double> /*drag_coefficient*/
            >(),
            py::arg("name") = "",
            py::arg("calculation_point_in_body_frame") = YamlCoordinates(),
            py::arg("diameter") = 0.0,
            py::arg("length") = 0.0,
            py::arg("spin_ratio") = std::vector<double>(),
            py::arg("lift_coefficient") = std::vector<double>(),
            py::arg("drag_coefficient") = std::vector<double>()
            )
        .def_readwrite("name", &FlettnerRotorForceModel::Input::name)
        .def_readwrite("calculation_point_in_body_frame", &FlettnerRotorForceModel::Input::calculation_point_in_body_frame)
        .def_readwrite("diameter", &FlettnerRotorForceModel::Input::diameter)
        .def_readwrite("length", &FlettnerRotorForceModel::Input::length)
        .def_readwrite("spin_ratio", &FlettnerRotorForceModel::Input::spin_ratio)
        .def_readwrite("lift_coefficient", &FlettnerRotorForceModel::Input::lift_coefficient)
        .def_readwrite("drag_coefficient", &FlettnerRotorForceModel::Input::drag_coefficient)
        ;

    py::class_<FlettnerRotorForceModel, ForceModel>(m, "FlettnerRotorForceModel", "Flettner rotor force")
        .def(py::init<const FlettnerRotorForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("parse", &FlettnerRotorForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &FlettnerRotorForceModel::model_name,
            "Returns model name \"Flettner rotor\"")
        .def("get_force", &FlettnerRotorForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands"),
            py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>()
            )
        ;

    py::class_<AbstractWageningen::Yaml>(m, "AbstractWageningenInput",
        "Input for AbstractWageningen")
        .def_readwrite("name", &AbstractWageningen::Yaml::name)
        .def_readwrite("position_of_propeller_frame", &AbstractWageningen::Yaml::position_of_propeller_frame)
        .def_readwrite("wake_coefficient", &AbstractWageningen::Yaml::wake_coefficient)
        .def_readwrite("relative_rotative_efficiency", &AbstractWageningen::Yaml::relative_rotative_efficiency)
        .def_readwrite("thrust_deduction_factor", &AbstractWageningen::Yaml::thrust_deduction_factor)
        .def_readwrite("rotating_clockwise", &AbstractWageningen::Yaml::rotating_clockwise)
        .def_readwrite("diameter", &AbstractWageningen::Yaml::diameter)
        ;

    py::class_<AbstractWageningen, ForceModel>(m, "AbstractWageningen")
    //    .def(py::init<const AbstractWageningen::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("parse", &AbstractWageningen::parse, py::arg("yaml"))
        .def("advance_ratio", &AbstractWageningen::advance_ratio)
        // Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;
        ;

    py::class_<KtKqForceModel::Yaml, AbstractWageningen::Yaml>(m, "KtKqForceModelInput",
        "Input for KtKqForceModel")
        .def_readwrite("J", &KtKqForceModel::Yaml::J)
        .def_readwrite("Kt", &KtKqForceModel::Yaml::Kt)
        .def_readwrite("Kq", &KtKqForceModel::Yaml::Kq)
        ;

    py::class_<KtKqForceModel, AbstractWageningen, ForceModel>(m, "KtKqForceModel", "Kt Kq force model")
        .def(py::init<const KtKqForceModel::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &KtKqForceModel::model_name,
            "Returns model name \"Kt(J) & Kq(J)\"")
        .def("get_force", &AbstractWageningen::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands"))
        .def_static("parse", &KtKqForceModel::parse, py::arg("yaml"))
        .def("get_Kt", &KtKqForceModel::get_Kt)
        .def("get_Kq", &KtKqForceModel::get_Kq)
        ;

    py::class_<WageningenControlledForceModel::Yaml, AbstractWageningen::Yaml>(m, "WageningenControlledForceModelInput",
        "Input for WageningenControlledForceModel")
        .def_readwrite("number_of_blades", &WageningenControlledForceModel::Yaml::number_of_blades)
        .def_readwrite("blade_area_ratio", &WageningenControlledForceModel::Yaml::blade_area_ratio)
        ;

    py::class_<WageningenControlledForceModel, AbstractWageningen, ForceModel>(m, "WageningenControlledForceModel",
        "Wageningen controlled force model")
        .def(py::init<const WageningenControlledForceModel::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &WageningenControlledForceModel::model_name,
            "Returns model name \"wageningen B-series\"")
        .def("get_force", &AbstractWageningen::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        .def_static("parse", &WageningenControlledForceModel::parse, py::arg("yaml"))
        .def("get_Kt", &WageningenControlledForceModel::get_Kt)
        .def("get_Kq", &WageningenControlledForceModel::get_Kq)
        .def("Kt", &WageningenControlledForceModel::Kt, py::arg("Z"),py::arg("AE_A0"),py::arg("P_D"),py::arg("J"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        .def("Kq", &WageningenControlledForceModel::Kq, py::arg("Z"),py::arg("AE_A0"),py::arg("P_D"),py::arg("J"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        ;

    py::class_<HoltropMennenForceModel::Input>(m, "HoltropMennenForceModelInput",
        "Input for HoltropMennenForceModel")
        .def(py::init<>())
        .def_readwrite("Lwl", &HoltropMennenForceModel::Input::Lwl)
        .def_readwrite("Lpp", &HoltropMennenForceModel::Input::Lpp)
        .def_readwrite("B", &HoltropMennenForceModel::Input::B)
        .def_readwrite("Ta", &HoltropMennenForceModel::Input::Ta)
        .def_readwrite("Tf", &HoltropMennenForceModel::Input::Tf)
        .def_readwrite("Vol", &HoltropMennenForceModel::Input::Vol)
        .def_readwrite("lcb", &HoltropMennenForceModel::Input::lcb)
        .def_readwrite("S", &HoltropMennenForceModel::Input::S)
        .def_readwrite("Abt", &HoltropMennenForceModel::Input::Abt)
        .def_readwrite("hb", &HoltropMennenForceModel::Input::hb)
        .def_readwrite("Cm", &HoltropMennenForceModel::Input::Cm)
        .def_readwrite("Cwp", &HoltropMennenForceModel::Input::Cwp)
        .def_readwrite("iE", &HoltropMennenForceModel::Input::iE) // WARNING: iE must be in degrees
        .def_readwrite("At", &HoltropMennenForceModel::Input::At)
        .def_readwrite("Sapp", &HoltropMennenForceModel::Input::Sapp)
        .def_readwrite("Cstern", &HoltropMennenForceModel::Input::Cstern)
        .def_readwrite("hull_form_coeff", &HoltropMennenForceModel::Input::hull_form_coeff)
        .def_readwrite("app_form_coeff", &HoltropMennenForceModel::Input::app_form_coeff)
        .def_readwrite("apply_on_ship_speed_direction", &HoltropMennenForceModel::Input::apply_on_ship_speed_direction)
        ;

    py::class_<HoltropMennenForceModel::DerivedData>(m, "HoltropMennenForceModelDerivedData")
        .def(py::init<const HoltropMennenForceModel::Input& /*input*/>())
        .def_readwrite("c17", &HoltropMennenForceModel::DerivedData::c17)
        .def_readwrite("c15", &HoltropMennenForceModel::DerivedData::c15)
        .def_readwrite("c7", &HoltropMennenForceModel::DerivedData::c7)
        .def_readwrite("c4", &HoltropMennenForceModel::DerivedData::c4)
        .def_readwrite("T", &HoltropMennenForceModel::DerivedData::T)
        .def_readwrite("Pb", &HoltropMennenForceModel::DerivedData::Pb)
        .def_readwrite("c14", &HoltropMennenForceModel::DerivedData::c14)
        .def_readwrite("c5", &HoltropMennenForceModel::DerivedData::c5)
        .def_readwrite("m3", &HoltropMennenForceModel::DerivedData::m3)
        .def_readwrite("Cb", &HoltropMennenForceModel::DerivedData::Cb)
        .def_readwrite("Cp", &HoltropMennenForceModel::DerivedData::Cp)
        .def_readwrite("c3", &HoltropMennenForceModel::DerivedData::c3)
        .def_readwrite("c2", &HoltropMennenForceModel::DerivedData::c2)
        .def_readwrite("Ca", &HoltropMennenForceModel::DerivedData::Ca)
        .def_readwrite("S", &HoltropMennenForceModel::DerivedData::S)
        .def_readwrite("c16", &HoltropMennenForceModel::DerivedData::c16)
        .def_readwrite("Lr", &HoltropMennenForceModel::DerivedData::Lr)
        .def_readwrite("Lambda", &HoltropMennenForceModel::DerivedData::lambda) // Capitalize first letter to avoid conflict with python reserved keyword lambda
        .def_readwrite("iE", &HoltropMennenForceModel::DerivedData::iE)
        .def_readwrite("c1", &HoltropMennenForceModel::DerivedData::c1)
        .def_readwrite("m1", &HoltropMennenForceModel::DerivedData::m1)
        .def_readwrite("hull_form_coeff", &HoltropMennenForceModel::DerivedData::hull_form_coeff)
        ;

    py::class_<HoltropMennenForceModel, ForceModel>(m, "HoltropMennenForceModel",
        "Holtrop Mennen force model")
        .def(py::init<const HoltropMennenForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"))
        .def("get_force", &HoltropMennenForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>()
            )
        .def_static("parse", &HoltropMennenForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &HoltropMennenForceModel::model_name,
            "Returns model name \"Holtrop & Mennen\"")
        .def("Rf", &HoltropMennenForceModel::Rf, py::arg("states"), py::arg("env"),"Frictional resistance over the hull")
        .def("Rapp", &HoltropMennenForceModel::Rapp, py::arg("states"), py::arg("env"), "Frictional resistance over the appendages")
        .def("Rw", &HoltropMennenForceModel::Rw, py::arg("states"), py::arg("env"), "Wave-making resistance")
        .def("Rb", &HoltropMennenForceModel::Rb, py::arg("states"), py::arg("env"), "Bulbous bow influence")
        .def("Rtr", &HoltropMennenForceModel::Rtr, py::arg("states"), py::arg("env"), "Immersed transom stern influence")
        .def("Ra", &HoltropMennenForceModel::Ra, py::arg("states"), py::arg("env"),"Correlation term between model and full scale")
        ;

    py::class_<RudderForceModel::Yaml, WageningenControlledForceModel::Yaml, AbstractWageningen::Yaml>(m, "RudderForceModelInput")
        .def(py::init<>())
        .def(py::init<const WageningenControlledForceModel::Yaml& /*yaml*/>())
        .def_readwrite("Ar", &RudderForceModel::Yaml::Ar) //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
        .def_readwrite("b", &RudderForceModel::Yaml::b)   //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
        .def_readwrite("effective_aspect_ratio_factor", &RudderForceModel::Yaml::effective_aspect_ratio_factor) //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
        .def_readwrite("lift_coeff", &RudderForceModel::Yaml::lift_coeff) //!< Non-dimensional: lift is multiplied by it (for tuning)
        .def_readwrite("drag_coeff", &RudderForceModel::Yaml::drag_coeff) //!< Non-dimensional: drag is multiplied by it (for tuning)
        .def_readwrite("position_of_the_rudder_frame_in_the_body_frame", &RudderForceModel::Yaml::position_of_the_rudder_frame_in_the_body_frame)
        ;

    py::class_<RudderForceModel, ForceModel>(m, "RudderForceModel", "Rudder force model")
        .def(py::init<const RudderForceModel::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("env"),
            py::arg("commands")
            )
        .def("get_force", &RudderForceModel::get_force)
        .def("get_rudder_force", &RudderForceModel::get_rudder_force)
        .def_static("parse", &RudderForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &RudderForceModel::model_name,
            "Returns model name \"propeller+rudder\"")
        ;

    py::class_<RudderForceModel::InOutWake<double>>(m, "RudderForceModelInOutWake")
        .def(py::init<>())
        .def_readwrite("in_wake", &RudderForceModel::InOutWake<double>::in_wake)
        .def_readwrite("outside_wake", &RudderForceModel::InOutWake<double>::outside_wake)
        ;

    py::class_<RudderForceModel::InOutWake<ssc::kinematics::Point>>(m, "RudderForceModelInOutWakeSscPoint")
        .def(py::init<>())
        .def_readwrite("in_wake", &RudderForceModel::InOutWake<ssc::kinematics::Point>::in_wake)
        .def_readwrite("outside_wake", &RudderForceModel::InOutWake<ssc::kinematics::Point>::outside_wake)
        ;

    py::class_<RudderForceModel::InOutWake<ssc::kinematics::Vector6d>>(m, "RudderForceModelInOutWakeSscVector6d")
        .def(py::init<>())
        .def_readwrite("in_wake", &RudderForceModel::InOutWake<ssc::kinematics::Vector6d>::in_wake)
        .def_readwrite("outside_wake", &RudderForceModel::InOutWake<ssc::kinematics::Vector6d>::outside_wake)
        ;

    py::class_<RudderForceModel::RudderModel>(m, "RudderModel")
        .def(py::init<const RudderForceModel::Yaml& /*parameters*/, const double /*rho*/, const double /*nu*/>(),
            py::arg("parameters"),
            py::arg("rho"),
            py::arg("nu")
            )
        .def("get_Ar", &RudderForceModel::RudderModel::get_Ar,
            py::arg("CTh"),
            R"(Calculates the rudder area (in or outside wake)

               Input:

               - `CTh` (float) Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20

               Return
               InOutWake<double> : Rudder area (in m^2)
               )")
        .def("get_angle_of_attack", &RudderForceModel::RudderModel::get_angle_of_attack,
            py::arg("rudder_angle"),
            py::arg("fluid_angle"),
            R"(Calculates the angle between the propeller's wake & the rudder
               When positive, the wake is coming towards the rudder's port side

               Input:

               - `rudder_angle` (float) Rudder angle (in radian): positive if rudder on port side
               - `fluid_angle` (float) Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)

               Return
               Angle of attack (in radian)
               )")
        .def("get_lift", &RudderForceModel::RudderModel::get_lift,
            py::arg("Vs"),
            py::arg("Cl"),
            py::arg("alpha"),
            py::arg("area"),
            R"(Calculates the norm of the lift force acting on the rudder

               Input:

               - `Vs` (float) Norm of the speed of the ship relative to the fluid
               - `Cl` (float) Rudder lift coefficient (non-dimensional)
               - `alpha` (float) Angle between the propeller's wake & the rudder (in radian)
               - `area` (float) Rudder area (in or outside wake) in m^2

               Return
               float: Lift force (in Newton)
               )")
        .def("get_drag", &RudderForceModel::RudderModel::get_drag,
            py::arg("Vs"),
            py::arg("Cl"),
            py::arg("alpha"),
            py::arg("area"),
            R"(Calculates the norm of the drag force acting on the rudder

               Input:

               - `Vs` (float) Norm of the speed of the ship relative to the fluid
               - `Cl` (float) Rudder lift coefficient (non-dimensional)
               - `alpha` (float) Angle between the propeller's wake & the rudder (in radian)
               - `area` (float) Rudder area (in or outside wake) in m^2

               Return
               float: Drag force (in Newton)
               )")
        .def("get_Cd", &RudderForceModel::RudderModel::get_Cd,
            py::arg("Vs"),
            py::arg("Cl"),
            R"(Calculates the drag coefficient (non-dimensional)

               Input:

               - `Vs` (float) Norm of the speed of the ship relative to the fluid
               - `Cl` (float) Rudder lift coefficient (non-dimensional)

               Return
               float: Drag coeffcient
               )")
        .def("get_Cl", &RudderForceModel::RudderModel::get_Cl,
            py::arg("alpha_wake"),
            R"(Calculates the lift coefficient (non-dimensional)

               Soeding formula, "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 eq 1.2.48 & p. 77 eq. 1.2.8

               Input:

               - `alpha_wake` (float) Angle of rudder wrt the fluid (in radian)

               Return
               float: Lift coeffcient
               )")
        .def("get_wrench",
            static_cast<RudderForceModel::InOutWake<ssc::kinematics::Vector6d> (RudderForceModel::RudderModel::*)(
                const double /*rudder_angle*/, //!< Rudder angle (in radian): positive if rudder on port side
                const RudderForceModel::InOutWake<double>& /*fluid_angle*/,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                const RudderForceModel::InOutWake<ssc::kinematics::Point>& /*Vs*/,  //!< Norm of the speed of the ship relative to the fluid (in m/s)
                const RudderForceModel::InOutWake<double>& /*area*/ //!< Rudder area (in or outside wake) in m^2
                ) const>(&RudderForceModel::RudderModel::get_wrench),
            py::arg("rudder_angle"),
            py::arg("fluid_angle"),
            py::arg("Vs"),
            py::arg("area"),
            R"(Wrench created by the rudder on the ship

               Input:

               - `rudder_angle` (float) Rudder angle (in radian): positive if rudder on port side
               - `fluid_angle` (InOutWake) Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
               - `Vs` (InOutWakeSscPoint) Norm of the speed of the ship relative to the fluid (in m/s)
               - `area` (InOutWake)Rudder area (in or outside wake) in m^2

               Return
                InOutWakeSscVector6d: wrench
               )")

        .def("get_wrench",
            static_cast<ssc::kinematics::Vector6d (RudderForceModel::RudderModel::*)(
                const double /*rudder_angle*/, //!< Rudder angle (in radian): positive if rudder on port side
                const double /*fluid_angle*/,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                const double /*Vs*/,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                const double /*area*/          //!< Rudder area (in or outside wake) in m^2
                ) const>(&RudderForceModel::RudderModel::get_wrench),
            py::arg("rudder_angle"),
            py::arg("fluid_angle"),
            py::arg("Vs"),
            py::arg("area"),
            R"(Wrench created by the rudder on the ship
               Expressed in the rudder's reference frame

               Input:

               - `rudder_angle` (float) Rudder angle (in radian): positive if rudder on port side
               - `fluid_angle` (float) Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
               - `Vs` (float) Norm of the speed of the ship relative to the fluid (in m/s)
               - `area` (float) Rudder area (in or outside wake) in m^2

               Return
                ssc::kinematics::Vector6d: wrench
               )")
        .def("get_force", &RudderForceModel::RudderModel::get_force,
            py::arg("lift"),
            py::arg("drag"),
            py::arg("angle"),
            R"(Wrench created by the rudder on the ship
               Expressed in the rudder's reference frame

               Input:

               - `lift` (float) Norm of the lift (in N)
               - `drag` (float) Norm of the drag (in N)
               - `angle` (float) Angle between the fluid & the rudder (in radian)

               Return
                ssc::kinematics::Vector6d: wrench
               )")
        .def("get_vs", &RudderForceModel::RudderModel::get_vs,
            py::arg("CTh"),
            py::arg("Va"),
            py::arg("v"),
            py::arg("T"),
            R"(Calculates speed inside & outside wake

               Ship speed relative to the fluid, in m/s

               Input:

               - `CTh` (float) Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
               - `Va` (float) Projection of the ship speed (relative to the current) on the X-axis of the ship's reference frame (m/s)
               - `v` (float) Projection of the ship speed (relative to the current) on the Y-axis of the ship's reference frame (m/s)
               - `T` (float) Propeller thrust (in N)

               Return
                InOutWakeSscPoint:
               )")
        .def("get_fluid_angle", &RudderForceModel::RudderModel::get_fluid_angle,
            py::arg("Vs"),
            R"(Calculates the angle of incidence of the fluid, inside & outside wake

               Ship speed relative to the fluid, in m/s

               Input:

               - `Vs` (InOutWakeSscPoint) Ship speed relative to the fluid, inside & outside wake

               Return
                InOutWake: Angle in radian
               )")
        .def("get_D", &RudderForceModel::RudderModel::get_D)
        ;

    py::class_<RadiationDampingForceModel::Input>(m, "RadiationDampingForceModelInput",
        "Input for RadiationDampingForceModel")
        .def(py::init<>())
        .def_readwrite("yaml", &RadiationDampingForceModel::Input::yaml)
        .def("set_hdb_parser_for_tests",
            [] (RadiationDampingForceModel::Input &a,
                const std::vector<double>& omega,
                const std::vector<double>& Ma,
                const std::vector<double>& Br,
                const bool only_diagonal_terms)
            {
                a.parser.reset(new HDBParserForTests(omega, Ma, Br, only_diagonal_terms));
            },
            py::arg("omega"),
            py::arg("Ma"),
            py::arg("Br"),
            py::arg("only_diagonal_terms")
        );

    py::class_<RadiationDampingForceModel, ForceModel>(m, "RadiationDampingForceModel", "Radiation damping force model")
        .def(py::init<const RadiationDampingForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>()
            )
        .def_static("model_name", &RadiationDampingForceModel::model_name,
            "Returns model name \"radiation damping\"")
        .def("get_force", &RadiationDampingForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>()
            )
        .def("get_Tmax", &RadiationDampingForceModel::get_Tmax)
        .def_static("parse", &RadiationDampingForceModel::parse, py::arg("yaml"), py::arg("parse_hdb")=true)
        ;

    py::class_<SimpleHeadingKeepingController::Yaml>(m, "SimpleHeadingKeepingControllerInput",
        "Input for SimpleHeadingKeepingController")
        .def_readwrite("name", &SimpleHeadingKeepingController::Yaml::name)
        .def_readwrite("ksi", &SimpleHeadingKeepingController::Yaml::ksi)
        .def_readwrite("Tp", &SimpleHeadingKeepingController::Yaml::Tp)
        ;

    py::class_<SimpleHeadingKeepingController, ForceModel>(m, "SimpleHeadingKeepingController",
        "Simple heading keeping controller")
        .def(py::init<const SimpleHeadingKeepingController::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &SimpleHeadingKeepingController::model_name,
            "Returns model name \"simple heading controller\"")
        .def("get_force", &SimpleHeadingKeepingController::get_force)
        .def_static("parse", &SimpleHeadingKeepingController::parse, py::arg("yaml"))
        ;

    py::class_<SimpleStationKeepingController::Yaml>(m, "SimpleStationKeepingControllerInput",
        "Input for SimpleStationKeepingController")
        .def_readwrite("name", &SimpleStationKeepingController::Yaml::name)
        .def_readwrite("ksi_x", &SimpleStationKeepingController::Yaml::ksi_x)
        .def_readwrite("T_x", &SimpleStationKeepingController::Yaml::T_x)
        .def_readwrite("ksi_y", &SimpleStationKeepingController::Yaml::ksi_y)
        .def_readwrite("T_y", &SimpleStationKeepingController::Yaml::T_y)
        .def_readwrite("ksi_psi", &SimpleStationKeepingController::Yaml::ksi_psi)
        .def_readwrite("T_psi", &SimpleStationKeepingController::Yaml::T_psi)
        ;

    py::class_<SimpleStationKeepingController, ForceModel>(m, "SimpleStationKeepingController", "Simple station keeping controller")
        .def(py::init<const SimpleStationKeepingController::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &SimpleStationKeepingController::model_name,
            "Returns model name \"simple station-keeping controller\"")
        .def("get_force", &SimpleStationKeepingController::get_force)
        .def_static("parse", &SimpleStationKeepingController::parse, py::arg("yaml"))
        ;

    py::class_<AbstractRaoForceModel, ForceModel>(m, "AbstractRaoForceModel", "Abstracts the commonalities between linear Froude-Krylov & diffraction force models")
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
        )
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/, const std::string& /*hdb_file_contents*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::arg("hdb_file_contents")
        )
        .def("get_force", &AbstractRaoForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>(),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        .def_static("parse", &AbstractRaoForceModel::parse, py::arg("yaml"), py::arg("type_of_rao"))
        ;

    py::class_<DiffractionForceModel, AbstractRaoForceModel, ForceModel>(m, "DiffractionForceModel", "Diffraction force model")
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
        )
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/, const std::string& /*hdb_file_contents*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::arg("hdb_file_contents"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
        )
        .def_static("parse", &DiffractionForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &DiffractionForceModel::model_name,
            "Returns model name \"diffraction\"")
        ;

    py::class_<LinearFroudeKrylovForceModel, AbstractRaoForceModel, ForceModel>(m, "LinearFroudeKrylovForceModel", "Linear Froude Krylov force model")
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
        )
        .def(py::init<const YamlRAO& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/, const std::string& /*hdb_file_contents*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env"),
            py::arg("hdb_file_contents"),
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
        )
        .def_static("parse", &LinearFroudeKrylovForceModel::parse, py::arg("yaml"))
        .def_static("model_name", &LinearFroudeKrylovForceModel::model_name,
            "Returns model name \"linear Froude-Krylov\"")
        ;

    py::class_<LinearHydrostaticForceModel::Input>(m, "LinearHydrostaticForceModelInput",
        "Input for LinearHydrostaticForceModel")
        .def(py::init<
            const double& /*z_eq*/,
            const double& /*theta_eq*/,
            const double& /*phi_eq*/,
            const std::vector<double>& /*K1*/,
            const std::vector<double>& /*K2*/,
            const std::vector<double>& /*K3*/
            >(),
            py::arg("z_eq") = 0.0,
            py::arg("theta_eq") = 0.0,
            py::arg("phi_eq") = 0.0,
            py::arg("K1") = std::vector<double>(),
            py::arg("K2") = std::vector<double>(),
            py::arg("K3") = std::vector<double>()
            )
        .def_readwrite("z_eq", &LinearHydrostaticForceModel::Input::z_eq)
        .def_readwrite("theta_eq", &LinearHydrostaticForceModel::Input::theta_eq)
        .def_readwrite("phi_eq", &LinearHydrostaticForceModel::Input::phi_eq)
        .def_readwrite("K1", &LinearHydrostaticForceModel::Input::K1)
        .def_readwrite("K2", &LinearHydrostaticForceModel::Input::K2)
        .def_readwrite("K3", &LinearHydrostaticForceModel::Input::K3)
        ;

    py::class_<LinearHydrostaticForceModel, ForceModel>(m, "LinearHydrostaticForceModel", "Linear hydrostatic force model")
        .def(py::init<const LinearHydrostaticForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
            )
        .def_static("model_name", &LinearHydrostaticForceModel::model_name, "Returns model name \"linear hydrostatics\"")
        .def_static("parse", &LinearHydrostaticForceModel::parse, py::arg("yaml"))
        .def("get_force", &LinearHydrostaticForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string,double>())
        ;

    py::class_<SurfaceForceModel::DF>(m, "SurfaceForceModelDF")
        .def(py::init<const EPoint& /*dF_*/, const EPoint& /*C_*/>(),
            py::arg("dF") = EPoint(), py::arg("C") = EPoint())
        ;

    py::class_<SurfaceForceModel, ForceModel>(m, "SurfaceForceModel")
        // .def(py::init<const std::string& /*name*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
        //     py::arg("name"),
        //     py::arg("body_name"),
        //     py::arg("env")
        // )
        .def("get_force", &SurfaceForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands") = std::map<std::string, double>())
        .def("potential_energy", &SurfaceForceModel::potential_energy,
            py::arg("states"),
            py::arg("x"),
            py::arg("env"),
            "Compute potential energy of the hydrostatic force model")
        .def("is_a_surface_force_model", &SurfaceForceModel::is_a_surface_force_model)
        ;

    py::class_<ImmersedSurfaceForceModel, SurfaceForceModel, ForceModel>(m, "ImmersedSurfaceForceModel")
        // .def(py::init<const std::string& /*name*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
        //     py::arg("name"),
        //     py::arg("body_name"),
        //     py::arg("env")
        // )
        ;

    py::class_<FastHydrostaticForceModel, ImmersedSurfaceForceModel, SurfaceForceModel, ForceModel>(m, "FastHydrostaticForceModel")
        .def(py::init<const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("body_name"),
            py::arg("env")
        )
        .def("get_name", &FastHydrostaticForceModel::get_name)
        .def_static("model_name", &FastHydrostaticForceModel::model_name,
            "Returns model name \"non-linear hydrostatic (fast)\"")
        ;

    py::class_<ExactHydrostaticForceModel, FastHydrostaticForceModel, ImmersedSurfaceForceModel, SurfaceForceModel, ForceModel>(m, "ExactHydrostaticForceModel",
        "Same as FastHydrostaticForceModel but with the exact application point")
        .def(py::init<const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("body_name"),
            py::arg("env")
        )
        .def_static("model_name", &ExactHydrostaticForceModel::model_name,
            "Returns model name \"non-linear hydrostatic (exact)\"")
        ;

    py::class_<ManeuveringForceModel::Yaml>(m, "ManeuveringForceModelInput",
        "Input for ManeuveringForceModel")
        .def(py::init<
            const std::string& /*name*/,
            const YamlPosition& /*frame_of_reference*/,
            const std::vector<std::string>& /*commands*/,
            const std::map<std::string, std::string> /*var2expr*/
        >(),
            py::arg("name") = "",
            py::arg("frame_of_reference") = YamlPosition(),
            py::arg("commands") = std::vector<std::string>(),
            py::arg("var2expr") = std::map<std::string, std::string>()
            )
        .def_readwrite("name", &ManeuveringForceModel::Yaml::name)
        .def_readwrite("frame_of_reference", &ManeuveringForceModel::Yaml::frame_of_reference)
        .def_readwrite("commands", &ManeuveringForceModel::Yaml::commands)
        .def_readwrite("var2expr", &ManeuveringForceModel::Yaml::var2expr)
        ;

    py::class_<ManeuveringForceModel, ForceModel>(m, "ManeuveringForceModel", "Maneuvering force model")
        .def(py::init<const ManeuveringForceModel::Yaml& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
            py::arg("input_data"),
            py::arg("body_name"),
            py::arg("env")
        )
        .def_static("model_name", &ManeuveringForceModel::model_name, "Returns model name \"maneuvering\"")
        .def_static("parse", &ManeuveringForceModel::parse, py::arg("yaml"))
        .def("get_Tmax", &ManeuveringForceModel::get_Tmax)
        .def("get_force", &ManeuveringForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands"))
        ;
}

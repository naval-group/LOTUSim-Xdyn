#include "py_xdyn_force.hpp"
#include "py_pybind_additions.hpp"
#include "core/inc/ForceModel.hpp"
#include "force_models/inc/AbstractWageningen.hpp"
#include "force_models/inc/DampingForceModel.hpp"
#include "force_models/inc/KtKqForceModel.hpp"
#include "force_models/inc/ConstantForceModel.hpp"
#include "force_models/inc/HydrostaticForceModel.hpp"
#include "force_models/inc/HydroPolarForceModel.hpp"
#include "force_models/inc/GravityForceModel.hpp"
#include "force_models/inc/LinearDampingForceModel.hpp"
#include "force_models/inc/MMGManeuveringForceModel.hpp"
#include "force_models/inc/QuadraticDampingForceModel.hpp"
#include "force_models/inc/RadiationDampingForceModel.hpp"
#include "force_models/inc/ResistanceCurveForceModel.hpp"
#include "force_models/inc/SimpleHeadingKeepingController.hpp"
#include "force_models/inc/SimpleStationKeepingController.hpp"
#include "force_models/inc/WageningenControlledForceModel.hpp"
#include "ssc/ssc/data_source/DataSource.hpp"
#include "ssc/ssc/kinematics/Wrench.hpp"
#include <string>

namespace py = pybind11;

void py_add_module_xdyn_force(py::module& m0)
{
    py::module m = m0.def_submodule("force");

    py::class_<ForceModel>(m, "ForceModel")
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

    py::class_<ConstantForceModel::Input>(m, "ConstantForceModelInput")
        .def(py::init<>())
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

    py::class_<ConstantForceModel, ForceModel>(m, "ConstantForceModel")
        .def(py::init<const ConstantForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def("get_name", &ConstantForceModel::get_name)
        .def("get_body_name", &ConstantForceModel::get_body_name)
        .def("get_force", &ConstantForceModel::get_force)
        .def_static("model_name", &ConstantForceModel::model_name)
        .def_static("parse", &ConstantForceModel::parse)
        ;

    py::class_<GravityForceModel, ForceModel>(m, "GravityForceModel")
        .def(py::init<const std::string&, const EnvironmentAndFrames&>())
        .def_static("model_name", &GravityForceModel::model_name)
        .def("potential_energy", &GravityForceModel::potential_energy)
        .def("get_force", &GravityForceModel::get_force)
        ;

    py::class_<ResistanceCurveForceModel::Yaml>(m, "ResistanceCurveForceModelInput")
        .def(py::init<>())
        .def_readwrite("Va", &ResistanceCurveForceModel::Yaml::Va)
        .def_readwrite("R", &ResistanceCurveForceModel::Yaml::R)
        ;

    py::class_<ResistanceCurveForceModel, ForceModel>(m, "ResistanceCurveForceModel", "Resistance curve given by interpolation table")
        .def(py::init<const ResistanceCurveForceModel::Yaml& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &ResistanceCurveForceModel::model_name)
        .def_static("parse", &ResistanceCurveForceModel::parse)
        .def("get_force", &ResistanceCurveForceModel::get_force,
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        ;

    py::class_<DampingForceModel, ForceModel>(m, "DampingForceModel")
        //.def(py::init<const std::string& /*name*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        //.def("get_force", &DampingForceModel::get_force)
        ;

    py::class_<LinearDampingForceModel, DampingForceModel, ForceModel>(m, "LinearDampingForceModel")
        .def(py::init<const LinearDampingForceModel::Input& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("parse", &LinearDampingForceModel::parse)
        .def_static("model_name", &LinearDampingForceModel::model_name)
        .def("get_force", &LinearDampingForceModel::get_force)
        ;

    py::class_<QuadraticDampingForceModel, DampingForceModel, ForceModel>(m, "QuadraticDampingForceModel")
        .def(py::init<const QuadraticDampingForceModel::Input& /*data*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("parse", &QuadraticDampingForceModel::parse)
        .def_static("model_name", &QuadraticDampingForceModel::model_name)
        ;

    py::class_<MMGManeuveringForceModel::Input>(m, "MMGManeuveringForceModelInput")
        .def(py::init<>())
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

    py::class_<MMGManeuveringForceModel, ForceModel>(m, "MMGManeuveringForceModel")
        .def(py::init<const MMGManeuveringForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def("parse", &MMGManeuveringForceModel::parse)
        .def("model_name", &MMGManeuveringForceModel::model_name)
        .def("get_force", &MMGManeuveringForceModel::get_force)
        ;

    py::class_<HydroPolarForceModel::Input>(m, "HydroPolarForceModelInput")
        .def(py::init<>())
        .def_readwrite("name", &HydroPolarForceModel::Input::name)
        .def_readwrite("internal_frame", &HydroPolarForceModel::Input::internal_frame)
        .def_readwrite("angle_of_attack", &HydroPolarForceModel::Input::angle_of_attack)
        .def_readwrite("lift_coefficient", &HydroPolarForceModel::Input::lift_coefficient)
        .def_readwrite("drag_coefficient", &HydroPolarForceModel::Input::drag_coefficient)
        .def_readwrite("moment_coefficient", &HydroPolarForceModel::Input::moment_coefficient)
        .def_readwrite("reference_area", &HydroPolarForceModel::Input::reference_area)
        .def_readwrite("chord_length", &HydroPolarForceModel::Input::chord_length)
        .def_readwrite("use_waves_velocity", &HydroPolarForceModel::Input::use_waves_velocity)
        ;

    py::class_<HydroPolarForceModel, ForceModel>(m, "HydroPolarForceModel")
        .def(py::init<const HydroPolarForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>(),
             py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>()
             )
        .def("parse", &HydroPolarForceModel::parse)
        .def("model_name", &HydroPolarForceModel::model_name)
        .def("get_force", &HydroPolarForceModel::get_force,
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        ;

    py::class_<AbstractWageningen::Yaml>(m, "AbstractWageningenInput")
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
        .def("parse", &AbstractWageningen::parse)
        .def("advance_ratio", &AbstractWageningen::advance_ratio)
        // Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;
        ;

    py::class_<KtKqForceModel::Yaml, AbstractWageningen::Yaml>(m, "KtKqForceModelInput")
        .def_readwrite("J", &KtKqForceModel::Yaml::J)
        .def_readwrite("Kt", &KtKqForceModel::Yaml::Kt)
        .def_readwrite("Kq", &KtKqForceModel::Yaml::Kq)
        ;

    py::class_<KtKqForceModel, AbstractWageningen, ForceModel>(m, "KtKqForceModel")
        .def(py::init<const KtKqForceModel::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &KtKqForceModel::model_name)
        .def("get_force", &AbstractWageningen::get_force)
        .def("parse", &KtKqForceModel::parse)
        .def("get_Kt", &KtKqForceModel::get_Kt)
        .def("get_Kq", &KtKqForceModel::get_Kq)
        ;

    py::class_<WageningenControlledForceModel::Yaml, AbstractWageningen::Yaml>(m, "WageningenControlledForceModelInput")
        .def_readwrite("number_of_blades", &WageningenControlledForceModel::Yaml::number_of_blades)
        .def_readwrite("blade_area_ratio", &WageningenControlledForceModel::Yaml::blade_area_ratio)
        ;

    py::class_<WageningenControlledForceModel, AbstractWageningen, ForceModel>(m, "WageningenControlledForceModel")
        .def(py::init<const WageningenControlledForceModel::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &WageningenControlledForceModel::model_name)
        .def("get_force", &AbstractWageningen::get_force,
            py::call_guard<py::scoped_ostream_redirect,
                           py::scoped_estream_redirect>()
            )
        .def("parse", &WageningenControlledForceModel::parse)
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

    py::class_<RadiationDampingForceModel::Input>(m, "RadiationDampingForceModelInput")
        .def(py::init<>())
        .def_readwrite("yaml", &RadiationDampingForceModel::Input::yaml)
        // TR1(shared_ptr)<HydroDBParser> parser;
        ;

    py::class_<RadiationDampingForceModel, ForceModel>(m, "RadiationDampingForceModel")
        .def(py::init<const RadiationDampingForceModel::Input& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &RadiationDampingForceModel::model_name)
        .def("get_force", &RadiationDampingForceModel::get_force,
            py::arg("states"),
            py::arg("t"),
            py::arg("env"),
            py::arg("commands")
            )
        .def("get_Tmax", &RadiationDampingForceModel::get_Tmax)
        .def_static("parse", &RadiationDampingForceModel::parse, py::arg("yaml"), py::arg("parse_hdb")=true)
        ;

    py::class_<SimpleHeadingKeepingController::Yaml>(m, "SimpleHeadingKeepingControllerInput")
        .def_readwrite("name", &SimpleHeadingKeepingController::Yaml::name)
        .def_readwrite("ksi", &SimpleHeadingKeepingController::Yaml::ksi)
        .def_readwrite("Tp", &SimpleHeadingKeepingController::Yaml::Tp)
        ;

    py::class_<SimpleHeadingKeepingController, ForceModel>(m, "SimpleHeadingKeepingController")
        .def(py::init<const SimpleHeadingKeepingController::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &SimpleHeadingKeepingController::model_name)
        .def("get_force", &SimpleHeadingKeepingController::get_force)
        .def_static("parse", &SimpleHeadingKeepingController::parse)
        ;

    py::class_<SimpleStationKeepingController::Yaml>(m, "SimpleStationKeepingControllerInput")
        .def_readwrite("name", &SimpleStationKeepingController::Yaml::name)
        .def_readwrite("ksi_x", &SimpleStationKeepingController::Yaml::ksi_x)
        .def_readwrite("T_x", &SimpleStationKeepingController::Yaml::T_x)
        .def_readwrite("ksi_y", &SimpleStationKeepingController::Yaml::ksi_y)
        .def_readwrite("T_y", &SimpleStationKeepingController::Yaml::T_y)
        .def_readwrite("ksi_psi", &SimpleStationKeepingController::Yaml::ksi_psi)
        .def_readwrite("T_psi", &SimpleStationKeepingController::Yaml::T_psi)
        ;

    py::class_<SimpleStationKeepingController, ForceModel>(m, "SimpleStationKeepingController")
        .def(py::init<const SimpleStationKeepingController::Yaml& /*input*/, const std::string& /*body_name*/, const EnvironmentAndFrames& /*env*/>())
        .def_static("model_name", &SimpleStationKeepingController::model_name)
        .def("get_force", &SimpleStationKeepingController::get_force)
        .def_static("parse", &SimpleStationKeepingController::parse)
        ;
}

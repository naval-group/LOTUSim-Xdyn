#include "py_xdyn_core.hpp"
#include "py_pybind_additions.hpp"
#include "core/inc/BodyBuilder.hpp"
#include "core/inc/BodyStates.hpp"
#include "core/inc/EnvironmentAndFrames.hpp"
#include "core/inc/Wrench.hpp"
#include "core/inc/StatesFilter.hpp"
#include "exceptions/inc/ConnexionError.hpp"
#include "exceptions/inc/GRPCError.hpp"
#include "exceptions/inc/InternalErrorException.hpp"
#include "exceptions/inc/InvalidInputException.hpp"
#include "exceptions/inc/MeshException.hpp"
#include "exceptions/inc/NumericalErrorException.hpp"
#include "external_data_structures/inc/YamlAngle.hpp"
#include "external_data_structures/inc/YamlBody.hpp"
#include "external_data_structures/inc/YamlController.hpp"
#include "external_data_structures/inc/YamlCoordinates.hpp"
#include "external_data_structures/inc/YamlDynamics.hpp"
#include "external_data_structures/inc/YamlPoint.hpp"
#include "external_data_structures/inc/YamlPosition.hpp"
#include "external_data_structures/inc/YamlRAO.hpp"
#include "external_data_structures/inc/YamlRadiationDamping.hpp"
#include "external_data_structures/inc/YamlRotation.hpp"
#include "external_data_structures/inc/YamlSpeed.hpp"
#include "external_data_structures/inc/YamlWaveOutput.hpp"
#include "external_data_structures/inc/YamlWaveModelInput.hpp"
namespace py = pybind11;

void py_add_module_xdyn_exceptions(py::module& m);
void py_add_module_xdyn_exceptions(py::module& m)
{
    py::module m_exceptions = m.def_submodule("exceptions");
    py::register_exception<ConnexionError>(m_exceptions, "ConnexionError");
    py::register_exception<GRPCError>(m_exceptions, "GRPCError");
    py::register_exception<InternalErrorException>(m_exceptions, "InternalErrorException");
    py::register_exception<InvalidInputException>(m_exceptions, "InvalidInputException");
    py::register_exception<MeshException>(m_exceptions, "MeshException");
    py::register_exception<NumericalErrorException>(m_exceptions, "NumericalErrorException");
}

void py_add_module_xdyn_core_io(py::module& m);
void py_add_module_xdyn_core_io(py::module& m)
{

    py::class_<YamlAngle>(m, "YamlAngle")
        .def(py::init<>())
        .def(py::init<const double /*phi*/, const double /*theta*/, const double /*psi*/>())
        .def_readwrite("phi", &YamlAngle::phi)
        .def_readwrite("theta", &YamlAngle::theta)
        .def_readwrite("psi", &YamlAngle::psi)
        .def("__repr__",
           [](const YamlAngle &a) {
               std::stringstream ss;
               ss << "{\"phi\":" << a.phi
                  << ",\"theta\":" << a.theta
                  << ",\"psi\":" << a.psi << "}";
                return ss.str();
            })
        ;

    py::class_<YamlCoordinates>(m, "YamlCoordinates")
        .def(py::init<>())
        .def(py::init<const double /*x*/, const double /*y*/, const double /*z*/>())
        .def_readwrite("x", &YamlCoordinates::x, "x")
        .def_readwrite("y", &YamlCoordinates::y, "y")
        .def_readwrite("z", &YamlCoordinates::z, "z")
        .def("__repr__",
           [](const YamlCoordinates &a) {
               std::stringstream ss;
               ss << "{\"x\":" << a.x
                  << ",\"y\":" << a.y
                  << ",\"z\":" << a.z << "}";
                return ss.str();
            })
        ;

    py::class_<YamlRotation>(m, "YamlRotation")
        .def(py::init<>())
        .def(py::init<const std::string& /*order_by_*/, const std::vector<std::string>& /*convention_*/>())
        .def_readwrite("convention", &YamlRotation::convention, "Convention. Use \"angle\"")
        .def_readwrite("order_by", &YamlRotation::order_by, "Order. Use '[\"z\",\"y'\",\"x''\"]\"")
        .def("__repr__",
           [](const YamlRotation &a) {
               std::stringstream ss;
               ss << "{\"order_by\": \"" << a.order_by
                  << "\" ,\"convention\": [\""
                  << a.convention[0] << "\",\""
                  << a.convention[1] << "\",\""
                  << a.convention[2] << "\"]"
                  << "}";
                return ss.str();
            })
        ;

    py::class_<YamlPosition>(m, "YamlPosition")
        .def(py::init<>())
        .def(py::init<const YamlCoordinates& /*c*/, const YamlAngle& /*a*/, const std::string& /*frame*/>())
        .def_readwrite("coordinates", &YamlPosition::coordinates)
        .def_readwrite("angle", &YamlPosition::angle)
        .def_readwrite("frame", &YamlPosition::frame)
        .def_static("Origin", &YamlPosition::Origin)
        ;

    py::class_<YamlController>(m, "YamlController",
        "Stores the controller parameters used to generate a command for a controlled force")
        .def(py::init<>())
        .def_readwrite("type", &YamlController::type, "Type of the controller (PID, gRPC)")
        .def_readwrite("dt", &YamlController::dt, "Time step of the discrete system")
        .def_readwrite("rest_of_the_yaml", &YamlController::rest_of_the_yaml, "All other fields that are spectific to the controller type")
        ;

    py::class_<YamlDOF<std::string> >(m, "YamlCSVString")
        .def(py::init<>())
        .def_readwrite("state", &YamlDOF<std::string>::state)
        .def_readwrite("t", &YamlDOF<std::string>::t)
        .def_readwrite("value", &YamlDOF<std::string>::value)
        .def_readwrite("interpolation", &YamlDOF<std::string>::interpolation)
        ;

    py::class_<YamlDOF<double> >(m, "YamlCSVDouble")
        .def(py::init<>())
        .def_readwrite("state", &YamlDOF<double>::state)
        .def_readwrite("t", &YamlDOF<double>::t)
        .def_readwrite("value", &YamlDOF<double>::value)
        .def_readwrite("interpolation", &YamlDOF<double>::interpolation)
        ;

    py::class_<YamlCSVDOF, YamlDOF<std::string> >(m, "YamlCSVDOF")
        .def(py::init<>())
        .def_readwrite("filename", &YamlCSVDOF::filename)
        ;

    py::class_<YamlBlockedDOF>(m, "YamlBlockedDOF")
        .def(py::init<>())
        .def_readwrite("from_yaml", &YamlBlockedDOF::from_yaml)
        .def_readwrite("from_csv", &YamlBlockedDOF::from_csv)
        ;

    py::class_<YamlBody>(m, "YamlBody")
        .def(py::init<>())
        .def_readwrite("name", &YamlBody::name)
        .def_readwrite("mesh", &YamlBody::mesh)
        .def_readwrite("position_of_body_frame_relative_to_mesh", &YamlBody::position_of_body_frame_relative_to_mesh)
        .def_readwrite("initial_position_of_body_frame_relative_to_NED_projected_in_NED", &YamlBody::initial_position_of_body_frame_relative_to_NED_projected_in_NED)
        .def_readwrite("initial_velocity_of_body_frame_relative_to_NED_projected_in_body", &YamlBody::initial_velocity_of_body_frame_relative_to_NED_projected_in_body)
        .def_readwrite("dynamics", &YamlBody::dynamics)
        .def_readwrite("external_forces", &YamlBody::external_forces)
        .def_readwrite("blocked_dof", &YamlBody::blocked_dof)
        .def_readwrite("filtered_states", &YamlBody::filtered_states)
        ;

    py::class_<YamlFilteredStates>(m, "YamlFilteredStates")
        .def(py::init<>())
        .def_readwrite("x", &YamlFilteredStates::x)
        .def_readwrite("y", &YamlFilteredStates::y)
        .def_readwrite("z", &YamlFilteredStates::z)
        .def_readwrite("u", &YamlFilteredStates::u)
        .def_readwrite("v", &YamlFilteredStates::v)
        .def_readwrite("w", &YamlFilteredStates::w)
        .def_readwrite("p", &YamlFilteredStates::p)
        .def_readwrite("q", &YamlFilteredStates::q)
        .def_readwrite("r", &YamlFilteredStates::r)
        .def_readwrite("phi", &YamlFilteredStates::phi)
        .def_readwrite("theta", &YamlFilteredStates::theta)
        .def_readwrite("psi", &YamlFilteredStates::psi)
        ;

    py::enum_<YamlRAO::TypeOfRao>(m, "TypeOfRao", py::arithmetic())
        .value("DIFFRACTION_RAO", YamlRAO::TypeOfRao::DIFFRACTION_RAO)
        .value("FROUDE_KRYLOV_RAO", YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO);

    py::class_<YamlRAO>(m, "YamlRAO")
        .def(py::init<>())
        .def_readwrite("hdb_filename", &YamlRAO::hdb_filename)
        .def_readwrite("precal_filename", &YamlRAO::precal_filename)
        .def_readwrite("calculation_point", &YamlRAO::calculation_point)
        .def_readwrite("mirror", &YamlRAO::mirror)
        .def_readwrite("use_encounter_period", &YamlRAO::use_encounter_period)
        .def_readwrite("type_of_rao", &YamlRAO::type_of_rao)
        ;

    py::enum_<TypeOfQuadrature>(m, "TypeOfQuadrature", py::arithmetic())
        .value("RECTANGLE", TypeOfQuadrature::RECTANGLE)
        .value("TRAPEZOIDAL", TypeOfQuadrature::TRAPEZOIDAL)
        .value("SIMPSON", TypeOfQuadrature::SIMPSON)
        .value("GAUSS_KRONROD", TypeOfQuadrature::GAUSS_KRONROD)
        .value("BURCHER", TypeOfQuadrature::BURCHER)
        .value("CLENSHAW_CURTIS", TypeOfQuadrature::CLENSHAW_CURTIS)
        .value("FILON", TypeOfQuadrature::FILON)
        ;

    py::class_<YamlRadiationDamping>(m, "YamlRadiationDamping")
        .def_readwrite("hdb_filename", &YamlRadiationDamping::hdb_filename)
        .def_readwrite("precal_r_filename", &YamlRadiationDamping::precal_r_filename)
        .def_readwrite("type_of_quadrature_for_cos_transform", &YamlRadiationDamping::type_of_quadrature_for_cos_transform)
        .def_readwrite("type_of_quadrature_for_convolution", &YamlRadiationDamping::type_of_quadrature_for_convolution)
        .def_readwrite("nb_of_points_for_retardation_function_discretization", &YamlRadiationDamping::nb_of_points_for_retardation_function_discretization)
        .def_readwrite("omega_min", &YamlRadiationDamping::omega_min)
        .def_readwrite("omega_max", &YamlRadiationDamping::omega_max)
        .def_readwrite("tau_min", &YamlRadiationDamping::tau_min)
        .def_readwrite("tau_max", &YamlRadiationDamping::tau_max)
        .def_readwrite("output_Br_and_K", &YamlRadiationDamping::output_Br_and_K)
        .def_readwrite("calculation_point_in_body_frame", &YamlRadiationDamping::calculation_point_in_body_frame)
        .def_readwrite("remove_constant_speed", &YamlRadiationDamping::remove_constant_speed)
        .def_readwrite("forward_speed_correction", &YamlRadiationDamping::forward_speed_correction)
        ;
}


void py_add_module_xdyn_core(py::module& m0)
{
    py_add_module_xdyn_exceptions(m0);
    py::module m = m0.def_submodule("core");
    py::module m_core_io = m.def_submodule("io");
    py_add_module_xdyn_core_io(m_core_io);

    py::class_<EnvironmentAndFrames>(m, "EnvironmentAndFrames")
        .def(py::init<>())
        .def_readwrite("rho", &EnvironmentAndFrames::rho)
        .def_readwrite("nu", &EnvironmentAndFrames::nu)
        .def_readwrite("g", &EnvironmentAndFrames::g)
        .def_readwrite("rot", &EnvironmentAndFrames::rot)
        .def_readwrite("k", &EnvironmentAndFrames::k)
        .def("k_add",
           [](const EnvironmentAndFrames &a, const ssc::kinematics::Transform& ssct) {
               a.k->add(ssct);
            }
        )
        .def("set_rho_air", &EnvironmentAndFrames::set_rho_air)
        .def("get_rho_air", &EnvironmentAndFrames::get_rho_air);

    py::class_<History>(m, "History")
        .def(py::init<>())
        .def(py::init<const double /*Tmax=0*/>())
        .def("average", &History::average,
        R"(
        Returns the average value integrated between t-length and t, t being the current instant.
        A trapezoidal integration is used.
        Returns Value at t-tau in history.
        )")
        .def("record", &History::record, "Adds a value to history")
        .def("size", &History::size, "Number of points in history")
        .def("get_Tmax", &History::get_Tmax, "Accessor for Tmax")
        .def("get_duration", &History::get_duration, "How far back in history can we currently go")
        .def("reset", &History::reset, "Reset the content of the object")
        .def("get_values", &History::get_values, "Return values stored")
        .def("get_dates", &History::get_dates, "Return the list of dates stored")
        .def("get_current_time", &History::get_current_time, "Return the last record timestamp")
        .def("__call__", &History::operator(), py::arg("tau") = 0.0, "Returns the value at t-tau, t being the current instant")
        .def("__getitem__", &History::operator[], "Get direct access to a (time, value) tuple of history")
        ;


    py::class_<BodyPtr>(m, "BodyPtr")
        .def("get_states",
           [](const BodyPtr &a) -> BodyStates {
               return a->get_states();
            }
        )
        .def("update",
           [](const BodyPtr &a, const EnvironmentAndFrames& env, const StateType& x, const double t) {
               return a->update(env, x, t);
            }
        )
        .def("set_history",
           [](const BodyPtr &a, const EnvironmentAndFrames& env, const State& states) {
               return a->set_history(env, states);
            }
        )
        .def("update_kinematics",
           [](const BodyPtr &a, const StateType& x, const ssc::kinematics::KinematicsPtr& k) {
               return a->update_kinematics(x, k);
            }
        )
        .def("update_body_states",
           [](const BodyPtr &a, StateType x, const double t) {
               return a->update_body_states(x, t);
            }
        )
        .def("force_states",
           [](const BodyPtr &a, StateType& x, const double t) {
               return a->force_states(x, t);
            }
        )

        ;
    py::class_<BodyBuilder>(m, "BodyBuilder")
        .def(py::init<const YamlRotation& /*convention*/>())
        .def("build" ,
            static_cast<BodyPtr (BodyBuilder::*)(
                const YamlBody& /*input*/,
                const VectorOfVectorOfPoints& /*mesh*/,
                const size_t /*idx*/,
                const double /*t0*/,
                const YamlRotation& /*convention*/,
                const double /*Tmax*/,
                const bool /*has_surface_forces = false*/
            ) const>(&BodyBuilder::build),
                py::arg("input"),
                py::arg("mesh"),
                py::arg("idx"),
                py::arg("t0"),
                py::arg("convention"),
                py::arg("Tmax"),
                py::arg("has_surface_forces") = false,
                "Build a 'Body' object from YAML & STL data")
        .def("build_for_test" ,
            static_cast<BodyPtr (BodyBuilder::*)(
                const std::string& /*name*/,
                const VectorOfVectorOfPoints& /*mesh*/,
                const size_t /*idx*/,
                const double /*t0*/,
                const YamlRotation& /*convention*/,
                const double /*Tmax*/,
                const bool /*has_surface_forces = false*/
            ) const>(&BodyBuilder::build),
                py::arg("name"),
                py::arg("mesh"),
                py::arg("idx"),
                py::arg("t0"),
                py::arg("convention"),
                py::arg("Tmax"),
                py::arg("has_surface_forces") = false,
            R"(Only used for testing purposes when we don't want to go
               through the hassle of defining the inertia matrix & initial
               positions)")
        ;

    py::class_<BodyStates>(m, "BodyStates")
        .def(py::init<const double>(), py::arg("Tmax") = 0.0)
        .def_readwrite("name", &BodyStates::name, "Body's name")
        .def_readwrite("G", &BodyStates::G, "Position of the ship's centre of gravity")
        .def_readwrite("x_relative_to_mesh", &BodyStates::x_relative_to_mesh)
        .def_readwrite("y_relative_to_mesh", &BodyStates::y_relative_to_mesh)
        .def_readwrite("z_relative_to_mesh", &BodyStates::z_relative_to_mesh)
        .def_readwrite("convention", &BodyStates::convention)
        .def("get_current_state_values", &BodyStates::get_current_state_values)
        .def("get_total_inertia",
           [](BodyStates &a) {return  Eigen::Ref<Eigen::Matrix<double,6,6> >(a.total_inertia);}, py::return_value_policy::reference_internal)
        .def("get_solid_body_inertia",
            [](BodyStates &a) {return  Eigen::Ref<Eigen::Matrix<double,6,6> >(a.solid_body_inertia);}, py::return_value_policy::reference_internal)
        .def("get_inverse_of_the_total_inertia",
            [](BodyStates &a) {return  Eigen::Ref<Eigen::Matrix<double,6,6> >(a.inverse_of_the_total_inertia);}, py::return_value_policy::reference_internal)
        // .def_readwrite("total_inertia", &BodyStates::total_inertia, "COPY of 6x6 rigid body inertia matrix (i.e. without added mass) in the body frame")
        // .def_readwrite("solid_body_inertia", &BodyStates::solid_body_inertia, "COPY of 6x6 rigid body inertia matrix (i.e. without added mass) in the body frame")
        // .def_readwrite("inverse_of_the_total_inertia", &BodyStates::inverse_of_the_total_inertia, "COPY of the inverse of the total inertia")
        .def_readwrite("x", &BodyStates::x)
        .def_readwrite("y", &BodyStates::y)
        .def_readwrite("z", &BodyStates::z)
        .def_readwrite("u", &BodyStates::u)
        .def_readwrite("v", &BodyStates::v)
        .def_readwrite("w", &BodyStates::w)
        .def_readwrite("p", &BodyStates::p)
        .def_readwrite("q", &BodyStates::q)
        .def_readwrite("r", &BodyStates::r)
        .def_readwrite("qr", &BodyStates::qr)
        .def_readwrite("qi", &BodyStates::qi)
        .def_readwrite("qj", &BodyStates::qj)
        .def_readwrite("qk", &BodyStates::qk)
        .def_readwrite("states_filter", &BodyStates::states_filter)
        .def("get_angles",static_cast<ssc::kinematics::EulerAngles (BodyStates::*)() const>(&BodyStates::get_angles))
        .def_static("convert_to_euler_angle",
            static_cast<ssc::kinematics::EulerAngles (*)(
                const ssc::kinematics::RotationMatrix&/*R*/, const YamlRotation& /*rotations*/
            )>(&BodyStates::convert))
        .def_static("convert_to_quaternion",
            static_cast<std::tuple<double,double,double,double> (*)(
                const ssc::kinematics::EulerAngles& /*R*/, const YamlRotation& /*rotations*/
            )>(&BodyStates::convert))
        .def("get_rot_from_ned_to_body", &BodyStates::get_rot_from_ned_to_body)
        .def("get_rot_from_ned_to", &BodyStates::get_rot_from_ned_to)
        ;

    py::class_<StatesFilter>(m, "StatesFilter")
        .def(py::init<const YamlFilteredStates& /*input*/>())
        .def("get_Tmax", &StatesFilter::get_Tmax)
        .def("get_filtered_x", &StatesFilter::get_filtered_x)
        .def("get_filtered_y", &StatesFilter::get_filtered_y)
        .def("get_filtered_z", &StatesFilter::get_filtered_z)
        .def("get_filtered_u", &StatesFilter::get_filtered_u)
        .def("get_filtered_v", &StatesFilter::get_filtered_v)
        .def("get_filtered_w", &StatesFilter::get_filtered_w)
        .def("get_filtered_p", &StatesFilter::get_filtered_p)
        .def("get_filtered_q", &StatesFilter::get_filtered_q)
        .def("get_filtered_r", &StatesFilter::get_filtered_r)
        .def("get_filtered_qr", &StatesFilter::get_filtered_qr)
        .def("get_filtered_qi", &StatesFilter::get_filtered_qi)
        .def("get_filtered_qj", &StatesFilter::get_filtered_qj)
        .def("get_filtered_qk", &StatesFilter::get_filtered_qk)
        .def("get_filtered_phi", &StatesFilter::get_filtered_phi)
        .def("get_filtered_theta", &StatesFilter::get_filtered_theta)
        .def("get_filtered_psi", &StatesFilter::get_filtered_psi)
        ;

    py::class_<YamlSpeed>(m, "YamlSpeed")
        .def(py::init<>())
        .def_readwrite("u", &YamlSpeed::u)
        .def_readwrite("v", &YamlSpeed::v)
        .def_readwrite("w", &YamlSpeed::w)
        .def_readwrite("p", &YamlSpeed::p)
        .def_readwrite("q", &YamlSpeed::q)
        .def_readwrite("r", &YamlSpeed::r)
        ;

    py::class_<YamlPoint, YamlCoordinates>(m, "YamlPoint")
        .def(py::init<>())
        .def_readwrite("name", &YamlPoint::name)
        .def_readwrite("frame", &YamlPoint::frame)
        ;

    py::class_<YamlDynamics>(m, "YamlDynamics")
        .def(py::init<>())
        .def_readwrite("centre_of_inertia", &YamlDynamics::centre_of_inertia)
        .def_readwrite("rigid_body_inertia", &YamlDynamics::rigid_body_inertia)
        .def_readwrite("added_mass", &YamlDynamics::added_mass)
        .def_readwrite("hydrodynamic_forces_calculation_point_in_body_frame", &YamlDynamics::hydrodynamic_forces_calculation_point_in_body_frame)
        ;

    py::enum_<InterpolationType>(m, "InterpolationType", py::arithmetic())
        .value("PIECEWISE_CONSTANT", InterpolationType::PIECEWISE_CONSTANT)
        .value("LINEAR", InterpolationType::LINEAR)
        .value("SPLINE", InterpolationType::SPLINE);

    py::enum_<BlockableState>(m, "BlockableState", py::arithmetic())
        .value("U", BlockableState::U)
        .value("V", BlockableState::V)
        .value("W", BlockableState::W)
        .value("P", BlockableState::P)
        .value("Q", BlockableState::Q)
        .value("R", BlockableState::R)
        ;

    py::enum_<FilterableState>(m, "FilterableState", py::arithmetic())
        .value("X", FilterableState::X)
        .value("Y", FilterableState::Y)
        .value("Z", FilterableState::Z)
        .value("U", FilterableState::U)
        .value("V", FilterableState::V)
        .value("W", FilterableState::W)
        .value("P", FilterableState::P)
        .value("Q", FilterableState::Q)
        .value("R", FilterableState::R)
        .value("PHI", FilterableState::PHI)
        .value("THETA", FilterableState::THETA)
        .value("PSI", FilterableState::PSI)
        ;

    py::class_<Wrench>(m, "Wrench")
        .def(py::init<const ssc::kinematics::Wrench&>())
        .def("get_point", &Wrench::get_point)
        .def("get_frame", &Wrench::get_frame)
        .def("get_force", &Wrench::get_force)
        .def("get_torque", &Wrench::get_torque)
        .def_readwrite("force", &Wrench::force)
        .def_readwrite("torque", &Wrench::torque)
        .def("X", static_cast<double (Wrench::*)() const>(&Wrench::X), "Get X")
        .def("Y", static_cast<double (Wrench::*)() const>(&Wrench::Y), "Get Y")
        .def("Z", static_cast<double (Wrench::*)() const>(&Wrench::Z), "Get Z")
        .def("K", static_cast<double (Wrench::*)() const>(&Wrench::K), "Get K")
        .def("M", static_cast<double (Wrench::*)() const>(&Wrench::M), "Get M")
        .def("N", static_cast<double (Wrench::*)() const>(&Wrench::N), "Get N")
        ;
}
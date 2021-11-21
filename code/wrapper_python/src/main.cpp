#include <pybind11/pybind11.h>
#include <pybind11/cast.h>
#include <pybind11/eigen.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <Eigen/Dense>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <boost/optional.hpp>

// Code to handle boost::optional usage in force_models
namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<boost::optional<T>> : optional_caster<boost::optional<T>> {};
}}
#include "demo.hpp"
#include "core/inc/BodyBuilder.hpp"
#include "core/inc/BodyStates.hpp"
#include "core/inc/EnvironmentAndFrames.hpp"
#include "core/inc/ForceModel.hpp"
#include "core/inc/Wrench.hpp"
#include "core/inc/StatesFilter.hpp"
#include "external_data_structures/inc/YamlAngle.hpp"
#include "external_data_structures/inc/YamlBody.hpp"
#include "external_data_structures/inc/YamlController.hpp"
#include "external_data_structures/inc/YamlCoordinates.hpp"
#include "external_data_structures/inc/YamlDynamics.hpp"
#include "external_data_structures/inc/YamlPoint.hpp"
#include "external_data_structures/inc/YamlPosition.hpp"
#include "external_data_structures/inc/YamlRotation.hpp"
#include "external_data_structures/inc/YamlSpeed.hpp"
#include "hdb_interpolators/inc/History.hpp"
#include "hdb_interpolators/inc/PrecalParser.hpp"
#include "hdb_interpolators/inc/TimestampedMatrix.hpp"
#include "force_models/inc/AbstractWageningen.hpp"
#include "force_models/inc/KtKqForceModel.hpp"
#include "force_models/inc/ConstantForceModel.hpp"
#include "force_models/inc/HydrostaticForceModel.hpp"
#include "force_models/inc/HydroPolarForceModel.hpp"
#include "force_models/inc/GravityForceModel.hpp"
#include "force_models/inc/MMGManeuveringForceModel.hpp"
#include "force_models/inc/WageningenControlledForceModel.hpp"
#include "ssc/ssc/data_source/DataSource.hpp"
#include "ssc/ssc/kinematics/coriolis_and_centripetal.hpp"
#include "ssc/ssc/kinematics/EulerAngles.hpp"
#include "ssc/ssc/kinematics/Kinematics.hpp"
#include "ssc/ssc/kinematics/KinematicTree.hpp"
#include "ssc/ssc/kinematics/Point.hpp"
#include "ssc/ssc/kinematics/PointMatrix.hpp"
#include "ssc/ssc/kinematics/RotationMatrix.hpp"
#include "ssc/ssc/kinematics/Transform.hpp"
#include "ssc/ssc/kinematics/Velocity.hpp"
#include "ssc/ssc/kinematics/Wrench.hpp"
#include "test_data_generator/inc/TriMeshTestData.hpp"
#include "test_data_generator/inc/yaml_data.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }
};

std::string call_go(Animal *animal);
std::string call_go(Animal *animal) {
    return animal->go(3);
}

class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERRIDE_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }
};


// Duplicated code from core/unit_tests and executables/src/test_hs.cpp
BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points);
BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points)
{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points, 0, 0.0, rot, 0.0, false);
}

class PyForceModel: public ForceModel
{
public:
    /* Inherit the constructors */
    using ForceModel::ForceModel;

    /* Trampoline (need one for each virtual function) */
    /*
    Wrench get_force(
                const BodyStates& states, const double t,
                const EnvironmentAndFrames& env,
                const std::map<std::string,double>& commands) override {
        PYBIND11_OVERRIDE_PURE(
            Wrench, // Return type
            ForceModel,      // Parent class
            get_force,          // Name of function in C++ (must match Python name)
            states,      // Argument(s)
            t,      // Argument(s)
            env,      // Argument(s)
            commands
        );
    }
    */
};

/*
PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
}
*/

PYBIND11_MODULE(xdyn, m) {
    m.doc() = R"pbdoc(
        xdyn
        ----

        .. currentmodule:: xdyn

        .. autosummary::
           :toctree: _generate
    )pbdoc";

    py::add_ostream_redirect(m, "ostream_redirect");

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    py::class_<Animal, PyAnimal>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());

    m.def("call_go", &call_go);

    py::module m_ssc = m.def_submodule("ssc");

    py::module m_ssc_data_source = m_ssc.def_submodule("datasource");

    py::class_<ssc::data_source::DataSource>(m_ssc_data_source, "DataSource")
        .def(py::init<>())
        ;

    py::module m_ssc_kinematics = m_ssc.def_submodule("kinematics");

    py::class_<ssc::kinematics::Point>(m_ssc_kinematics, "Point")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def(py::init<const ssc::kinematics::Point&>())
        .def(py::init<const std::string&, const Eigen::Vector3d&>())
        .def(py::init<const std::string&, const double, const double, const double>())
        .def("get_frame", &ssc::kinematics::Point::get_frame)
        .def("x", static_cast<double (ssc::kinematics::Point::*)() const>(&ssc::kinematics::Point::x), "Get X")
        .def("y", static_cast<double (ssc::kinematics::Point::*)() const>(&ssc::kinematics::Point::y), "Get Y")
        .def("z", static_cast<double (ssc::kinematics::Point::*)() const>(&ssc::kinematics::Point::z), "Get Z")
        .def_readwrite("v", &ssc::kinematics::Point::v, py::return_value_policy::reference_internal);

    py::class_<ssc::kinematics::Wrench>(m_ssc_kinematics, "Wrench",
        "Stores a force & a torque, projected in a given reference frame.")
        .def(py::init<>())
        .def("get_frame",&ssc::kinematics::Wrench::get_frame, "The reference frame in which the Wrench coordinates are expressed")
        .def_readwrite("force", &ssc::kinematics::Wrench::force, py::return_value_policy::reference_internal)
        .def_readwrite("torque", &ssc::kinematics::Wrench::torque, py::return_value_policy::reference_internal)
        .def("X", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::X), "Get X")
        .def("Y", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::Y), "Get Y")
        .def("Z", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::Z), "Get Z")
        .def("K", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::K), "Get K")
        .def("M", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::M), "Get M")
        .def("N", static_cast<double (ssc::kinematics::Wrench::*)() const>(&ssc::kinematics::Wrench::N), "Get N")
        ;

    py::class_<ssc::kinematics::EulerAngles>(m_ssc_kinematics, "EulerAngles")
        .def(py::init<>())
        .def(py::init<const double, const double, const double>())
        .def_readwrite("phi", &ssc::kinematics::EulerAngles::phi, "Phi in radian")
        .def_readwrite("theta", &ssc::kinematics::EulerAngles::theta, "Theta in radian")
        .def_readwrite("psi", &ssc::kinematics::EulerAngles::psi, "Psi in radian")
        .def("__repr__",
           [](const ssc::kinematics::EulerAngles &a) {
               std::stringstream ss;
               ss << "{\"phi\":" << a.phi
                  << ",\"theta\":" << a.theta
                  << ",\"psi\":" << a.psi << "}";
                return ss.str();
            }
        );

    py::class_<ssc::kinematics::Kinematics>(m_ssc_kinematics, "Kinematics",
        "Lazily computes the optimal transform between two reference frames.")
        .def(py::init<>())
        .def("add",&ssc::kinematics::Kinematics::add, "Adds a transform between two reference frames")
        .def("get",&ssc::kinematics::Kinematics::get, "Returns the transform from one frame to another (or throws a KinematicsException).");

    py::class_<ssc::kinematics::KinematicsPtr>(m_ssc_kinematics, "KinematicsPtr");

    py::class_<ssc::kinematics::KinematicTree>(m_ssc_kinematics, "KinematicTree")
        .def(py::init<>())
        .def("add",&ssc::kinematics::KinematicTree::add)
        .def("get_path",&ssc::kinematics::KinematicTree::get_path);

    py::class_<ssc::kinematics::PointMatrix>(m_ssc_kinematics, "PointMatrix")
        .def(py::init<>())
        .def(py::init<const std::string& /*frame*/, const size_t /*nb_of_columns*/>())
        .def(py::init<const ssc::kinematics::Matrix3Xd& /*m*/,const std::string& /*frame*/>())
        .def_readwrite("m",&ssc::kinematics::PointMatrix::m);

    py::class_<ssc::kinematics::RotationMatrix>(m_ssc_kinematics, "RotationMatrix");

    py::class_<ssc::kinematics::Transform>(m_ssc_kinematics, "Transform")
        .def(py::init<>())
        .def(py::init<const ssc::kinematics::Point& /*translation*/, const ssc::kinematics::RotationMatrix& /*rotation*/, const std::string& /*to_frame*/>())
        .def(py::init<const ssc::kinematics::Point& /*translation*/, const std::string& /*to_frame*/>())
        .def(py::init<const ssc::kinematics::RotationMatrix& /*rotation*/, const std::string& /*from_frame*/, const std::string& /*to_frame*/>())
        .def("get_from_frame",&ssc::kinematics::Transform::get_from_frame)
        .def("get_to_frame",&ssc::kinematics::Transform::get_to_frame)
        .def("inverse",&ssc::kinematics::Transform::inverse)
        .def("get_point",&ssc::kinematics::Transform::get_point)
        .def("get_rot",&ssc::kinematics::Transform::get_rot);

    py::class_<ssc::kinematics::Velocity>(m_ssc_kinematics, "Velocity")
        .def(py::init<const ssc::kinematics::Point& /*p*/, const ssc::kinematics::AngularVelocityVector& /*w*/>())
        .def(py::init<const ssc::kinematics::Point& /*p*/, const ssc::kinematics::TranslationVelocityVector& /*t*/, const ssc::kinematics::AngularVelocityVector& /*w*/>())
        .def("get_rot",&ssc::kinematics::Velocity::get_frame)
        .def("get_point",&ssc::kinematics::Velocity::get_point)
        .def("get_angular_velocity",&ssc::kinematics::Velocity::get_angular_velocity)
        .def("get_translation_velocity",&ssc::kinematics::Velocity::get_translation_velocity)
        .def("change_point",&ssc::kinematics::Velocity::change_point)
        ;

    m_ssc_kinematics.def("coriolis_and_centripetal", &ssc::kinematics::coriolis_and_centripetal,
         R"(
        These are the forces due to the fact that the equation of Newton's
        second law of motion is written in a non-Gallilean frame: hence its speed
        must be taken into account. The inertia matrix provided is the
        rigid body inertia. It should, in general, not be the added mass matrix or
        the sum of the added mass matrix & the rigid-body inertia matrix because
        the added mass forces are experimentally found not to be proportional to
        the acceleration but they are proportional to the derivative of the projection
        in the body frame of the linear velocity of the body with respect to the
        (Gallilean) earth frame.)");

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
        Returns Value at t-tau in history
        )")
        .def("record", &History::record, "Adds a value to history")
        .def("size", &History::size, "Number of points in history")
        .def("get_Tmax", &History::get_Tmax, "Accessor for Tmax")
        .def("get_duration", &History::get_duration, "How far back in history can we currently go")
        .def("reset", &History::reset, "Reset the content of the object")
        .def("get_values", &History::get_values, "Return values stored")
        .def("get_dates", &History::get_dates, "Return the list of dates stored")
        .def("get_current_time", &History::get_current_time, "Return the last record timestamp")
        .def("__call__", &History::operator(), "Returns the value at t-tau, t being the current instant")
        .def("__getitem__", &History::operator[], "Get direct access to a (time, value) tuple of history")
        ;

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
        .def("get_force", &GravityForceModel::get_force);

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
        .def("get_force", &HydroPolarForceModel::get_force)
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
        .def("get_force", &AbstractWageningen::get_force)
        .def("parse", &WageningenControlledForceModel::parse)
        .def("get_Kt", &WageningenControlledForceModel::get_Kt)
        .def("get_Kq", &WageningenControlledForceModel::get_Kq)
        ;

    py::module m_hdb_interpolators = m.def_submodule("hdbinterpolators");

    py::class_<RAOData>(m_hdb_interpolators, "RAOData")
        .def(py::init<>())
        .def_readwrite("periods", &RAOData::periods)
        .def_readwrite("psi", &RAOData::psi)
        .def_readwrite("values", &RAOData::values);

    py::module m_data = m.def_submodule("data");
    py::module m_data_body = m_data.def_submodule("body");
    py::module m_data_mesh = m_data.def_submodule("mesh");
    py::module m_data_yaml = m_data.def_submodule("yaml");
    m_data_body.def("get_body", &get_body,
        py::arg("name"),
        py::arg("points") = two_triangles()
        );
    m_data_mesh.def("one_triangle",&one_triangle);
    m_data_mesh.def("one_triangle_clockwise",&one_triangle_clockwise);
    m_data_mesh.def("degenerated_triangle",&degenerated_triangle);
    m_data_mesh.def("unit_cube",&unit_cube);
    m_data_mesh.def("unit_cube_with_incorrect_orientation",&unit_cube_with_incorrect_orientation);
    m_data_mesh.def("unit_cube_clockwise",&unit_cube_clockwise);
    m_data_mesh.def("two_triangles",&two_triangles);
    m_data_mesh.def("two_triangles_immerged",&two_triangles_immerged);
    m_data_mesh.def("trapezium",&trapezium);
    m_data_mesh.def("n_gone",&n_gone);
    m_data_mesh.def("cube",&cube);
    m_data_mesh.def("tetrahedron",&tetrahedron);
    m_data_mesh.def("tetrahedron_clockwise",&tetrahedron_clockwise);
    m_data_mesh.def("generated_stl",&generated_stl);
    m_data_mesh.def("L",&L);
    m_data_mesh.def("U",&U);
    m_data_yaml.def("bug_2655", &test_data::bug_2655);
    m_data_yaml.def("hydrostatic_test", &test_data::hydrostatic_test);
    m_data_yaml.def("added_mass_from_hdb_file", &test_data::added_mass_from_hdb_file);
    m_data_yaml.def("full_example", &test_data::full_example);
    m_data_yaml.def("full_example_with_diagonal_inertia", &test_data::full_example_with_diagonal_inertia);
    m_data_yaml.def("full_example_with_propulsion", &test_data::full_example_with_propulsion);
    m_data_yaml.def("full_example_with_propulsion_and_old_key_name", &test_data::full_example_with_propulsion_and_old_key_name);
    m_data_yaml.def("falling_ball_example", &test_data::falling_ball_example);
    m_data_yaml.def("oscillating_cube_example", &test_data::oscillating_cube_example);
    m_data_yaml.def("new_oscillating_cube_example", &test_data::new_oscillating_cube_example);
    m_data_yaml.def("stable_cube_example", &test_data::stable_cube_example);
    m_data_yaml.def("stable_rolling_cube_test", &test_data::stable_rolling_cube_test);
    m_data_yaml.def("test_ship_hydrostatic_test", &test_data::test_ship_hydrostatic_test);
    m_data_yaml.def("test_ship_exact_hydrostatic_test", &test_data::test_ship_exact_hydrostatic_test);
    m_data_yaml.def("test_ship_fast_hydrostatic_test", &test_data::test_ship_fast_hydrostatic_test);
    m_data_yaml.def("test_ship_new_hydrostatic_test", &test_data::test_ship_new_hydrostatic_test);
    m_data_yaml.def("test_ship_propulsion", &test_data::test_ship_propulsion);
    m_data_yaml.def("test_ship_waves_test", &test_data::test_ship_waves_test);
    m_data_yaml.def("test_ship_diffraction", &test_data::test_ship_diffraction);
    m_data_yaml.def("waves", &test_data::waves);
    m_data_yaml.def("simple_waves", &test_data::simple_waves);
    m_data_yaml.def("cube_in_waves", &test_data::cube_in_waves);
    m_data_yaml.def("waves_for_parser_validation_only", &test_data::waves_for_parser_validation_only);
    m_data_yaml.def("test_ship_froude_krylov", &test_data::test_ship_froude_krylov);
    m_data_yaml.def("falling_cube", &test_data::falling_cube);
    m_data_yaml.def("rolling_cube", &test_data::rolling_cube);
    m_data_yaml.def("test_ship_damping", &test_data::test_ship_damping);
    m_data_yaml.def("controlled_forces", &test_data::controlled_forces);
    m_data_yaml.def("bug_2961", &test_data::bug_2961);
    m_data_yaml.def("setpoints", &test_data::setpoints);
    m_data_yaml.def("unknown_controller", &test_data::unknown_controller);
    m_data_yaml.def("grpc_controller", &test_data::grpc_controller);
    m_data_yaml.def("controllers", &test_data::controllers);
    m_data_yaml.def("wageningen", &test_data::wageningen);
    m_data_yaml.def("resistance_curve", &test_data::resistance_curve);
    m_data_yaml.def("propulsion_and_resistance", &test_data::propulsion_and_resistance);
    m_data_yaml.def("dummy_controllers_and_commands_for_propulsion_and_resistance", &test_data::dummy_controllers_and_commands_for_propulsion_and_resistance);
    m_data_yaml.def("heading_keeping_base", &test_data::heading_keeping_base);
    m_data_yaml.def("heading_keeping_controllers", &test_data::heading_keeping_controllers);
    m_data_yaml.def("radiation_damping", &test_data::radiation_damping);
    m_data_yaml.def("diffraction", &test_data::diffraction);
    m_data_yaml.def("diffraction_precalr", &test_data::diffraction_precalr);
    m_data_yaml.def("test_ship_radiation_damping", &test_data::test_ship_radiation_damping);
    m_data_yaml.def("simple_track_keeping", &test_data::simple_track_keeping);
    m_data_yaml.def("maneuvering_commands", &test_data::maneuvering_commands);
    m_data_yaml.def("maneuvering_with_same_frame_of_reference", &test_data::maneuvering_with_same_frame_of_reference);
    m_data_yaml.def("maneuvering", &test_data::maneuvering);
    m_data_yaml.def("bug_2641", &test_data::bug_2641);
    m_data_yaml.def("simple_station_keeping", &test_data::simple_station_keeping);
    m_data_yaml.def("rudder", &test_data::rudder);
    m_data_yaml.def("bug_in_exact_hydrostatic", &test_data::bug_in_exact_hydrostatic);
    m_data_yaml.def("bug_2714_heading_keeping", &test_data::bug_2714_heading_keeping);
    m_data_yaml.def("bug_2714_station_keeping", &test_data::bug_2714_station_keeping);
    m_data_yaml.def("bug_2732", &test_data::bug_2732);
    m_data_yaml.def("L_config", &test_data::L_config);
    m_data_yaml.def("GM_cube", &test_data::GM_cube);
    m_data_yaml.def("bug_2838", &test_data::bug_2838);
    m_data_yaml.def("bug_2845", &test_data::bug_2845);
    m_data_yaml.def("maneuvering_with_commands", &test_data::maneuvering_with_commands);
    m_data_yaml.def("kt_kq", &test_data::kt_kq);
    m_data_yaml.def("linear_hydrostatics", &test_data::linear_hydrostatics);
    m_data_yaml.def("test_ship_linear_hydrostatics_without_waves", &test_data::test_ship_linear_hydrostatics_without_waves);
    m_data_yaml.def("test_ship_linear_hydrostatics_with_waves", &test_data::test_ship_linear_hydrostatics_with_waves);
    m_data_yaml.def("fmi", &test_data::fmi);
    m_data_yaml.def("dummy_history", &test_data::dummy_history);
    m_data_yaml.def("complete_yaml_message_from_gui", &test_data::complete_yaml_message_from_gui);
    m_data_yaml.def("JSON_message_with_requested_output", &test_data::JSON_message_with_requested_output);
    m_data_yaml.def("JSON_server_request_GM_cube_with_output", &test_data::JSON_server_request_GM_cube_with_output);
    m_data_yaml.def("complete_yaml_message_for_falling_ball", &test_data::complete_yaml_message_for_falling_ball);
    m_data_yaml.def("bug_2963_hs_fast", &test_data::bug_2963_hs_fast);
    m_data_yaml.def("bug_2963_hs_exact", &test_data::bug_2963_hs_exact);
    m_data_yaml.def("bug_2963_fk", &test_data::bug_2963_fk);
    m_data_yaml.def("bug_2963_diff", &test_data::bug_2963_diff);
    m_data_yaml.def("bug_2963_gm", &test_data::bug_2963_gm);
    m_data_yaml.def("bug_3004", &test_data::bug_3004);
    m_data_yaml.def("bug_3003", &test_data::bug_3003);
    m_data_yaml.def("bug_2984", &test_data::bug_2984);
    m_data_yaml.def("bug_3217", &test_data::bug_3217);
    m_data_yaml.def("bug_3227", &test_data::bug_3227);
    m_data_yaml.def("yml_bug_3230", &test_data::yml_bug_3230);
    m_data_yaml.def("bug_3235", &test_data::bug_3235);
    m_data_yaml.def("bug_3207_yml", &test_data::bug_3207_yml);
    m_data_yaml.def("bug_3241", &test_data::bug_3241);
    m_data_yaml.def("constant_force", &test_data::constant_force);
    m_data_yaml.def("issue_20", &test_data::issue_20);
    m_data_yaml.def("simserver_test_with_commands_and_delay", &test_data::simserver_test_with_commands_and_delay);
    m_data_yaml.def("simserver_message_without_Dt", &test_data::simserver_message_without_Dt);
    m_data_yaml.def("man_with_delay", &test_data::man_with_delay);
    m_data_yaml.def("invalid_json_for_cs", &test_data::invalid_json_for_cs);
    m_data_yaml.def("tutorial_09_gRPC_wave_model", &test_data::tutorial_09_gRPC_wave_model);
    m_data_yaml.def("tutorial_10_gRPC_force_model", &test_data::tutorial_10_gRPC_force_model);
    m_data_yaml.def("tutorial_10_gRPC_force_model_commands", &test_data::tutorial_10_gRPC_force_model_commands);
    m_data_yaml.def("grpc_setpoints", &test_data::grpc_setpoints);
    m_data_yaml.def("gRPC_force_model", &test_data::gRPC_force_model);
    m_data_yaml.def("gRPC_controller", &test_data::gRPC_controller);
    m_data_yaml.def("bug_3187", &test_data::bug_3187);
    m_data_yaml.def("bug_3185_with_invalid_frame", &test_data::bug_3185_with_invalid_frame);
    m_data_yaml.def("bug_3185", &test_data::bug_3185);
    m_data_yaml.def("tutorial_11_gRPC_controller", &test_data::tutorial_11_gRPC_controller);
    m_data_yaml.def("added_mass_from_precal_file", &test_data::added_mass_from_precal_file);
    m_data_yaml.def("tutorial_13_hdb_force_model", &test_data::tutorial_13_hdb_force_model);
    m_data_yaml.def("tutorial_13_precal_r_force_model", &test_data::tutorial_13_precal_r_force_model);
    m_data_yaml.def("tutorial_14_filtered_states", &test_data::tutorial_14_filtered_states);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

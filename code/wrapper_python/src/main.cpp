#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <Eigen/Dense>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "demo.hpp"
#include "core/inc/BodyStates.hpp"
#include "core/inc/EnvironmentAndFrames.hpp"
#include "core/inc/ForceModel.hpp"
#include "core/inc/Wrench.hpp"
#include "force_models/inc/HydrostaticForceModel.hpp"
#include "force_models/inc/GravityForceModel.hpp"
#include "ssc/ssc/kinematics/coriolis_and_centripetal.hpp"
#include "ssc/ssc/kinematics/EulerAngles.hpp"
#include "ssc/ssc/kinematics/KinematicTree.hpp"
#include "ssc/ssc/kinematics/Point.hpp"
#include "ssc/ssc/kinematics/RotationMatrix.hpp"
#include "ssc/ssc/kinematics/Transform.hpp"
#include "ssc/ssc/kinematics/Velocity.hpp"
#include "ssc/ssc/kinematics/Wrench.hpp"

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
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: xdyn

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    py::module m_ssc = m.def_submodule("ssc");
    py::module m_ssc_kinematics = m_ssc.def_submodule("kinematics");

    py::class_<ssc::kinematics::Point>(m_ssc_kinematics, "Point")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def(py::init<const ssc::kinematics::Point&>())
        .def(py::init<const std::string&, const Eigen::Vector3d&>())
        .def(py::init<const std::string&, const double, const double, const double>())
        .def("get_frame",&ssc::kinematics::Point::get_frame)
        .def_readwrite("v", &ssc::kinematics::Point::v, py::return_value_policy::reference_internal);

    py::class_<ssc::kinematics::Wrench>(m_ssc_kinematics, "Wrench")
        .def(py::init<>())
        .def("get_frame",&ssc::kinematics::Wrench::get_frame)
        .def_readwrite("force", &ssc::kinematics::Wrench::force, py::return_value_policy::reference_internal)
        .def_readwrite("torque", &ssc::kinematics::Wrench::torque, py::return_value_policy::reference_internal);

    py::class_<ssc::kinematics::EulerAngles>(m_ssc_kinematics, "EulerAngles")
        .def(py::init<>())
        .def(py::init<const double, const double, const double>())
        .def_readwrite("phi", &ssc::kinematics::EulerAngles::phi)
        .def_readwrite("theta", &ssc::kinematics::EulerAngles::theta)
        .def_readwrite("psi", &ssc::kinematics::EulerAngles::psi)
        .def("__repr__",
           [](const ssc::kinematics::EulerAngles &a) {
               std::stringstream ss;
               ss << "{\"phi\":" << a.phi
                  << ",\"theta\":" << a.theta
                  << ",\"psi\":" << a.psi << "}";
                return ss.str();
            }
        );

    py::class_<ssc::kinematics::KinematicTree>(m_ssc_kinematics, "KinematicTree")
        .def(py::init<>())
        .def("add",&ssc::kinematics::KinematicTree::add)
        .def("get_path",&ssc::kinematics::KinematicTree::get_path);


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
        .def("get_rot",&ssc::kinematics::Velocity::get_point)
        .def("get_rot",&ssc::kinematics::Velocity::get_angular_velocity)
        .def("get_rot",&ssc::kinematics::Velocity::get_translation_velocity)
        .def("get_rot",&ssc::kinematics::Velocity::change_point)
        ;

    m_ssc_kinematics.def("coriolis_and_centripetal", &ssc::kinematics::coriolis_and_centripetal,
         R"(
        These are the forces due to the fact that the equation of Newton\'s
        second law of motion is written in a non-Gallilean frame: hence its speed
        must be taken into account. The inertia matrix provided is the
        rigid body inertia. It should, in general, not be the added mass matrix or
        the sum of the added mass matrix & the rigid-body inertia matrix because
        the added mass forces are experimentally found not to be proportional to
        the acceleration but they are proportional to the derivative of the projection
        in the body frame of the linear velocity of the body with respect to the
        (Gallilean) earth frame.)");

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

    py::class_<EnvironmentAndFrames>(m, "EnvironmentAndFrames")
        .def(py::init<>())
        .def_readwrite("rho", &EnvironmentAndFrames::rho)
        .def_readwrite("nu", &EnvironmentAndFrames::nu)
        .def_readwrite("g", &EnvironmentAndFrames::g)
        .def("set_rho_air", &EnvironmentAndFrames::set_rho_air)
        .def("get_rho_air", &EnvironmentAndFrames::get_rho_air);

    py::class_<BodyStates>(m, "BodyStates")
        .def(py::init<const double>())
        .def_readwrite("name", &BodyStates::name)
        .def_readwrite("x_relative_to_mesh", &BodyStates::x_relative_to_mesh)
        .def_readwrite("y_relative_to_mesh", &BodyStates::y_relative_to_mesh)
        .def_readwrite("z_relative_to_mesh", &BodyStates::z_relative_to_mesh);

    py::class_<Wrench>(m, "Wrench")
        .def(py::init<const ssc::kinematics::Wrench&>())
        .def_readwrite("force", &Wrench::force)
        .def_readwrite("torque", &Wrench::torque);

    py::class_<ForceModel>(m, "ForceModel");
    py::class_<GravityForceModel, ForceModel>(m, "GravityForceModel")
        .def(py::init<const std::string&, const EnvironmentAndFrames&>())
        .def("model_name", &GravityForceModel::model_name)
        .def("potential_energy", &GravityForceModel::potential_energy)
        .def("get_force", &GravityForceModel::get_force);
        // Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;


    // GravityForceModel(const std::string& body_name, const EnvironmentAndFrames& env);

    /*
    py::class_<ForceModel, PyForceModel>(m, "ForceModel")
        .def(py::init<>());
    */
    /*
    py::class_<HydrostaticForceModel, PyForceModel>(m, "HydrostaticForceModel")
        //.def(py::init<>())
        .def("model_name", &HydrostaticForceModel::model_name);
    */


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

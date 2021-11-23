#include "py_ssc.hpp"
#include "py_pybind_additions.hpp"
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
#include <sstream>

namespace py = pybind11;

void py_add_module_ssc(py::module& m)
{
    py::module m_ssc = m.def_submodule("ssc");
    py::module m_ssc_random = m_ssc.def_submodule("random");
    py::module m_ssc_datasource = m_ssc.def_submodule("datasource");

    py::class_<ssc::data_source::DataSource>(m_ssc_datasource, "DataSource")
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
        .def_readwrite("v", &ssc::kinematics::Point::v, py::return_value_policy::reference_internal)
        .def("__repr__",
           [](const ssc::kinematics::Point &a) {
               std::stringstream ss;
               ss << "{\"frame\":" << a.get_frame()
                  << ",\"x\":" << a.x()
                  << ",\"y\":" << a.y()
                  << ",\"z\":" << a.z()
                  << "}";
                return ss.str();
            })
        ;
        ;

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
        .def("__repr__",
           [](const ssc::kinematics::Wrench &a) {
               std::stringstream ss;
               ss << "{\"X\":" << a.X()
                  << ",\"Y\":" << a.Y()
                  << ",\"Z\":" << a.Z()
                  << ",\"K\":" << a.K()
                  << ",\"M\":" << a.M()
                  << ",\"N\":" << a.N()
                  << "}";
                return ss.str();
            })
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
}

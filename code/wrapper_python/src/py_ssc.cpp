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
#include "ssc/ssc/random_data_generator/DataGenerator.hpp"
#include "ssc/ssc/integrate/Burcher.hpp"
#include "ssc/ssc/integrate/ClenshawCurtisCosine.hpp"
#include "ssc/ssc/integrate/ClenshawCurtis.hpp"
#include "ssc/ssc/integrate/ClenshawCurtisSine.hpp"
#include "ssc/ssc/integrate/Cumulate.hpp"
#include "ssc/ssc/integrate/Filon.hpp"
#include "ssc/ssc/integrate/GaussKronrod.hpp"
#include "ssc/ssc/integrate/IntegratorException.hpp"
#include "ssc/ssc/integrate/Integrator.hpp"
#include "ssc/ssc/integrate/QuadPack.hpp"
#include "ssc/ssc/integrate/Rectangle.hpp"
#include "ssc/ssc/integrate/Simpson.hpp"
#include "ssc/ssc/integrate/TrapezoidalIntegration.hpp"
#include <sstream>

namespace py = pybind11;
namespace rdg = ssc::random_data_generator;

template<typename T>
void declare_typed_scalar_data_generator(py::module &m_ssc_random, const std::string &typestr);
template<typename T>
void declare_typed_scalar_data_generator(py::module &m_ssc_random, const std::string &typestr) {
    const std::string pyclass_name = std::string("TypedScalarDataGenerator") + typestr;
    py::class_<rdg::TypedScalarDataGenerator<T>,
               rdg::DataGenerator>(m_ssc_random, pyclass_name.c_str())
        .def(py::init<const rdg::DataGenerator& /*rhs*/>())
        .def("__call__", &rdg::TypedScalarDataGenerator<T>::operator())
        .def("greater_than", &rdg::TypedScalarDataGenerator<T>::greater_than)
        .def("between", &rdg::TypedScalarDataGenerator<T>::between)
        .def("no", &rdg::TypedScalarDataGenerator<T>::no)
        .def("but", &rdg::TypedScalarDataGenerator<T>::but)
        .def("outside", &rdg::TypedScalarDataGenerator<T>::outside)
        .def("but_not",
            static_cast<rdg::TypedScalarDataGenerator<T>&
                (rdg::TypedScalarDataGenerator<T>::*)()>(&rdg::TypedScalarDataGenerator<T>::but_not))
        .def("but_not",
            static_cast<rdg::TypedScalarDataGenerator<T>&
                (rdg::TypedScalarDataGenerator<T>::*)(const T& t)>(&rdg::TypedScalarDataGenerator<T>::but_not))
        ;
}

template<typename T>
void declare_typed_vector_data_generator(py::module &m_ssc_random, const std::string &typestr);
template<typename T>
void declare_typed_vector_data_generator(py::module &m_ssc_random, const std::string &typestr) {
    const std::string pyclass_name = std::string("TypedVectorDataGenerator") + typestr;
    py::class_<rdg::TypedVectorDataGenerator<T>,
               rdg::DataGenerator>(m_ssc_random, pyclass_name.c_str())
        .def(py::init<const rdg::DataGenerator& /*rhs*/>())
        .def("of_size", &rdg::TypedVectorDataGenerator<T>::of_size)
        .def("__call__", &rdg::TypedVectorDataGenerator<T>::get_impl)
        .def("greater_than", &rdg::TypedVectorDataGenerator<T>::greater_than)
        .def("but", &rdg::TypedVectorDataGenerator<T>::but)
        .def("between", &rdg::TypedVectorDataGenerator<T>::between)
        .def("no", &rdg::TypedVectorDataGenerator<T>::no)
        ;
}

void py_add_module_ssc_random(py::module& m_ssc);
void py_add_module_ssc_random(py::module& m_ssc)
{
    py::module m_ssc_random = m_ssc.def_submodule("random");
    py::class_<rdg::DataGenerator>(m_ssc_random, "DataGenerator")
        .def(py::init<const size_t& /*seed*/>(), py::arg("seed")=666)
        .def("random_double",
            static_cast<rdg::TypedScalarDataGenerator<double>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random<double>))
        .def("random_int",
            static_cast<rdg::TypedScalarDataGenerator<int>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random<int>))
        .def("random_size_t",
            static_cast<rdg::TypedScalarDataGenerator<size_t>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random<size_t>))
        .def("random_string",
            static_cast<rdg::TypedScalarDataGenerator<std::string>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random<std::string>))
        .def("random_vector_of_double",
            static_cast<rdg::TypedVectorDataGenerator<double>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random_vector_of<double>))
        .def("random_vector_of_int",
            static_cast<rdg::TypedVectorDataGenerator<int>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random_vector_of<int>))
        .def("random_vector_of_size_t",
            static_cast<rdg::TypedVectorDataGenerator<size_t>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random_vector_of<size_t>))
        .def("random_vector_of_string",
            static_cast<rdg::TypedVectorDataGenerator<std::string>
                (rdg::DataGenerator::*)() const>(&rdg::DataGenerator::random_vector_of<std::string>))
        ;

    declare_typed_scalar_data_generator<double>(m_ssc_random, "Double");
    declare_typed_scalar_data_generator<int>(m_ssc_random, "Int");
    declare_typed_scalar_data_generator<size_t>(m_ssc_random, "SizeT");
    declare_typed_scalar_data_generator<std::string>(m_ssc_random, "String");

    declare_typed_vector_data_generator<double>(m_ssc_random, "Double");
    declare_typed_vector_data_generator<int>(m_ssc_random, "Int");
    declare_typed_vector_data_generator<size_t>(m_ssc_random, "SizeT");
    declare_typed_vector_data_generator<std::string>(m_ssc_random, "String");
}

void py_add_module_ssc_datasource(py::module& m_ssc);
void py_add_module_ssc_datasource(py::module& m_ssc)
{
    py::module m_ssc_datasource = m_ssc.def_submodule("datasource");
    py::class_<ssc::data_source::DataSource>(m_ssc_datasource, "DataSource")
        .def(py::init<>())
        ;
}

void py_add_module_ssc_kinematics(py::module& m_ssc);
void py_add_module_ssc_kinematics(py::module& m_ssc)
{
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
        .def("get_path",&ssc::kinematics::KinematicTree::get_path)
        ;

    py::class_<ssc::kinematics::PointMatrix>(m_ssc_kinematics, "PointMatrix")
        .def(py::init<>())
        .def(py::init<const std::string& /*frame*/, const size_t /*nb_of_columns*/>())
        .def(py::init<const ssc::kinematics::Matrix3Xd& /*m*/,const std::string& /*frame*/>())
        .def_readwrite("m",&ssc::kinematics::PointMatrix::m)
        .def("get_frame", &ssc::kinematics::PointMatrix::get_frame)
        ;

    py::class_<ssc::kinematics::PointMatrixPtr>(m_ssc_kinematics, "PointMatrixPtr");

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
        py::arg("P"), py::arg("M"), py::arg("v"), py::arg("omega"),
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

void py_add_module_ssc(py::module& m)
{
    py::module m_ssc = m.def_submodule("ssc");
    py_add_module_ssc_datasource(m_ssc);
    py_add_module_ssc_kinematics(m_ssc);
    py_add_module_ssc_random(m_ssc);
}
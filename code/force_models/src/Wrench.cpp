#include "Wrench.hpp"

Wrench::Wrench(const ssc::kinematics::Point& P, const std::string& frame_):
        force(Eigen::Vector3d(0, 0, 0)),
        torque(Eigen::Vector3d(0, 0, 0)),
        point(P),
        frame(frame_)
{
}

Wrench::Wrench(const ssc::kinematics::Point& P, const std::string& frame_, const ssc::kinematics::Vector6d& v):
        force(Eigen::Vector3d(v(0),v(1),v(2))),
        torque(Eigen::Vector3d(v(3),v(4),v(5))),
        point(P),
        frame(frame_)
{
}

Wrench::Wrench(const ssc::kinematics::Point& P, const std::string& frame_, const Eigen::Vector3d& force_, const Eigen::Vector3d& torque_):
        force(force_),
        torque(torque_),
        point(P),
        frame(frame_)
{
}

ssc::kinematics::Point Wrench::get_point() const
{
    return point;
}

std::string Wrench::get_frame() const
{
    return frame;
}

Eigen::Vector3d Wrench::get_force() const
{
    return force;
}

Eigen::Vector3d Wrench::get_torque() const
{
    return torque;
}

ssc::kinematics::Vector6d Wrench::to_vector() const
{
    double v[6] = {X(),Y(),Z(),K(),M(),N()};
    return Eigen::Map<ssc::kinematics::Vector6d>(v);
}

std::ostream& operator<<(std::ostream& os, const Wrench& w)
{
    os << "frame: " << w.get_frame() << ", point: " << w.get_point() << ", force: [" << w.get_force().transpose() << "], torque: [" << w.get_torque().transpose() << "]" ;
    return os;
}


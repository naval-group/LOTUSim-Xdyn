#include "Wrench.hpp"
#include <iostream>

#include "InternalErrorException.hpp"

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

void Wrench::change_frame(const std::string new_frame, const ssc::kinematics::RotationMatrix& R)
{
    force = R * force;
    torque = R * torque;
    frame = new_frame;
}

void Wrench::change_frame(const std::string new_frame, const ssc::kinematics::KinematicsPtr k)
{
    if(new_frame!=frame)
    {
        const ssc::kinematics::RotationMatrix R = k->get(new_frame,frame).get_rot();
        change_frame(new_frame, R);
    }

}

bool operator!=(const ssc::kinematics::Point& A, const ssc::kinematics::Point& B);
bool operator!=(const ssc::kinematics::Point& A, const ssc::kinematics::Point& B)
{
    if(A.get_frame()==B.get_frame() && A.v.isApprox(B.v))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Wrench::transport_to(const ssc::kinematics::Point& P, const ssc::kinematics::KinematicsPtr k)
{
    if(P!=point)
    {
        const auto BA = get_BA(P, k);
        torque = torque + BA.cross(force);
        point = P;
    }
}

void Wrench::change_point_and_frame(const ssc::kinematics::Point& P, const std::string new_frame, const ssc::kinematics::KinematicsPtr k)
{
    transport_to(P, k);
    change_frame(new_frame, k);
}

Wrench Wrench::change_frame(const std::string new_frame, const ssc::kinematics::RotationMatrix& R) const
{
    Wrench ret(*this);
    ret.change_frame(new_frame, R);
    return ret;
}

Wrench Wrench::change_frame(const std::string new_frame, const ssc::kinematics::KinematicsPtr k) const
{
    Wrench ret(*this);
    ret.change_frame(new_frame, k);
    return ret;
}

Wrench Wrench::transport_to(const ssc::kinematics::Point& P, const ssc::kinematics::KinematicsPtr k) const
{
    Wrench ret(*this);
    ret.transport_to(P, k);
    return ret;
}

Wrench Wrench::change_point_and_frame(const ssc::kinematics::Point& P, const std::string new_frame, const ssc::kinematics::KinematicsPtr k) const
{
    Wrench ret(*this);
    ret.change_point_and_frame(P, new_frame, k);
    return ret;
}

void Wrench::add(const Wrench& other, const ssc::kinematics::KinematicsPtr k)
{
    Wrench other_wrench = other.change_point_and_frame(point, frame, k);
    force += other_wrench.force;
    torque += other_wrench.torque;
}

Wrench Wrench::add(const Wrench& other, const ssc::kinematics::KinematicsPtr k) const
{
    Wrench ret(*this);
    ret.add(other, k);
    return ret;
}

ssc::kinematics::Point operator*(const ssc::kinematics::Point& P, const ssc::kinematics::Transform T);
ssc::kinematics::Point operator*(const ssc::kinematics::Point& P, const ssc::kinematics::Transform T)
{
    if (P.get_frame() != T.get_from_frame())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, std::string("Frames don't match: transform goes from ") + T.get_from_frame() + " to " + T.get_to_frame() + ", but point lies in " + P.get_frame());
    }
    return ssc::kinematics::Point(T.get_to_frame(), T.inverse().get_point().v + T.get_rot().inverse()*P.v);
}

Eigen::Vector3d Wrench::get_BA(const ssc::kinematics::Point& B, const ssc::kinematics::KinematicsPtr k) const
{
    Eigen::Vector3d B_coord;
    if(B.get_frame() == frame)
    {
        B_coord = B.v;
    }
    else
    {
        auto B_in_force_frame = B*k->get(B.get_frame(),frame);
        B_coord = B_in_force_frame.v;
    }
    Eigen::Vector3d A_coord;
    if(point.get_frame() == frame)
    {
        A_coord = point.v;
    }
    else
    {
        auto A_in_force_frame = point*k->get(point.get_frame(),frame);
        A_coord = A_in_force_frame.v;
    }
    return A_coord - B_coord;
}

Wrench operator+(const Wrench& lhs, const Wrench& rhs)
{
    if (lhs.get_point() != rhs.get_point())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Points don't match: LHS is expressed at point " << lhs.get_point() << " and RHS at point " << rhs.get_point());
    }
    if (lhs.get_frame() != rhs.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Frames don't match: LHS is expressed in frame " << lhs.get_frame() << " and RHS in frame " << rhs.get_frame());
    }
    return Wrench(lhs.get_point(), lhs.get_frame(), lhs.force + rhs.force, lhs.torque + rhs.torque);
}

std::ostream& operator<<(std::ostream& os, const Wrench& w)
{
    os << "frame: " << w.get_frame() << ", point: " << w.get_point() << ", force: [" << w.get_force().transpose() << "], torque: [" << w.get_torque().transpose() << "]" ;
    return os;
}


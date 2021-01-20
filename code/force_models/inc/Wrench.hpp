#ifndef FORCE_MODELS_INC_WRENCH_HPP_
#define FORCE_MODELS_INC_WRENCH_HPP_

#include <string>
#include <ssc/kinematics.hpp>
#include <Eigen/Dense>

class Wrench
{
public:
    Wrench(const ssc::kinematics::Point& P, const std::string& frame);
    Wrench(const ssc::kinematics::Point& P, const std::string& frame, const ssc::kinematics::Vector6d& v);
    Wrench(const ssc::kinematics::Point& P, const std::string& frame, const Eigen::Vector3d& force, const Eigen::Vector3d& torque);

    ssc::kinematics::Point get_point() const;
    std::string get_frame() const;
    Eigen::Vector3d get_force() const;
    Eigen::Vector3d get_torque() const;

    inline double X() const {return force[0];}
    inline double Y() const {return force[1];}
    inline double Z() const {return force[2];}
    inline double K() const {return torque[0];}
    inline double M() const {return torque[1];}
    inline double N() const {return torque[2];}

    ssc::kinematics::Vector6d to_vector() const;

private:
    Wrench(); // Deactivated
    Eigen::Vector3d force;
    Eigen::Vector3d torque;
    const ssc::kinematics::Point point;
    const std::string frame;
};

std::ostream& operator<<(std::ostream& os, const Wrench& w);

#endif /* FORCE_MODELS_INC_WRENCH_HPP_ */

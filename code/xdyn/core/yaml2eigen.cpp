/*
 * yaml2eigen.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "yaml2eigen.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "xdyn/external_data_structures/YamlAngle.hpp"
#include "xdyn/external_data_structures/YamlCoordinates.hpp"
#include "xdyn/external_data_structures/YamlDynamics6x6Matrix.hpp"
#include "xdyn/external_data_structures/YamlPoint.hpp"
#include "xdyn/external_data_structures/YamlPosition.hpp"
#include "xdyn/external_data_structures/YamlRotation.hpp"

#include <ssc/exception_handling.hpp>

ssc::kinematics::RotationMatrix angle2matrix(const YamlAngle& a, const YamlRotation& rotations)
{
    const ssc::kinematics::EulerAngles e(a.phi, a.theta, a.psi);

    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
            return ssc::kinematics::rotation_matrix<ssc::kinematics::INTRINSIC,
                                                    ssc::kinematics::CHANGING_ANGLE_ORDER,
                                                    3, 2, 1>(e);
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In YAML file, section 'rotations convention', convention [" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "] is not currently supported.");
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In YAML file, section 'rotations convention', ordering rotations by '" << rotations.order_by << "' is not currently supported");
    }
    return ssc::kinematics::RotationMatrix();
}

bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third)
{
    return (convention.at(0) == first) and (convention.at(1) == second) and (convention.at(2) == third);
}

ssc::kinematics::Point make_point(const YamlCoordinates& point, const std::string& frame)
{
    return ssc::kinematics::Point(frame,
                                  point.x,
                                  point.y,
                                  point.z);
}

ssc::kinematics::Point make_point(const YamlPoint& point)
{
    return make_point(point, point.frame);
}

ssc::kinematics::Transform make_transform(const YamlPosition& p, const std::string& to_frame, const YamlRotation& rotations)
{
    const ssc::kinematics::Point translation = make_point(p.coordinates, p.frame);
    const ssc::kinematics::RotationMatrix rotation = angle2matrix(p.angle, rotations);
    return ssc::kinematics::Transform(translation, rotation, to_frame);
}


void convert_matrix_from_aquaplus_to_xdyn_frame(Eigen::Matrix<double,6,6>& matrix)
{
    matrix(0, 1) = -matrix(0, 1);
    matrix(0, 2) = -matrix(0, 2);
    matrix(0, 4) = -matrix(0, 4);
    matrix(0, 5) = -matrix(0, 5);
    matrix(1, 0) = -matrix(1, 0);
    matrix(1, 3) = -matrix(1, 3);
    matrix(2, 0) = -matrix(2, 0);
    matrix(2, 3) = -matrix(2, 3);
    matrix(3, 1) = -matrix(3, 1);
    matrix(3, 2) = -matrix(3, 2);
    matrix(3, 4) = -matrix(3, 4);
    matrix(3, 5) = -matrix(3, 5);
    matrix(4, 0) = -matrix(4, 0);
    matrix(4, 3) = -matrix(4, 3);
    matrix(5, 0) = -matrix(5, 0);
    matrix(5, 3) = -matrix(5, 3);
}

Eigen::Matrix<double,6,6> make_matrix6x6(const YamlDynamics6x6Matrix& M)
{
    if (M.read_from_file)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "One expects a description with 6 rows");
    }
    Eigen::Matrix<double,6,6> ret;
    for (size_t j = 0 ; j < 6 ; ++j)
    {
        ret(0, static_cast<Eigen::Index>(j)) = M.row_1.at(j);
        ret(1, static_cast<Eigen::Index>(j)) = M.row_2.at(j);
        ret(2, static_cast<Eigen::Index>(j)) = M.row_3.at(j);
        ret(3, static_cast<Eigen::Index>(j)) = M.row_4.at(j);
        ret(4, static_cast<Eigen::Index>(j)) = M.row_5.at(j);
        ret(5, static_cast<Eigen::Index>(j)) = M.row_6.at(j);
    }
    if (not(M.row_convention_xdyn_with_z_down))
    {
        convert_matrix_from_aquaplus_to_xdyn_frame(ret);
    }
    return ret;
}

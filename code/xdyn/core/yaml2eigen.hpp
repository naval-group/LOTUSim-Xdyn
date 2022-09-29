/*
 * yaml2eigen.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef YAML2EIGEN_HPP_
#define YAML2EIGEN_HPP_

#include <ssc/kinematics.hpp>
#include <Eigen/Dense>
#include <string>
#include <vector>

struct YamlAngle;
struct YamlCoordinates;
struct YamlDynamics6x6Matrix;
struct YamlPoint;
struct YamlPosition;
struct YamlRotation;

/** \details Converts (phi,theta,psi) to a rotation matrix, taking the
 *           rotation convention into account
 */
ssc::kinematics::RotationMatrix angle2matrix(const YamlAngle& a, const YamlRotation& rotations);

/** \brief Utility function used by angle2matrix
 *  \returns True if 'convention' == [first,second,third]
 */
bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third);

ssc::kinematics::Point make_point(const YamlPoint& point);
ssc::kinematics::Point make_point(const YamlCoordinates& point, const std::string& frame);
ssc::kinematics::Transform make_transform(const YamlPosition& position_of_frame_relative_to_another, const std::string& to_frame, const YamlRotation& rotations);


void convert_matrix_from_aquaplus_to_xdyn_frame(Eigen::Matrix<double,6,6>& matrix);
Eigen::Matrix<double,6,6> make_matrix6x6(const YamlDynamics6x6Matrix& M);

#endif /* YAML2EIGEN_HPP_ */

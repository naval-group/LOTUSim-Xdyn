/*
 * Seabed.hpp
 *
 *  Created on: 20 June 2025
 *      Author: julien.prodhon
 * 
 * Naval Group Far East - CoE
 * Seabed height map implementation with bilinear interpolation
 */

#ifndef SEABED_HPP_
#define SEABED_HPP_

#include <Eigen/Dense>
#include <map>

struct InterpolationWeights {
    double left, right, down, up;
    int xleft, xright, ydown, yup;
};

class Seabed
{
public:
    Seabed(const std::string sea_bed_file);
    Seabed(const double depth);
    double get_height(const double, const double) const;
    Eigen::Vector2d get_gradient(const double, const double) const;
private:
    std::map< std::pair<int, int>, double> seabed;
    int width;
    int height;
    double xscale;
    double yscale;
    double zscale;
    double xoffset;
    double yoffset;
    InterpolationWeights calculateWeights(const double, const double) const;
};




#endif /* SEABED_HPP_ */

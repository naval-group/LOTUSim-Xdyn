/*
 * Seabed.hpp
 *
 *  Created on: Dec 20, 2016
 *      Author: cady
 */

#ifndef SEABED_HPP_
#define SEABED_HPP_

#include <Eigen/Dense>
#include <map>

class Seabed
{
public:
    Seabed(std::string sea_bed_file);
    Seabed(double depth);
    std::tuple<double, double, double, double, int, int, int, int> get_pos(double, double) const;
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
};


#endif /* SEABED_HPP_ */

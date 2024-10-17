/*
 * UWCurrentModelInterface.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "UWCurrentModel.hpp"

#include <map>

#include "ssc/stb.hpp"

UWCurrentModel::UWCurrentModel()
{
}

UWCurrentModel::~UWCurrentModel()
{
}


UWCurrentModel::Seabed::Seabed(std::string sea_bed_file) : seabed_file(sea_bed_file), seabed(), xmin(), xmax(), ymin(), ymax(), xscale(), yscale(), zscale(), xoffset(), yoffset()
{
    std::map< std::pair<int, int>, double> seabed;
    int width, height, channels;
    const char *load_file = sea_bed_file.c_str();
    const uint8_t* img = stbi_load(load_file, &width, &height, &channels, 0);
    for (int i=0; i < xmax*ymax; i++)
    {
        int x = (int) std::floor(i/xmax);
        int y = (int) i%ymax;
        seabed[std::make_pair(x,y)] = img[i];
    }
}

double UWCurrentModel::Seabed::get_height(const double x, const double y) const
{
    double xx = x / xscale - xoffset; double yy = y / yscale - yoffset;
    if (xx < xmin) {xx = xmin;} if (xx > xmax) {xx = xmax;}
    if (yy < ymin) {yy = ymin;} if (yy > ymax) {yy = ymax;}
    int xp = (int) std::ceil(xx); int xm = (int) std::floor(xx);
    int yp = (int) std::ceil(yy); int ym = (int) std::floor(yy);
    double x2 = xp - xx; double x1 = xx - xm;
    double y2 = yp - yy; double y1 = yy - ym;
    double h11 = seabed.at(std::make_pair(xm,ym));
    double h12 = seabed.at(std::make_pair(xm,yp));
    double h21 = seabed.at(std::make_pair(xp,ym));
    double h22 = seabed.at(std::make_pair(xp,yp));
    return zscale * (x2*y2*h11 + x2*y1*h12 + x1*y2*h21 + x1*y1*h22);
}

Eigen::Vector2d UWCurrentModel::Seabed::get_gradient(const double x, const double y) const
{
    double xx = x / xscale - xoffset; double yy = y / yscale - yoffset;
    if (xx < xmin || xx > xmax || yy < ymin || yy > ymax) 
    {
        return Eigen::Vector2d {0, 0};
    }
    else
    {
        int xp = (int) std::ceil(xx); int xm = (int) std::floor(xx);
        int yp = (int) std::ceil(yy); int ym = (int) std::floor(yy);
        double x2 = xp - xx; double x1 = xx - xm;
        double y2 = yp - yy; double y1 = yy - ym;
        double h11 = seabed.at(std::make_pair(xm,ym));
        double h12 = seabed.at(std::make_pair(xm,yp));
        double h21 = seabed.at(std::make_pair(xp,ym));
        double h22 = seabed.at(std::make_pair(xp,yp));
        double gx1 = h21 - h11; double gx2 = h22 - h21;
        double gy1 = h12 - h11; double gy2 = h22 - h12;
        return zscale * Eigen::Vector2d {y1*gx1 + y2*gx2, x1*gy1 + x2*gy2};
    }
}

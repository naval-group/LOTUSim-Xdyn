/*
 * Seabed.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include <iostream>
#include <boost/filesystem.hpp>
#include "Seabed.hpp"
#include "ssc/stb.hpp"

Seabed::Seabed(std::string sea_bed_file)
    : seabed()
    , width(1)
    , height(1)
    , xscale(1)
    , yscale(1)
    , zscale(1)
    , xoffset(0)
    , yoffset(0)
{    
    const char* seabed_path = sea_bed_file.c_str();
    const char* lotus_model_path = std::getenv("LOTUSIM_MODELS_PATH"); // env defined when installing lotusim
    boost::filesystem::path seabed_file_path(seabed_path);
    if (!seabed_file_path.is_absolute()) // Check if file exists at the relative path
    {
        if (boost::filesystem::exists(seabed_path)) {} // File exists at relative path, keep it
        else if (lotus_model_path)  // File doesn't exist, try with LOTUSIM_MODELS_PATH
        {
            boost::filesystem::path lotus_path(lotus_model_path);
            seabed_path = (lotus_path / seabed_file_path).c_str(); // Update the const char* pointer
        }
    }
    int img_width, img_height, channels;
    unsigned char* img = stbi_load(seabed_path, &img_width, &img_height, &channels, 0);
    this->width = img_width;
    this->height = img_height;
    // Error checking
    if (!img) {
        std::cerr << "Failed to load seabed image: " << stbi_failure_reason() << std::endl;
    }
    for (int i = 0; i < width * height; i++)
    {
        int x = i % width;
        int y = i / width;
        seabed[std::make_pair(x, y)] = img[i];
    }
    stbi_image_free(img); // Free memory
}

Seabed::Seabed(double depth)
    : seabed()
    , width(1)
    , height(1)
    , xscale(1)
    , yscale(1)
    , zscale(1)
    , xoffset(0)
    , yoffset(0)
{    
    seabed[std::make_pair(0, 0)] = depth;
}

std::tuple<double, double, double, double, int, int, int, int> Seabed::get_pos(double x, double y) const
{
    double eps = 0.00000001;
    double xx = x / xscale - xoffset;
    double yy = y / yscale - yoffset;
    double left, right, down, up;
    int xright = (int)std::ceil(xx+eps);
    int xleft = (int)std::floor(xx);
    if (xx <= 0) {right = 0; left = 1;}
    else if (xx >= width-1) {right = 1; left = 0;}
    else {right = xright - xx; left = xx - xleft;}
    int yup = (int)std::ceil(yy+eps);
    int ydown = (int)std::floor(yy);
    if (yy <= 0) {up = 0; down = 1;}
    else if (yy >= height-1) {up = 1; down = 0;}
    else {up = yup - yy; down = yy - ydown;}
    if (xleft <= 0) {xleft = 0;}
    else if (xleft >= width-1) {xleft = width-1;}
    if (xright <= 0) {xright = 0;}
    else if (xright >= width-1) {xright = width-1;}
    if (ydown <= 0) {ydown = 0;}
    else if (ydown >= height-1) {ydown = height-1;}
    if (yup <= 0) {yup = 0;}
    else if (yup >= height-1) {yup = height-1;}
    return std::make_tuple(left,right,down,up,xleft,xright,ydown,yup);
}

double Seabed::get_height(const double x, const double y) const
{
    if (width == 1 && height == 1) {return seabed.at(std::make_pair(0, 0));}
    else 
    {
        double left,right,down,up;
        int xleft,xright,ydown,yup;
        std::tie(left,right,down,up,xleft,xright,ydown,yup) = Seabed::get_pos(x,y);
        double h_up_right = seabed.at(std::make_pair(xleft, ydown));
        double h_down_right = seabed.at(std::make_pair(xleft, yup));
        double h_up_left = seabed.at(std::make_pair(xright, ydown));
        double h_down_left = seabed.at(std::make_pair(xright, yup));
        double output = zscale * (right * up * h_up_right + right * down * h_down_right + left * up * h_up_left + left * down * h_down_left);
        return output;
    }
}

Eigen::Vector2d Seabed::get_gradient(const double x, const double y) const
{
    double xx = x / xscale - xoffset;
    double yy = y / yscale - yoffset;
    if (xx < 0 || xx > width || yy < 0 || yy > height)
    {
        return Eigen::Vector2d{ 0, 0 };
    }
    else
    {
        double left,right,down,up;
        int xleft,xright,ydown,yup;
        std::tie(left,right,down,up,xleft,xright,ydown,yup) = Seabed::get_pos(x,y);
        double h_up_right = seabed.at(std::make_pair(xleft, ydown));
        double h_down_right = seabed.at(std::make_pair(xleft, yup));
        double h_up_left = seabed.at(std::make_pair(xright, ydown));
        double h_down_left = seabed.at(std::make_pair(xright, yup));
        double gx1 = h_up_left - h_up_right;
        double gx2 = h_down_left - h_up_left;
        double gy1 = h_down_right - h_up_right;
        double gy2 = h_down_left - h_down_right;
        return zscale * Eigen::Vector2d{ down * gx1 + up * gx2, left * gy1 + right * gy2 };
    }
}

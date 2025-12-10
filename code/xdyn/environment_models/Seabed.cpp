/*
 * Seabed.cpp
 *
 *  Created on: 20 June 2025
 *      Author: julien.prodhon
 * 
 * Naval Group Far East - CoE
 * Seabed height map implementation with bilinear interpolation
 */

#include <iostream>
#include <boost/filesystem.hpp>
#include "Seabed.hpp"
#include "stb.hpp"

// Constructor: Load seabed from image file
Seabed::Seabed(const std::string sea_bed_file)
    : seabed(), width(), height(), xscale(1), yscale(1), zscale(1), xoffset(0), yoffset(0)
{    
    // Resolve file path
    std::string seabed_path;
    boost::filesystem::path file_path(sea_bed_file);
    // If absolute path or file exists locally, use as-is
    if (file_path.is_absolute() || boost::filesystem::exists(sea_bed_file)) {
        seabed_path = sea_bed_file;
    }
    else { // Try with LOTUSIM_MODELS_PATH environment variable
        const char* lotus_path = std::getenv("LOTUSIM_MODELS_PATH");
        if (lotus_path) {
            boost::filesystem::path full_path = boost::filesystem::path(lotus_path) / file_path;
            seabed_path = full_path.string();
        }
        else {
            std::cerr << "Can't find image with this path: " << stbi_failure_reason() << std::endl;
        }
    }
    // Load Image data
    int channels;
    unsigned char* img = stbi_load(seabed_path.c_str(), &width, &height, &channels, 1);
    if (!img) {
        std::cerr << "Failed to load seabed image: " << stbi_failure_reason() << std::endl;
    }
    // Store pixel data in seabed map
    for (int i = 0; i < width * height; i++) {
        int x = i % width;
        int y = i / width;
        seabed[{x, y}] = img[i];
    }
    stbi_image_free(img);
}

// Constructor: Create uniform depth seabed
Seabed::Seabed(const double depth)
    : seabed(), width(1), height(1), xscale(1), yscale(1), zscale(1), xoffset(0), yoffset(0)
{    
    seabed[{0, 0}] = depth;
}

double Seabed::get_height(const double x, const double y) const
{
    // Handle single-point seabed
    if (width == 1 && height == 1) {
        return seabed.at({0, 0});
    }
    const InterpolationWeights weights = calculateWeights(x, y);
    // Get corner heights for bilinear interpolation
    const double h_ul = seabed.at({weights.xleft, weights.yup});    // up-left
    const double h_ur = seabed.at({weights.xright, weights.yup});   // up-right  
    const double h_dl = seabed.at({weights.xleft, weights.ydown});  // down-left
    const double h_dr = seabed.at({weights.xright, weights.ydown}); // down-right
    // Bilinear interpolation
    const double interpolated = weights.left * weights.up * h_ul +
                         weights.right * weights.up * h_ur +
                         weights.left * weights.down * h_dl +
                         weights.right * weights.down * h_dr;
    return zscale * interpolated;
}

Eigen::Vector2d Seabed::get_gradient(const double x, const double y) const
{
    const double x_rel = x / xscale - xoffset;
    const double y_rel = y / yscale - yoffset;
    // Return zero gradient if outside bounds
    if (x_rel < 0 || x_rel > width || y_rel < 0 || y_rel > height) {
        return Eigen::Vector2d::Zero();
    }
    const InterpolationWeights weights = calculateWeights(x, y);
    // Get corner heights
    const double h_dr = seabed.at({weights.xright, weights.ydown});
    const double h_dl = seabed.at({weights.xleft, weights.ydown});
    const double h_ur = seabed.at({weights.xright, weights.yup});
    const double h_ul = seabed.at({weights.xleft, weights.yup});
    // X-gradient: interpolate between left and right differences
    const double gx = weights.down * (h_dr - h_dl) + weights.up * (h_ur - h_ul);
    // Y-gradient: interpolate between up and down differences  
    const double gy = weights.left * (h_ul - h_dl) + weights.right * (h_ur - h_dr);
    return zscale * Eigen::Vector2d{gx, gy};
}

InterpolationWeights Seabed::calculateWeights(const double x_abs, const double y_abs) const
{
    const double x = x_abs / xscale - xoffset;
    const double y = y_abs / yscale - yoffset;
    // Get grid indices
    const int xleft = std::max(0, std::min(width - 1, (int)std::floor(x)));
    const int xright = std::max(0, std::min(width - 1, (int)std::ceil(x)));
    const int ydown = std::max(0, std::min(height - 1, (int)std::floor(y)));
    const int yup = std::max(0, std::min(height - 1, (int)std::ceil(y)));
    // Calculate interpolation weights
    double left, right, down, up;
    if (x <= 0) { 
        left = 1; 
        right = 0; 
    }
    else if (x >= width - 1) { 
        left = 0; 
        right = 1; 
    }
    else { 
        left = x - xleft; 
        right = xright - x; 
    }
    if (y <= 0) { 
        up = 0; 
        down = 1; 
    }
    else if (y >= height - 1) { 
        up = 1; 
        down = 0; 
    }
    else { 
        up = yup - y; 
        down = y - ydown; 
    }
    return {left, right, down, up, xleft, xright, ydown, yup};
}

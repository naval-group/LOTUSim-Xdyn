/*
 * EkmanUWCurrentModel.cpp
 *
 *  Created on: 21 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "EkmanUWCurrentModel.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "yaml.h"
#include <ssc/yaml_parser.hpp>

// CAREFUL NED convention, REVERIFY WITH THE CONVENTION

EkmanUWCurrentModel::EkmanUWCurrentModel(const Input& input) : seabed(input.seabed), 
                                                                rho(input.rho), 
                                                                dTop(input.dTop), 
                                                                dBottom(input.dBottom), 
                                                                velocity(input.velocity), 
                                                                phi(input.phi), 
                                                                omega(input.omega),
                                                                windTau(input.windTau)
{
}

EkmanUWCurrentModel::EkmanUWCurrentModel(int i) : seabed(Seabed("init")),
                                                    rho(1),   
                                                    dTop(0), 
                                                    dBottom(i-1), 
                                                    velocity(0), 
                                                    phi(0), 
                                                    omega(0),
                                                    windTau({0,0})
{
}

EkmanUWCurrentModel::~EkmanUWCurrentModel()
{
}

std::string EkmanUWCurrentModel::model_name() {return "ekman current";}

EkmanUWCurrentModel::Input EkmanUWCurrentModel::parse(const std::string& yaml_input)
{
    Input ret;
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);

    std::string sea_bed_file;
    node["seabed"] >> sea_bed_file;
    ret.seabed = Seabed(sea_bed_file);

    double wind_angle;
    if (node.FindValue("wind angle"))
    {
        ssc::yaml_parser::parse_uv(node["wind angle"], wind_angle);
    }
    else
    {
        wind_angle = 0;
    }

    double U10;
    if (node.FindValue("U10"))
    {
        ssc::yaml_parser::parse_uv(node["U10"], U10);
    }
    else
    {
        U10 = 1.;
    }

    double rho_air;
    if (node.FindValue("rho air"))
    {
        node["rho air"] >> rho_air;
    }
    else
    {
        rho_air = 1.225;
    }

    if (node.FindValue("velocity"))
    {
        ssc::yaml_parser::parse_uv(node["velocity"], ret.velocity);
    }
    else
    {
        ret.velocity = 1.;
    }

    if (node.FindValue("phi"))
    {
        node["phi"] >> ret.phi;
    }
    else
    {
        ret.phi = 48;
    }

    if (node.FindValue("rho"))
    {
        node["rho"] >> ret.rho;
    }
    else
    {
        ret.rho = 1026;
    }

    if (node.FindValue("omega"))
    {
        node["omega"] >> ret.omega;
    }
    else
    {
        ret.omega = 7.2921e-5; // rad/s
    }

    if (node.FindValue("dTop"))
    {
        node["dTop"] >> ret.dTop;
    }
    else
    {
        ret.dTop = 10;
    }

    if (node.FindValue("dBottom"))
    {
        node["dBottom"] >> ret.dBottom;
    }
    else
    {
        ret.dBottom = 100;
    }

    double CD = 0;
    if (U10 < 20.228) {CD = 7.88*10e-4 + 8.08*10e-5 * U10;}
    else {CD = 0.002423;}
    Eigen::Vector2d wind_orientation {cos(wind_angle), sin(wind_angle)};
    Eigen::Vector2d wind = CD * rho_air * U10*U10 * wind_orientation;
    ret.windTau = CD * rho_air * U10 * wind;

    return ret;
}

Eigen::Vector3d EkmanUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
{
    Eigen::Vector3d middle_current = MiddleLayer();
    double z = p(2);
    double height = seabed.get_height(p(0),p(1));
    if (z > wave_height && z < (wave_height - dTop))
    {
        return TopLayer(p, middle_current);
    }
    else if (z > (wave_height - dTop) && z < (height - dBottom)) 
    {
        return middle_current;
    }
    else if (z > (height - dBottom) && z < height)
    {
        return BottomLayer(p, middle_current);
    }
    else 
    {
        return Eigen::Vector3d::Zero();
    }
}

Eigen::Vector3d EkmanUWCurrentModel::TopLayer(const Eigen::Vector3d& p, const Eigen::Vector3d& middle_current) const
{
    double z = -p(2);
    double C1 = (sqrt(2)/(rho * 2 * omega * sin(phi) * dTop)) * exp(z / dTop);
    double C2 = p(2)/dTop - PI/4;
    double u = middle_current(0) + C1 * ( windTau[0] * cos(C2) - windTau[1] * sin(C2) );
    double v = middle_current(1) + C1 * ( windTau[0] * sin(C2) - windTau[1] * cos(C2) );
    double w = 0;
    return Eigen::Vector3d (u, v, -w);
}

Eigen::Vector3d EkmanUWCurrentModel::MiddleLayer() const
{
    return Eigen::Vector3d (velocity*cos(phi), velocity*sin(phi), 0);
}

Eigen::Vector3d EkmanUWCurrentModel::BottomLayer(const Eigen::Vector3d& p, const Eigen::Vector3d& middle_current) const
{
    // double dmx = ( MiddleLayer(ceil(x),y,z) - MiddleLayer(floor(x),y,z) ) / dx;
    // double dmy = ( MiddleLayer(x,ceil(y),z) - MiddleLayer(x,floor(y),z) ) / dy;
    Eigen::Vector2d grad = seabed.get_gradient(p(0),p(1));
    double C = - (seabed.get_height(p(0),p(1)) - p(2)) / dBottom;
    double C1 = (1 - exp(C)*cos(-C));
    double C2 = exp(C)*sin(-C);
    double C3 = 0; // dBottom * (dmx - dmy)/2;
    double u = middle_current(0) * C1 - middle_current(1) * C2;
    double v = middle_current(0) * C2 - middle_current(1) * C1;
    double w = middle_current(0) * grad[0] + middle_current(1) * grad[1] - C3;
    return Eigen::Vector3d (u, v, w);
}

EkmanUWCurrentModel::Input::Input() : seabed(Seabed("init")), rho(), dTop(), dBottom(), velocity(), phi(), omega(), windTau()
{
}
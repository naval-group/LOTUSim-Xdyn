/*
 * SurfaceElevationInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "SurfaceElevationInterface.hpp"
#include "InternalErrorException.hpp"
#include <ssc/exception_handling.hpp>
#include <string>

/**
 * \note One can optimize the evaluation of the transformation in case only a
 * translation is required. For example, if transformation is defined from
 * NED to NED(body), only a translation is required
 */
template <typename PointType>
PointType compute_position_in_NED_frame(
        const PointType& P,
        const ssc::kinematics::KinematicsPtr& k)
{
    const std::string frame = P.get_frame();
    if (frame!="NED")
    {
        try
        {
            ssc::kinematics::Transform T = k->get("NED", P.get_frame());
            // Create the equivalent transformation just by swapping frame names
            T.swap();
            return T*P;
        }
        catch(const ssc::kinematics::KinematicsException& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::kinematics::KinematicsException, "frame '" << P.get_frame() << "' does not exist");
        }
    }
    return P;
}

SurfaceElevationInterface::SurfaceElevationInterface(
        const ssc::kinematics::PointMatrixPtr& output_mesh_,
        const std::pair<std::size_t,std::size_t>& output_mesh_size_) :
                output_mesh(output_mesh_),
                output_mesh_size(output_mesh_size_),
                relative_wave_height_for_each_point_in_mesh(),
                surface_elevation_for_each_point_in_mesh()
{
}

SurfaceElevationInterface::~SurfaceElevationInterface()
{
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::get_points_on_free_surface(
        const double t,
        const ssc::kinematics::PointMatrixPtr& Mned
        ) const
{
    if (Mned->get_frame()!="NED")
    {
        std::stringstream ss;
        ss << "Points should be expressed in NED frame" <<std::endl;
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,ss.str());
    }
    ssc::kinematics::PointMatrix ret(*Mned);
    const size_t n = (size_t)Mned->m.cols();
    std::vector<double> x(n), y(n);
    for (size_t i = 0; i < n; ++i)
    {
        x[i] = ret.m(0, static_cast<Eigen::Index>(i));
        y[i] = ret.m(1, static_cast<Eigen::Index>(i));
    }
    const std::vector<double> wave_height_ = get_and_check_wave_height(x, y, t);
    for (size_t i = 0; i < n; ++i)
    {
        ret.m(2, static_cast<Eigen::Index>(i)) = wave_height_.at(i);
    }
    return ret;
}

std::vector<double> SurfaceElevationInterface::get_relative_wave_height() const
{
    return relative_wave_height_for_each_point_in_mesh;
}

std::vector<double> SurfaceElevationInterface::get_surface_elevation() const
{
    return surface_elevation_for_each_point_in_mesh;
}

void SurfaceElevationInterface::update_surface_elevation(
    const ssc::kinematics::PointMatrixPtr& P,       //!< Points for which to compute the relative wave height
    const ssc::kinematics::KinematicsPtr& k,        //!< Object used to compute the transforms to the NED frame
    const double t                                  //!< Current instant (in seconds)
    )
{
    const size_t n = (size_t)P->m.cols();
    if (n<=0) return;
    const ssc::kinematics::PointMatrix OP = compute_position_in_NED_frame(*P, k);
    relative_wave_height_for_each_point_in_mesh.resize(n);

    std::vector<double> x(n), y(n);
    for (size_t i = 0; i < n; ++i)
    {
        x[i] = OP.m(0, static_cast<Eigen::Index>(i));
        y[i] = OP.m(1, static_cast<Eigen::Index>(i));
    }
    surface_elevation_for_each_point_in_mesh = get_and_check_wave_height(x, y, t);
    for (size_t i = 0; i < n; ++i)
    {
        relative_wave_height_for_each_point_in_mesh[i] = OP.m(2, static_cast<Eigen::Index>(i)) - surface_elevation_for_each_point_in_mesh.at(i);
    }
}

std::vector<double> SurfaceElevationInterface::get_and_check_wave_height(
    const std::vector<double> &x, //!< x-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
    const std::vector<double> &y, //!< y-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
    const double t                //!< Current instant (in seconds)
    ) const
{
    if (x.size() != y.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating surface elevation: the x and y vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ")");
    }
    return wave_height(x,y,t);
}

std::vector<double> SurfaceElevationInterface::get_and_check_dynamic_pressure(
    const double rho,               //!< water density (in kg/m^3)
    const double g,                 //!< gravity (in m/s^2)
    const std::vector<double> &x,   //!< x-positions in the NED frame (in meters)
    const std::vector<double> &y,   //!< y-positions in the NED frame (in meters)
    const std::vector<double> &z,   //!< z-positions in the NED frame (in meters)
    const std::vector<double> &eta, //!< Wave elevations at (x,y) in the NED frame (in meters)
    const double t                  //!< Current time instant (in seconds)
    ) const
{
    if (x.size() != y.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating dynamic pressures: the x and y vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ")");
    }
    if (x.size() != z.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating dynamic pressures: the x and z vectors don't have the same size (size of x: "
            << x.size() << ", size of z: " << z.size() << ")");
    }
    if (x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating dynamic pressures: the x and eta vectors don't have the same size (size of x: "
            << x.size() << ", size of eta: " << eta.size() << ")");
    }
    return dynamic_pressure(rho, g, x, y, z, eta, t);
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::get_and_check_orbital_velocity(
    const double g,                //!< gravity (in m/s^2)
    const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
    const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
    const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
    const double t,                //!< current instant (in seconds)
    const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
    ) const
{
    if (x.size() != y.size() || x.size() != z.size() || x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating orbital velocity: the x, y, z and eta vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ", size of z: " << z.size() << ", size of eta: " << eta.size() << ")");
    }
    return orbital_velocity(g, x, y, z, t, eta);
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_directions_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<std::vector<double> > SurfaceElevationInterface::get_wave_angular_frequency_for_each_model() const
{
    return std::vector<std::vector<double> >();
}

std::vector<double> SurfaceElevationInterface::get_dynamic_pressure(
    const double rho,                        //!< Water density (in kg/m^3)
    const double g,                          //!< Gravity (in m/s^2)
    const ssc::kinematics::PointMatrix& P,   //!< Positions of points P, relative to the centre of the NED frame, but projected in any frame
    const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
    const std::vector<double>& eta,          //!< Wave elevations at P in the NED frame (in meters)
    const double t                           //!< Current instant (in seconds)
    ) const
{
    const size_t n = (size_t)P.m.cols();
    if (n != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
            "Error when calculating dynamic pressure: the vector of positions of points P and the vector of their corresponding wave elevations don't have the same size (size of P: "
                << n << ", size of eta: " << eta.size() << ")")
    }
    const ssc::kinematics::PointMatrix OP = compute_position_in_NED_frame(P, k);
    std::vector<double> x(n), y(n), z(n);
    for (size_t i = 0; i < n; ++i) {;
        x.at(i) = OP.m(0, static_cast<Eigen::Index>(i));
        y.at(i) = OP.m(1, static_cast<Eigen::Index>(i));
        z.at(i) = OP.m(2, static_cast<Eigen::Index>(i));
    }
    return dynamic_pressure(rho, g, x, y, z, eta, t);
}

ssc::kinematics::PointMatrixPtr SurfaceElevationInterface::get_output_mesh_in_NED_frame(
    const ssc::kinematics::KinematicsPtr& k
    ) const
{
    using namespace ssc::kinematics;
    return PointMatrixPtr(new PointMatrix(compute_position_in_NED_frame(*output_mesh,k)));
}

ssc::kinematics::PointMatrix SurfaceElevationInterface::get_waves_on_mesh(
    const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
    const double t                           //!< Current instant (in seconds)
    ) const
{
    if (output_mesh->m.cols()==0) return ssc::kinematics::PointMatrix("NED",0);
    return get_points_on_free_surface(t, get_output_mesh_in_NED_frame(k));
}

SurfaceElevationGrid SurfaceElevationInterface::get_waves_on_mesh_as_a_grid(
    const ssc::kinematics::KinematicsPtr& k,    //!< Object used to compute the transforms to the NED frame
    const double t                              //!< Current instant (in seconds)
    ) const
{
    ssc::kinematics::PointMatrix res = get_waves_on_mesh(k,t);
    const Eigen::Index nb_points = res.m.cols();
    if (nb_points==0) return SurfaceElevationGrid(t);
    const Eigen::Index nx = static_cast<Eigen::Index>(output_mesh_size.first);
    const Eigen::Index ny = static_cast<Eigen::Index>(output_mesh_size.second);
    if ((nx*ny)!=nb_points)
    {
        std::stringstream ss;
        ss << "SurfaceElevation Problem : " <<std::endl
           << "nx*ny = "<<nx << "*"<<ny <<"="<<nx*ny
           <<" should be equal to res.size() = "<<nb_points<<std::endl<<std::endl
           <<" Make sure your 'environment models' are correctly defined for waves"<<std::endl
           <<" For example, if one declares a 'no waves' model, one should not have an 'output' section"<<std::endl;
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception,ss.str());
    }
    SurfaceElevationGrid s(nx,ny,t);
    for(Eigen::Index i=0; i<nx; ++i)
    {
        s.x(i) = res.m(0,i);
    }
    for(Eigen::Index j=0; j<ny; ++j)
    {
        s.y(j) = res.m(1,j*nx);
    }
    Eigen::Index idx = 0;
    for(Eigen::Index j=0; j<ny; ++j)
    {
        for(Eigen::Index i = 0; i<nx; ++i)
        {
            s.z(i,j) = res.m(2, idx++);
        }
    }
    return s;
}

void SurfaceElevationInterface::serialize_wave_spectra_before_simulation(ObserverPtr&) const
{
}

/*
 * BodyBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "BodyWithSurfaceForces.hpp"
#include "BodyWithoutSurfaceForces.hpp"
#include "yaml2eigen.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "xdyn/exceptions/InternalErrorException.hpp"
#include "xdyn/external_data_structures/YamlBody.hpp"
#include "xdyn/external_data_structures/YamlRotation.hpp"
#include "xdyn/hdb_interpolators/HDBParser.hpp"
#include "xdyn/hdb_interpolators/PrecalParser.hpp"
#include "xdyn/mesh/MeshBuilder.hpp"

#include <ssc/kinematics.hpp>
#include <Eigen/Dense>

bool isSymmetric(const Eigen::MatrixXd& m)
{
    const double tol = 1e-10;
    if (m.rows()!=m.cols()) return false;
    const Eigen::Index n = m.rows();
    for (Eigen::Index i = 0;i<n;++i)
        for (Eigen::Index j = i+1;j<n;++j)
            if (std::abs(m(i,j)-m(j,i))>tol) return false;
    return true;
}
/**
 * \note is based on Sylvester criterion
 */
bool isSymmetricDefinitePositive(const Eigen::MatrixXd& m)
{
    if (!isSymmetric(m)) return false;
    const Eigen::Index n = m.rows();
    for (Eigen::Index i = 1;i<=n;++i)
    {
        if (m.block(0, 0, i, i).determinant()<=0.0)
        {
            return false;
        }
    }
    return true;
}

BodyBuilder::BodyBuilder(const YamlRotation& convention) : rotations(convention)
{
}

void BodyBuilder::change_mesh_ref_frame(BodyStates& states, const VectorOfVectorOfPoints& mesh) const
{
    const ssc::kinematics::Point translation(states.name, states.x_relative_to_mesh, states.y_relative_to_mesh, states.z_relative_to_mesh);
    const ssc::kinematics::Transform transform(translation, states.mesh_to_body, "mesh("+states.name+")");
    states.mesh = MeshPtr(new Mesh(MeshBuilder(mesh).build()));
    const auto T = transform.inverse();
    states.mesh->nodes = (T*ssc::kinematics::PointMatrix(states.mesh->nodes, "mesh("+states.name+")")).m;
    states.mesh->all_nodes = (T*ssc::kinematics::PointMatrix(states.mesh->all_nodes, "mesh("+states.name+")")).m;
    for (size_t i = 0 ; i < states.mesh->facets.size() ; ++i)
    {
        states.mesh->facets[i].centre_of_gravity = T*states.mesh->facets[i].centre_of_gravity;
        states.mesh->facets[i].unit_normal = T.get_rot()*states.mesh->facets[i].unit_normal;
    }
    states.M = ssc::kinematics::PointMatrixPtr(new ssc::kinematics::PointMatrix(states.mesh->nodes, states.name));
}

BodyPtr BodyBuilder::build(const YamlBody& input, const VectorOfVectorOfPoints& mesh, const size_t idx, const double t0, const YamlRotation& convention, const double Tmax, const bool has_surface_forces) const
{
    const StatesFilter states_filter(input.filtered_states);
    BodyStates states(input.filtered_states, std::max(Tmax, states_filter.get_Tmax()));
    states.name = input.name;
    states.G = make_point(input.dynamics.centre_of_inertia);

    states.hydrodynamic_forces_calculation_point = make_point(input.dynamics.hydrodynamic_forces_calculation_point_in_body_frame, input.name);

    states.x_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.x;
    states.y_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.y;
    states.z_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.z;
    states.mesh_to_body = angle2matrix(input.position_of_body_frame_relative_to_mesh.angle, rotations);
    change_mesh_ref_frame(states, mesh);
    add_inertia(states, input.dynamics.rigid_body_inertia, input.dynamics.added_mass);
    states.u.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u);
    states.v.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v);
    states.w.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w);
    states.p.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p);
    states.q.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q);
    states.r.record(t0, input.initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r);
    states.intersector = MeshIntersectorPtr(new MeshIntersector(states.mesh));
    states.convention = convention;

    BodyPtr ret;
    const BlockedDOF blocked_states(input.blocked_dof,idx);
    if (has_surface_forces) ret.reset(new BodyWithSurfaceForces(states,idx,blocked_states, states_filter));
    else                    ret.reset(new BodyWithoutSurfaceForces(states,idx,blocked_states, states_filter));
    return ret;
}

void BodyBuilder::add_inertia(BodyStates& states, const YamlDynamics6x6Matrix& rigid_body_inertia, const YamlDynamics6x6Matrix& added_mass) const
{
    const Eigen::Matrix<double,6,6> Mrb = convert(rigid_body_inertia);
    if(!isSymmetricDefinitePositive(Mrb))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "The rigid body inertia mass matrix is not symmetric definite positive "
                << "for body '" << states.name << "': " << std::endl
                << "Mrb = " << std::endl
                << Mrb << std::endl);
    }
    Eigen::Matrix<double,6,6> Ma;
    if (added_mass.read_from_file)
    {
        if (added_mass.hdb_filename.empty())
        {
            if (added_mass.precal_filename.empty())
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException,
                "Error reading the added mass matrix: xdyn was expecting a filename (as value for key 'from PRECAL_R' or 'from HDB') but an empty string was found."
                );
            }
            else
            {
                Ma = PrecalParser::from_file(added_mass.precal_filename).get_added_mass();
            }
        }
        else
        {
            Ma = HDBParser::from_file(added_mass.hdb_filename).get_added_mass();
        }
    }
    else
    {
        Ma = convert(added_mass);
    }
    if(!isSymmetric(Ma))
    {
        std::cerr << "Warning! The input added mass is not symmetric"
                  << " for body '" << states.name << "': " << std::endl
                  << "Ma = " << std::endl
                  << Ma << std::endl;
    }
    const Eigen::Matrix<double,6,6> Mt = Mrb + Ma;
    if(!isSymmetricDefinitePositive(Mt))
    {
        std::cerr << "Warning! The total inertia matrix (rigid body inertia + added mass) is not symmetric definite positive"
                  << " for body '" << states.name << "': " << std::endl
                  << "Mrb = " << std::endl
                  << Mrb << std::endl
                  << "Ma = " << std::endl
                  << Ma << std::endl
                  << "Mrb+Ma = " << std::endl
                  << Mt << std::endl;
    }
    Eigen::Matrix<double,6,6> M_inv = Mt.inverse();
    states.inverse_of_the_total_inertia = M_inv;
    states.solid_body_inertia = Mrb;
    states.total_inertia = Mt;
}


void convert_matrix_from_aquaplus_to_xdyn_frame(Eigen::Matrix<double,6,6>& matrix);
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

Eigen::Matrix<double,6,6> BodyBuilder::convert(const YamlDynamics6x6Matrix& M) const
{
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

BodyPtr BodyBuilder::build(const std::string& name, const VectorOfVectorOfPoints& mesh, const size_t idx, const double t0, const YamlRotation& convention, const double Tmax, const bool has_surface_forces) const
{
    YamlBody input;
    input.name = name;
    input.dynamics.centre_of_inertia.frame = name;
    input.dynamics.rigid_body_inertia.frame = name;
    input.dynamics.rigid_body_inertia.row_1 = {1,0,0,0,0,0};
    input.dynamics.rigid_body_inertia.row_2 = {0,1,0,0,0,0};
    input.dynamics.rigid_body_inertia.row_3 = {0,0,1,0,0,0};
    input.dynamics.rigid_body_inertia.row_4 = {0,0,0,1,0,0};
    input.dynamics.rigid_body_inertia.row_5 = {0,0,0,0,1,0};
    input.dynamics.rigid_body_inertia.row_6 = {0,0,0,0,0,1};
    input.dynamics.added_mass = input.dynamics.rigid_body_inertia;
    return build(input, mesh, idx, t0, convention, Tmax, has_surface_forces);
}

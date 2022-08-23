/*
 * FastHydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "FastHydrostaticForceModel.hpp"
#include "Body.hpp"
#include "calculate_gz.hpp"
#include "mesh_manipulations.hpp"
#include "Observer.hpp"

std::string FastHydrostaticForceModel::model_name(){return "non-linear hydrostatic (fast)";}

FastHydrostaticForceModel::FastHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env) : ImmersedSurfaceForceModel(model_name(), body_name_, env),
        centre_of_buoyancy(new Eigen::Vector3d())
{
    if (env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" + model_name() + "' needs a wave model, even if it's 'no waves'");
    }
}

FastHydrostaticForceModel::FastHydrostaticForceModel(const std::string& force_name_, const std::string& body_name_, const EnvironmentAndFrames& env) : ImmersedSurfaceForceModel(force_name_, body_name_, env),
        centre_of_buoyancy(new Eigen::Vector3d())
{
}

void FastHydrostaticForceModel::calculations_after_surface_integration(const BodyStates& states) const
{
    auto C = states.intersector->center_of_mass_immersed();

    if (C.all_facets_are_in_same_plane) C.volume = 0;

    for (Eigen::Index i = 0 ; i < 3 ; ++i) centre_of_buoyancy->operator()(i) = C.G(i);
}


std::string FastHydrostaticForceModel::get_name() const
{
    return this->model_name();
}

EPoint FastHydrostaticForceModel::get_application_point(const FacetIterator& that_facet, const BodyStates&, const double) const
{
    return that_facet->centre_of_gravity; // In Body frame
}

ssc::kinematics::Point FastHydrostaticForceModel::get_centre_of_buoyancy() const
{
    return ssc::kinematics::Point(get_body_name(), centre_of_buoyancy->operator()(0),
                                                   centre_of_buoyancy->operator()(1),
                                                   centre_of_buoyancy->operator()(2));
}

std::function<SurfaceForceModel::DF(const FacetIterator &, const size_t, const EnvironmentAndFrames &, const BodyStates &, const double)>
    FastHydrostaticForceModel::get_dF(const FacetIterator& /*begin_facet*/,
                                      const FacetIterator& /*end_facet*/,
                                      const EnvironmentAndFrames& /*env*/,
                                      const BodyStates& /*states*/,
                                      const double /*t*/) const
{
    return [this](const FacetIterator &that_facet,
                  const size_t,
                  const EnvironmentAndFrames &env,
                  const BodyStates &states,
                  const double /*t*/)
    {
        if (that_facet->area == 0) return DF(EPoint(0,0,0),EPoint(0,0,0));
        const double zG = zg_calculator->get_zG_in_NED(that_facet->centre_of_gravity);
        const EPoint dS = that_facet->area*that_facet->unit_normal;
        const EPoint C = get_application_point(that_facet, states, zG);
        return DF(-env.rho*env.g*zG*dS,C);
    };
}

double FastHydrostaticForceModel::pe(const BodyStates& states, const std::vector<double>&, const EnvironmentAndFrames& env) const
{
    const auto G = states.intersector->center_of_mass_immersed();
    const double zC = G.G(2);
    const double immersed_volume = G.volume;
    return -env.rho*env.g*immersed_volume*zC;
}

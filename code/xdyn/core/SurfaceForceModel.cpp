/*
 * SurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "xdyn/core/BodyStates.hpp"
#include "SurfaceForceModel.hpp"

SurfaceForceModel::SurfaceForceModel(const std::string& name_, const std::string& body_name, const EnvironmentAndFrames& env) :
        ForceModel(name_, {}, body_name, env),
        g_in_NED(ssc::kinematics::Point("NED", 0, 0, env.g)),
        zg_calculator(new ZGCalculator())
{
}

SurfaceForceModel::~SurfaceForceModel()
{
}

Wrench SurfaceForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>&) const
{
    zg_calculator->update_transform(env.k->get("NED", states.name));
    Wrench F(states.G, body_name);

    const auto b = begin(states.intersector);
    const auto e = end(states.intersector);
    std::function<SurfaceForceModel::DF(const FacetIterator &,
                                        const size_t,
                                        const EnvironmentAndFrames &,
                                        const BodyStates &,
                                        const double)> dF_lambda =
        get_dF(b, e, env, states, t);

    size_t facet_index = 0;
    for (auto that_facet = begin(states.intersector) ; that_facet != e ; ++that_facet)
    {
        const DF f = dF_lambda(that_facet, facet_index, env, states, t);
        const double x = (f.C(0)-states.G.v(0));
        const double y = (f.C(1)-states.G.v(1));
        const double z = (f.C(2)-states.G.v(2));
        F.X() += f.dF(0);
        F.Y() += f.dF(1);
        F.Z() += f.dF(2);
        F.K() += (y*f.dF(2)-z*f.dF(1));
        F.M() += (z*f.dF(0)-x*f.dF(2));
        F.N() += (x*f.dF(1)-y*f.dF(0));
        ++facet_index;
    }
    calculations_after_surface_integration(states);
    return F;
}

void SurfaceForceModel::calculations_after_surface_integration(const BodyStates&) const
{
}

double SurfaceForceModel::potential_energy(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const
{
    return pe(states, x, env);
}

bool SurfaceForceModel::is_a_surface_force_model() const
{
    return true;
}

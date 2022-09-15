/*
 * GMForceModel.hpp
 *
 *  Created on: April 10, 2015
 *      Author: cady
 */

#ifndef GMFORCEMODEL_HPP_
#define GMFORCEMODEL_HPP_

#include "xdyn/core/ImmersedSurfaceForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"

class Body;

/** \brief Provides an interface to QuadraticDampingForceModel & LinearDampingForceModel
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 */
class GMForceModel : public ImmersedSurfaceForceModel
{
    public:
        struct Yaml : public YamlModel
        {
            Yaml();
            std::string name_of_hydrostatic_force_model;
            double roll_step;
            ForceParser try_to_parse;
        };
        GMForceModel(const Yaml& data, const std::string& body_name, const EnvironmentAndFrames& env);
        std::function<DF(const FacetIterator &,
                         const size_t,
                         const EnvironmentAndFrames &,
                         const BodyStates &,
                         const double
                        )> get_dF(const FacetIterator& begin_facet,
                                  const FacetIterator& end_facet,
                                  const EnvironmentAndFrames& env,
                                  const BodyStates& states,
                                  const double t
                                 ) const;
        static Yaml parse(const std::string& yaml);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;
        void extra_observations(Observer& ) const;
        static std::string model_name();
        double get_GM() const;

    private:
        GMForceModel();
        double get_gz_for_shifted_states(const BodyStates& states, const double t, const EnvironmentAndFrames& env) const;
        BodyStates get_shifted_states(const BodyStates& states,
                const double t) const;
        double pe(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const;

        ForcePtr underlying_hs_force_model;
        double dphi;
        EnvironmentAndFrames env;
        TR1(shared_ptr)<double> GM;
        TR1(shared_ptr)<double> GZ;
};

#endif /* GMFORCEMODEL_HPP_ */

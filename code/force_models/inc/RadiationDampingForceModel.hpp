/*
 * RadiationDampingForceModel.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */

#ifndef RadiationDampingForceModel_HPP_
#define RadiationDampingForceModel_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "ForceModel.hpp"
#include "YamlRadiationDamping.hpp"

class HydroDBParser;

struct EnvironmentAndFrames;

class RadiationDampingForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input() : parser(), yaml(){}
            TR1(shared_ptr)<HydroDBParser> parser;
            YamlRadiationDamping yaml;
        };
        RadiationDampingForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;
        static Input parse(const std::string& yaml, const bool parse_hdb=true);
        static std::string model_name();
        double get_Tmax() const override;

    private:
        RadiationDampingForceModel();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* RadiationDampingForceModel_HPP_ */

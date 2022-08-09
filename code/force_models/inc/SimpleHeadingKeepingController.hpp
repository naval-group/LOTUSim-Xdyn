/*
 * SimpleHeadingKeepingController.hpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#ifndef SIMPLEHEADINGKEEPINGCONTROLLER_HPP_
#define SIMPLEHEADINGKEEPINGCONTROLLER_HPP_

#include "ForceModel.hpp"

class SimpleHeadingKeepingController : public ForceModel
{
    public:
        struct Yaml
        {
            Yaml();
            std::string name;
            double ksi;
            double Tp;
        };
        static Yaml parse(const std::string& yaml);
        static std::string model_name();

        SimpleHeadingKeepingController(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;

    private:
        SimpleHeadingKeepingController();
        double ksi;
        double omega0;
        YamlRotation rotation_convention;
};

#endif /* SIMPLEHEADINGKEEPINGCONTROLLER_HPP_ */

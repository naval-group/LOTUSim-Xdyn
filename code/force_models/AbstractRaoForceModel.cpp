/*
 * AbstractRaoForceModel.cpp
 *
 *  Created on: July 16, 2021
 *      Author: cady
 */

#include "AbstractRaoForceModel.hpp"

#include "xdyn/core/Body.hpp"
#include "hdb_interpolators/HDBParser.hpp"
#include "HydroDBParser.hpp"
#include "hdb_interpolators/PrecalParser.hpp"
#include "exceptions/InvalidInputException.hpp"
#include "exceptions/InternalErrorException.hpp"
#include "yaml.h"
#include "yaml_parser/external_data_structures_parsers.hpp"

std::string AbstractRaoForceModel::get_model_name(const YamlRAO::TypeOfRao& type_of_rao)
{
    switch(type_of_rao)
    {
        case YamlRAO::TypeOfRao::DIFFRACTION_RAO:
            return "diffraction";
        case YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO:
            return "linear Froude-Krylov";
    }
    return "";
}

RaoInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao);
RaoInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao)
{
    return RaoInterpolator(*parser_factory(yaml_rao.hdb_filename, yaml_rao.precal_filename), yaml_rao);
}

RaoInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao, const std::string& hdb_contents);
RaoInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao, const std::string& hdb_contents)
{
    return RaoInterpolator(HDBParser(HDBParser::from_string(hdb_contents)), yaml_rao);
}

AbstractRaoForceModel::AbstractRaoForceModel(const YamlRAO& yaml_rao, const std::string& body_name_, const EnvironmentAndFrames& env):
        ForceModel(get_model_name(yaml_rao.type_of_rao), {}, body_name_, env),
        pimpl(new PhaseModuleRAOEvaluator(rao_interpolator_factory(yaml_rao), env, body_name_, get_model_name(yaml_rao.type_of_rao)))
{
}

AbstractRaoForceModel::AbstractRaoForceModel(const Input& yaml_rao, const std::string& body_name_, const EnvironmentAndFrames& env, const std::string& hdb_file_contents):
        ForceModel(get_model_name(yaml_rao.type_of_rao), {}, body_name_, env),
        pimpl(new PhaseModuleRAOEvaluator(rao_interpolator_factory(yaml_rao, hdb_file_contents), env, body_name_, get_model_name(yaml_rao.type_of_rao)))
{
}

Wrench AbstractRaoForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& /*commands*/) const
{
    return Wrench(ssc::kinematics::Point(body_name, pimpl->get_application_point()), body_name, pimpl->evaluate(states, t, env));
}

AbstractRaoForceModel::Input AbstractRaoForceModel::parse(const std::string& yaml, const YamlRAO::TypeOfRao& type_of_rao)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlRAO ret;
    ret.type_of_rao = type_of_rao;

    if (node.FindValue("hdb"))
    {
        if (node.FindValue("raodb"))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "cannot specify both an HDB filename and a PRECAL_R filename "
                  "(both keys 'hdb' and 'raodb' were found in the YAML file).");
        }
        node["hdb"] >> ret.hdb_filename;
        node["calculation point in body frame"] >> ret.calculation_point;
    }
    else if (node.FindValue("raodb"))
    {
        node["raodb"] >> ret.precal_filename;
        ret.calculation_point = YamlCoordinates(0, 0, 0);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
              "should specify either an HDB filename or a PRECAL_R filename "
              "(no 'hdb' or 'raodb' keys were found in the YAML file).");
    }

    node["mirror for 180 to 360"]           >> ret.mirror;
    parse_optional(node, "use encounter period", ret.use_encounter_period);
    return ret;
}

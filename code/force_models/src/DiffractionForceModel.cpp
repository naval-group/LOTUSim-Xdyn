/*
 * DiffractionForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "DiffractionForceModel.hpp"

#include "Body.hpp"
#include "HDBParser.hpp"
#include "HydroDBParser.hpp"
#include "PrecalParser.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "SurfaceElevationInterface.hpp"
#include "yaml.h"
#include "external_data_structures_parsers.hpp"
#include "yaml2eigen.hpp"

#include <ssc/interpolation.hpp>

#include <array>


std::string DiffractionForceModel::model_name() { return "diffraction";}

std::shared_ptr<HydroDBParser> parser_factory(const std::string& hdb_filename, const std::string& precal_filename);
std::shared_ptr<HydroDBParser> parser_factory(const std::string& hdb_filename, const std::string& precal_filename)
{
    if (hdb_filename.empty())
    {
        return std::shared_ptr<HydroDBParser>(new PrecalParser(PrecalParser::from_file(precal_filename)));
    }
    return std::shared_ptr<HydroDBParser>(new HDBParser(HDBParser::from_file(hdb_filename)));
}

DiffractionInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao);
DiffractionInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao)
{
    return DiffractionInterpolator(*parser_factory(yaml_rao.hdb_filename, yaml_rao.precal_filename), yaml_rao);
}

DiffractionInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao, const std::string& hdb_contents);
DiffractionInterpolator rao_interpolator_factory(const YamlRAO& yaml_rao, const std::string& hdb_contents)
{
    return DiffractionInterpolator(HDBParser(HDBParser::from_string(hdb_contents)), yaml_rao);
}

DiffractionForceModel::DiffractionForceModel(const YamlRAO& yaml_rao, const std::string& body_name_, const EnvironmentAndFrames& env):
        ForceModel(DiffractionForceModel::model_name(), {}, body_name_, env),
        pimpl(new PhaseModuleRAOEvaluator(rao_interpolator_factory(yaml_rao), env, body_name_, DiffractionForceModel::model_name()))
{
}

DiffractionForceModel::DiffractionForceModel(const Input& yaml_rao, const std::string& body_name_, const EnvironmentAndFrames& env, const std::string& hdb_file_contents):
        ForceModel("diffraction", {}, body_name_, env),
        pimpl(new PhaseModuleRAOEvaluator(rao_interpolator_factory(yaml_rao, hdb_file_contents), env, body_name_, DiffractionForceModel::model_name()))
{
}

Wrench DiffractionForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    return Wrench(ssc::kinematics::Point(body_name, pimpl->get_application_point()), body_name, pimpl->evaluate(states, t, env));
}

DiffractionForceModel::Input DiffractionForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlRAO ret;

    if (node.FindValue("hdb"))
    {
        if (node.FindValue("precal"))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "cannot specify both an HDB filename and a PRECAL_R filename "
                  "(both keys 'hdb' and 'precal' were found in the YAML file).");
        }
        node["hdb"] >> ret.hdb_filename;
        node["calculation point in body frame"] >> ret.calculation_point;
    }
    else if (node.FindValue("precal"))
    {
        node["precal"] >> ret.precal_filename;
        ret.calculation_point = YamlCoordinates(0, 0, 0);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
              "should specify either an HDB filename or a PRECAL_R filename "
              "(no 'hdb' or 'precal' keys were found in the YAML file).");
    }

    node["mirror for 180 to 360"]           >> ret.mirror;
    parse_optional(node, "use encounter period", ret.use_encounter_period);
    return ret;
}

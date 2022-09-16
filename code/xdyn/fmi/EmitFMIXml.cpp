/*
 * EmitFMIXml.cpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "EmitFMIXml.hpp"
#include "FMIXml.hpp"
#include "get_sha.hpp"
#include "xdyn/core/Sim.hpp"
#include "xdyn/yaml_parser/SimulatorYamlParser.hpp"
#include "observers_and_api/simulator_api.hpp"

#include <ctime>
#include <iostream>

fmi::DateTime now();
fmi::DateTime now()
{
    std::time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    fmi::DateTime ret;
    ret.year = static_cast<size_t>(now->tm_year + 1900);
    ret.month = static_cast<size_t>(now->tm_mon + 1);
    ret.day = static_cast<size_t>(now->tm_mday);
    ret.hours = static_cast<size_t>(now->tm_hour);
    ret.minutes = static_cast<size_t>(now->tm_min);
    ret.seconds = static_cast<size_t>(now->tm_sec);
    return ret;
}

void put(boost::property_tree::ptree& tree, const std::vector<fmi::Type<fmi::RealType> >& s);
void put(boost::property_tree::ptree& tree, const std::vector<fmi::Type<fmi::RealType> >& s)
{
    for (auto b:s)
    {
        tree.add("TypeDefinitions.Type.<xmlattr>.name", b.name);
        tree.add("TypeDefinitions.Type.<xmlattr>.description", b.description);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.displayUnit", b.type_.displayUnit);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.max", b.type_.max);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.min", b.type_.min);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.nominal", b.type_.nominal);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.quantity", b.type_.quantity);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.relativeQuantity", b.type_.relativeQuantity);
        tree.add("TypeDefinitions.Type.RealType.<xmlattr>.unit", b.type_.unit);
    }
}

void put(boost::property_tree::ptree& tree, const std::vector<fmi::Tool>& s);
void put(boost::property_tree::ptree& tree, const std::vector<fmi::Tool>& s)
{
    for (auto b:s)
    {
        tree.add("VendorAnnotations.Tool.<xmlattr>.name", b.name);
        for (auto i:b.annotations)
        {
            tree.add("VendorAnnotations.Tool.Annotation.name", i.name);
            tree.add("VendorAnnotations.Tool.Annotation.value", i.value);
        }
    }
}

void put(boost::property_tree::ptree& tree, const fmi::RealAttributes& attributes);
void put(boost::property_tree::ptree& tree, const fmi::RealAttributes& attributes)
{
    tree.add("<xmlattr>.declaredType", attributes.declaredType);
    tree.add("<xmlattr>.displayUnit", attributes.displayUnit);
    tree.add("<xmlattr>.fixed", attributes.fixed);
    tree.add("<xmlattr>.max", attributes.max);
    tree.add("<xmlattr>.min", attributes.min);
    tree.add("<xmlattr>.nominal", attributes.nominal);
    tree.add("<xmlattr>.quantity", attributes.quantity);
    tree.add("<xmlattr>.relativeQuantity", attributes.relativeQuantity);
    tree.add("<xmlattr>.start", attributes.start);
    tree.add("<xmlattr>.unit", attributes.unit);
}

void put(boost::property_tree::ptree& tree, const std::vector<fmi::ScalarVariable<fmi::RealAttributes> >& s);
void put(boost::property_tree::ptree& tree, const std::vector<fmi::ScalarVariable<fmi::RealAttributes> >& s)
{
    boost::property_tree::ptree model_variables;
    for (auto b:s)
    {
        boost::property_tree::ptree var;
        var.add("<xmlattr>.name", b.name);
        var.add("<xmlattr>.valueReference", b.valueReference);
        var.add("<xmlattr>.description", b.description);
        switch(b.variability)
        {
            case fmi::Variability::CONSTANT:
                var.add("<xmlattr>.variability", "constant");
                break;
            case fmi::Variability::CONTINUOUS:
                var.add("<xmlattr>.variability", "continuous");
                break;
            case fmi::Variability::DISCRETE:
                var.add("<xmlattr>.variability", "discrete");
                break;
            case fmi::Variability::PARAMETER:
                var.add("<xmlattr>.variability", "parameter");
                break;
            default:
                break;
        }
        switch(b.causality)
        {
            case fmi::Causality::INPUT:
                var.add("<xmlattr>.causality", "input");
                break;
            case fmi::Causality::INTERNAL:
                var.add("<xmlattr>.causality", "internal");
                break;
            case fmi::Causality::NONE:
                var.add("<xmlattr>.causality", "none");
                break;
            case fmi::Causality::OUTPUT:
                var.add("<xmlattr>.causality", "output");
                break;
            default:
                break;
        }
        switch(b.alias)
        {
            case fmi::Alias::ALIAS:
                var.add("<xmlattr>.alias", "alias");
                break;
            case fmi::Alias::NEGATED_ALIAS:
                var.add("<xmlattr>.alias", "negatedAlias");
                break;
            case fmi::Alias::NO_ALIAS:
                var.add("<xmlattr>.alias", "noAlias");
                break;
            default:
                break;
        }
        boost::property_tree::ptree attributes;
        put(attributes, b.attributes);
        var.add_child("Real", attributes);
        model_variables.add_child("ScalarVariable", var);
    }
    tree.add_child("ModelVariables", model_variables);
}


void put(boost::property_tree::ptree& tree, const fmi::DefaultExperiment & s);
void put(boost::property_tree::ptree& tree, const fmi::DefaultExperiment & s)
{
    boost::property_tree::ptree& new_node = tree.add("DefaultExperiment", "");
    new_node.add("<xmlattr>.startTime", s.startTime);
    new_node.add("<xmlattr>.stopTime", s.stopTime);
    new_node.add("<xmlattr>.tolerance", s.tolerance);
}

void put(boost::property_tree::ptree& tree, const std::vector<fmi::DisplayUnitDefinition> & s);
void put(boost::property_tree::ptree& tree, const std::vector<fmi::DisplayUnitDefinition> & s)
{
    for (auto b:s)
    {
        tree.add("DisplayUnitDefinition.displayUnit", b.displayUnit);
        tree.add("DisplayUnitDefinition.gain", b.gain);
        tree.add("DisplayUnitDefinition.offset", b.offset);
    }
}

boost::property_tree::ptree& put(boost::property_tree::ptree& tree, const std::vector<fmi::BaseUnit> & s);
boost::property_tree::ptree& put(boost::property_tree::ptree& tree, const std::vector<fmi::BaseUnit> & s)
{
    boost::property_tree::ptree& new_node = tree.add("UnitDefinitions","");
    for (auto b:s)
    {
        new_node.add("BaseUnit.unit", b.unit);
        put(new_node, b.definitions);
    }
    return new_node;
}

void put(boost::property_tree::ptree& tree, const fmi::NamingConvention& s);
void put(boost::property_tree::ptree& tree, const fmi::NamingConvention& s)
{
    switch(s)
    {
        case fmi::NamingConvention::FLAT:
            tree.put("<xmlattr>.variableNamingConvention", "flat");
            break;
        case fmi::NamingConvention::STRUCTURED:
            tree.put("<xmlattr>.variableNamingConvention", "structured");
            break;
        default:
            break;
    }
}

void twodigits(const size_t d, std::stringstream& ss);
void twodigits(const size_t d, std::stringstream& ss)
{
    ss << std::setfill('0') << std::setw(2) << d;
}

void put(boost::property_tree::ptree& tree, const fmi::DateTime& s);
void put(boost::property_tree::ptree& tree, const fmi::DateTime& s)
{
    std::stringstream ss;
    ss << s.year << "-";
    twodigits(s.month, ss);
    ss << "-";
    twodigits(s.day, ss);
    ss << "T";
    twodigits(s.hours, ss);
    ss << ":";
    twodigits(s.minutes, ss);
    ss << ":";
    twodigits(s.seconds, ss);
    ss << "Z";
    tree.put("<xmlattr>.generationDateAndTime", ss.str());
}

void put(boost::property_tree::ptree& tree, const fmi::Attributes& s);
void put(boost::property_tree::ptree& tree, const fmi::Attributes& s)
{
    tree.put("<xmlattr>.author", s.author);
    tree.put("<xmlattr>.description", s.description);
    tree.put("<xmlattr>.fmiVersion", s.fmiVersion);
    put(tree, s.generationDateAndTime);
    tree.put("<xmlattr>.generationTool", s.generationTool);
    tree.put("<xmlattr>.guid", s.guid);
    tree.put("<xmlattr>.modelName", s.modelName);
    tree.put("<xmlattr>.modelIdentifier", s.modelidentifier);
    tree.put("<xmlattr>.numberOfEventIndicators", s.numberOfEventsIndicators);
    put(tree, s.variableNamingConvention);
    tree.put("<xmlattr>.version", s.version);
    tree.put("<xmlattr>.numberOfContinuousStates", s.numberOfContinuousStates);

}


void put(boost::property_tree::ptree& tree, const fmi::Xml& s);
void put(boost::property_tree::ptree& tree, const fmi::Xml& s)
{
    boost::property_tree::ptree& new_node = tree.add("fmiModelDescription", "");
    put(new_node, s.attributes);
    put(new_node, s.UnitDefinitions);
    put(new_node, s.default_experiment);
    put(new_node, s.real_model_variables);
    put(new_node, s.type_definitions);
    put(new_node, s.vendor_annotations);
}


std::string fmi::emit(const fmi::Xml& xml)
{
    boost::property_tree::ptree tree;
    put(tree, xml);
    std::stringstream ss;
    boost::property_tree::write_xml(ss, tree);
    return ss.str();
}

void fill_common_attributes(fmi::Xml& xml, const YamlSimulatorInput& input);
void fill_common_attributes(fmi::Xml& xml, const YamlSimulatorInput& input)
{
    xml.attributes.author = "SES";
    xml.attributes.description = "Ship & Environment Simulator";
    xml.attributes.fmiVersion = "1.0";
    xml.attributes.modelName = "fmi_simulator";
    xml.attributes.modelidentifier = "fmi_simulator";
    xml.attributes.variableNamingConvention = fmi::NamingConvention::FLAT;
    xml.attributes.numberOfContinuousStates = 13;
    xml.attributes.generationTool = "SES";
    xml.attributes.generationDateAndTime = now();
    xml.default_experiment.stopTime = 10;
    xml.default_experiment.tolerance = 1E-3;
    xml.attributes.guid = fmi::get_sha(input);
}

typedef std::function<std::tuple<double,double,double,double>(const YamlAngle& angle)> GetQuat;

class ScalarVariableBuilder
{
    public:
        ScalarVariableBuilder(const YamlBody& body, const GetQuat& euler2quat_) : body_name(body.name),
                                                      value_reference(0),
                                                      initial_position(body.initial_position_of_body_frame_relative_to_NED_projected_in_NED),
                                                      initial_speed(body.initial_velocity_of_body_frame_relative_to_NED_projected_in_body),
                                                      euler2quat(euler2quat_)
        {
        }

        std::vector<fmi::ScalarVariable<fmi::RealAttributes> > build(const std::vector<std::string>& commands)
        {
            std::vector<fmi::ScalarVariable<fmi::RealAttributes> > ret;
            ret.push_back(build_with_suffix("x", "m", "Distance", initial_position.coordinates.x));
            ret.push_back(build_with_suffix("y", "m", "Distance", initial_position.coordinates.y));
            ret.push_back(build_with_suffix("z", "m", "Distance", initial_position.coordinates.z));
            ret.push_back(build_with_suffix("u", "m/s", "Speed", initial_speed.u));
            ret.push_back(build_with_suffix("v", "m/s", "Speed", initial_speed.v));
            ret.push_back(build_with_suffix("w", "m/s", "Speed", initial_speed.w));
            ret.push_back(build_with_suffix("p", "rad/s", "Angular velocity", initial_speed.p));
            ret.push_back(build_with_suffix("q", "rad/s", "Angular velocity", initial_speed.q));
            ret.push_back(build_with_suffix("r", "rad/s", "Angular velocity", initial_speed.r));
            const auto q = euler2quat(initial_position.angle);
            ret.push_back(build_with_suffix("qr", "", "", std::get<0>(q)));
            ret.push_back(build_with_suffix("qi", "", "", std::get<1>(q)));
            ret.push_back(build_with_suffix("qj", "", "", std::get<2>(q)));
            ret.push_back(build_with_suffix("qk", "", "", std::get<3>(q)));
            for (auto command:commands) ret.push_back(build_without_suffix(command, "", "", 0));
            return ret;
        }

    private:
        ScalarVariableBuilder(); // Disabled
        fmi::ScalarVariable<fmi::RealAttributes> build(const std::string& varname,
                                                       const std::string& unit,
                                                       const std::string quantity,
                                                       const double start)
        {
            fmi::ScalarVariable<fmi::RealAttributes> ret;
            ret.alias = fmi::Alias::NO_ALIAS;
            ret.causality = fmi::Causality::OUTPUT;
            ret.name = varname;
            ret.valueReference = value_reference++;
            ret.attributes.quantity = quantity;
            ret.attributes.unit = unit;
            ret.attributes.relativeQuantity = false;
            ret.attributes.start = start;
            ret.variability = fmi::Variability::CONTINUOUS;
            return ret;
        }
        fmi::ScalarVariable<fmi::RealAttributes> build_with_suffix(const std::string& varname,
                                                       const std::string& unit,
                                                       const std::string quantity,
                                                       const double start)
        {
            return build(varname + "(" + body_name + ")", unit, quantity, start);
        }
        fmi::ScalarVariable<fmi::RealAttributes> build_without_suffix(const std::string& varname,
                                                               const std::string& unit,
                                                               const std::string quantity,
                                                               const double start)
        {
            return build(varname, unit, quantity, start);
        }
        std::string body_name;
        size_t value_reference;
        YamlPosition initial_position;
        YamlSpeed initial_speed;
        GetQuat euler2quat;
};


fmi::Xml fmi::build(const std::string& yaml)
{
    const SimulatorYamlParser parser(yaml);
    auto input = parser.parse();
    fmi::Xml xml;
    fill_common_attributes(xml, input);
    for (auto&& body:input.bodies) body.mesh = "";
    const Sim sim(get_system(input, 0));

    GetQuat get_quaternions = [&sim,&input](const YamlAngle& angle) -> std::tuple<double,double,double,double>
    {
        const ssc::kinematics::EulerAngles e(angle.phi, angle.theta, angle.psi);
        return sim.get_bodies().front()->get_quaternions(e, input.rotations);
    };

    ScalarVariableBuilder builder(input.bodies.front(), get_quaternions);
    xml.real_model_variables = builder.build(sim.get_command_names());
    return xml;
}

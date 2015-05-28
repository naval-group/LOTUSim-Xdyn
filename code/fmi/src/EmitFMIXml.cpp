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


void put(boost::property_tree::ptree& tree, const std::vector<fmi::ScalarVariable<fmi::RealAttributes> >& s);
void put(boost::property_tree::ptree& tree, const std::vector<fmi::ScalarVariable<fmi::RealAttributes> >& s)
{
    for (auto b:s)
    {
        tree.add("ModelVariables.ScalarVariable.<xmlattr>.name", b.name);
        tree.add("ModelVariables.ScalarVariable.<xmlattr>.valueReference", b.valueReference);
        tree.add("ModelVariables.ScalarVariable.<xmlattr>.description", b.description);
        switch(b.variability)
        {
            case fmi::Variability::CONSTANT:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.variability", "constant");
                break;
            case fmi::Variability::CONTINUOUS:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.variability", "continuous");
                break;
            case fmi::Variability::DISCRETE:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.variability", "discrete");
                break;
            case fmi::Variability::PARAMETER:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.variability", "parameter");
                break;
            default:
                break;
        }
        switch(b.causality)
        {
            case fmi::Causality::INPUT:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "input");
                break;
            case fmi::Causality::INTERNAL:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "internal");
                break;
            case fmi::Causality::NONE:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "none");
                break;
            case fmi::Causality::OUTPUT:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "output");
                break;
            default:
                break;
        }
        switch(b.alias)
        {
            case fmi::Alias::ALIAS:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "alias");
                break;
            case fmi::Alias::NEGATED_ALIAS:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "negatedAlias");
                break;
            case fmi::Alias::NO_ALIAS:
                tree.add("ModelVariables.ScalarVariable.<xmlattr>.causality", "noAlias");
                break;
            default:
                break;
        }
    }
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

void put(boost::property_tree::ptree& tree, const fmi::DateTime& s);
void put(boost::property_tree::ptree& tree, const fmi::DateTime& s)
{
    std::stringstream ss;
    ss << s.year << "-" << s.month << "-" << s.day << "T" << s.hours << ":" << s.minutes << ":" << s.seconds << "Z";
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



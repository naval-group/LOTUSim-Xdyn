/*
 * ParseFMIXml.cpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#include <limits> // std::numeric_limits

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "FMIException.hpp"
#include "ParseFMIXml.hpp"
#include <ssc/macros.hpp>

using boost::property_tree::ptree;

template <typename T> typename std::vector<T> parse_vector(const boost::property_tree::ptree& tree, const std::string& name)
{
    std::vector<T> ret;
    for (auto it = tree.begin() ; it != tree.end() ; ++it)
    {
        if (it->first == name)
        {
            T b;
            it->second >> b;
            ret.push_back(b);
        }
    }
    return ret;
}

void operator>>(const boost::property_tree::ptree& tree, fmi::Xml& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::DateTime& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Attributes& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::DisplayUnitDefinition& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::BaseUnit& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::DefaultExperiment& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Annotation& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Tool& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::RealType& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Type<fmi::RealType>& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::RealAttributes& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::ScalarVariable<fmi::RealAttributes>& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::NamingConvention& out);


void operator>>(const boost::property_tree::ptree& tree, fmi::Xml& xml)
{
    for (auto v: tree.get_child("fmiModelDescription"))
    {
        if (v.first == "<xmlattr>")
        {
            v.second >> xml.attributes;
        }
        if (v.first == "UnitDefinitions")
        {
            xml.UnitDefinitions = parse_vector<fmi::BaseUnit>(v.second, "BaseUnit");
        }
        if (v.first == "DefaultExperiment")
        {
            v.second >> xml.default_experiment;
        }
        if (v.first == "ModelVariables")
        {
            xml.real_model_variables = parse_vector<fmi::ScalarVariable<fmi::RealAttributes> >(v.second, "ScalarVariable");
        }
        if (v.first == "TypeDefinitions") xml.type_definitions = parse_vector<fmi::Type<fmi::RealType> >(v.second, "Type");
    }
}

void operator>>(const boost::property_tree::ptree& tree, fmi::BaseUnit& out)
{
    out.definitions = parse_vector<fmi::DisplayUnitDefinition>(tree, "DisplayUnitDefinition");
    out.unit = tree.get<std::string>("<xmlattr>.unit");
}

void operator>>(const boost::property_tree::ptree& tree, fmi::NamingConvention& out)
{
    const std::string name = tree.get<std::string>("variableNamingConvention");
    if (name == "flat")       out = fmi::NamingConvention::FLAT;
    if (name == "structured") out = fmi::NamingConvention::STRUCTURED;
}

void operator>>(const boost::property_tree::ptree& tree, fmi::DisplayUnitDefinition& out)
{
    out.displayUnit = tree.get<std::string>("<xmlattr>.displayUnit");
    out.gain = tree.get<double>("<xmlattr>.gain");
    out.offset = tree.get<double>("<xmlattr>.offset",0);
}

void operator>>(const boost::property_tree::ptree& tree, fmi::DefaultExperiment& out)
{
    out.startTime = tree.get<double>("<xmlattr>.startTime");
    out.stopTime = tree.get<double>("<xmlattr>.stopTime");
    out.tolerance = tree.get<double>("<xmlattr>.tolerance");
}

size_t convert(const std::string& s);
size_t convert(const std::string& s)
{
    std::stringstream ss(s);
    size_t out;
    ss >> out;
    return out;
}

void operator>>(const boost::property_tree::ptree& tree, fmi::DateTime& out)
{
    const std::string date_time = tree.get<std::string>("generationDateAndTime");
    if (date_time.size() != 20)
    {
        std::stringstream ss;
        ss << "Invalid date/time: expected YYYY-MM-DDThh:mm:ssZ but received " << date_time;
        THROW(__PRETTY_FUNCTION__, fmi::Exception, ss.str());
    }
    out.year    = convert(date_time.substr(0,4));
    out.month   = convert(date_time.substr(5,2));
    out.day     = convert(date_time.substr(8,2));
    out.hours   = convert(date_time.substr(11,2));
    out.minutes = convert(date_time.substr(14,2));
    out.seconds = convert(date_time.substr(17,2));
}

void operator>>(const boost::property_tree::ptree& tree, fmi::Attributes& out)
{
    out.author = tree.get<std::string>("author", "");
    out.description = tree.get<std::string>("description");
    out.fmiVersion = tree.get<std::string>("fmiVersion");
    tree >> out.generationDateAndTime;
    out.generationTool = tree.get<std::string>("generationTool");
    out.guid = tree.get<std::string>("guid");
    out.modelName = tree.get<std::string>("modelName");
    out.modelidentifier = tree.get<std::string>("modelIdentifier");
    out.numberOfEventsIndicators = tree.get<size_t>("numberOfEventIndicators");
    tree >> out.variableNamingConvention;
    out.version = tree.get<std::string>("version");
}

fmi::Alias get_alias(const boost::property_tree::ptree& tree, const std::string& str);
fmi::Alias get_alias(const boost::property_tree::ptree& tree, const std::string& str)
{
    const std::string s = tree.get<std::string>(str, "");
    if (s == "alias")
    {
        return fmi::Alias::ALIAS;
    }
    if (s == "negatedAlias")
    {
        return fmi::Alias::NEGATED_ALIAS;
    }
    return fmi::Alias::NO_ALIAS;
}

fmi::Causality get_causality(const boost::property_tree::ptree& tree, const std::string& str);
fmi::Causality get_causality(const boost::property_tree::ptree& tree, const std::string& str)
{
    const std::string s = tree.get<std::string>(str, "");
    if (s == "input")
    {
        return fmi::Causality::INPUT;
    }
    if (s == "output")
    {
        return fmi::Causality::OUTPUT;
    }
    if (s == "internal")
    {
        return fmi::Causality::INTERNAL;
    }
    return fmi::Causality::NONE;
}

fmi::Variability get_variability(const boost::property_tree::ptree& tree, const std::string& str);
fmi::Variability get_variability(const boost::property_tree::ptree& tree, const std::string& str)
{
    const std::string s = tree.get<std::string>(str, "");
    if (s == "input")
    {
        return fmi::Variability::CONSTANT;
    }
    if (s == "output")
    {
        return fmi::Variability::CONTINUOUS;
    }
    if (s == "internal")
    {
        return fmi::Variability::DISCRETE;
    }
    return fmi::Variability::PARAMETER;
}

void operator>>(const boost::property_tree::ptree& tree, fmi::ScalarVariable<fmi::RealAttributes>& out)
{
    if (tree.find("Real") != tree.not_found())
    {
        out.alias = get_alias(tree, "<xmlattr>.alias");
        tree.get_child("Real") >> out.attributes;
        out.causality = get_causality(tree, "<xmlattr>.causality");
        out.description = tree.get<std::string>("<xmlattr>.description");
        auto t = tree.find("<xmlattr>.DirectDependency");
        if (t != tree.not_found())
        {
            for (auto it = t->second.begin() ; it != t->second.end() ; ++it)
            {
                if (it->first == "Name")
                {
                    out.direct_dependency.push_back(it->second.get<std::string>("Name"));
                }
            }
        }
        out.name = tree.get<std::string>("<xmlattr>.name");
        out.valueReference = tree.get<size_t>("<xmlattr>.valueReference");
        out.variability = get_variability(tree, "<xmlattr>.variability");
    }
}

void operator>>(const boost::property_tree::ptree& tree, fmi::RealAttributes& out)
{
    out.declaredType = tree.get<std::string>("<xmlattr>.declaredType", "");
    out.displayUnit = tree.get<std::string>("<xmlattr>.displayUnit", "unit");
    out.fixed = tree.get<bool>("<xmlattr>.fixed", false);
    out.max = tree.get<double>("<xmlattr>.max", std::numeric_limits<double>::max());
    out.min = tree.get<double>("<xmlattr>.min", std::numeric_limits<double>::lowest());
    out.nominal = tree.get<double>("<xmlattr>.nominal", 1);
    out.quantity = tree.get<std::string>("<xmlattr>.quantity","");
    out.relativeQuantity = tree.get<bool>("<xmlattr>.relativeQuantity", false);
    out.start = tree.get<double>("<xmlattr>.start");
    out.unit = tree.get<std::string>("<xmlattr>.unit", "");
}

void operator>>(const boost::property_tree::ptree& tree, fmi::Type<fmi::RealType>& out)
{
    out.description = tree.get<std::string>("<xmlattr>.description", "");
    out.name = tree.get<std::string>("<xmlattr>.name", "");
    tree.get_child("RealType") >> out.type_;
}

void operator>>(const boost::property_tree::ptree& tree, fmi::RealType& out)
{
    out.displayUnit = tree.get<std::string>("<xmlattr>.description", "unit");
    out.max = tree.get<double>("<xmlattr>.max", std::numeric_limits<double>::max());
    out.min = tree.get<double>("<xmlattr>.min", std::numeric_limits<double>::lowest());
    out.nominal = tree.get<double>("<xmlattr>.nominal", 1);
    out.quantity = tree.get<std::string>("<xmlattr>.quantity","");
    out.relativeQuantity = tree.get<bool>("<xmlattr>.relativeQuantity", false);
    out.unit = tree.get<std::string>("<xmlattr>.unit", "");
}

fmi::Xml fmi::parse(const std::string& xml)
{
    ptree pt;
    std::istringstream is(xml);
    read_xml(is, pt);
    Xml ans;
    pt >> ans;
    return ans;
}


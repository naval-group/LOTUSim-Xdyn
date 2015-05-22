/*
 * ParseFMIXml.cpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */
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
void operator>>(const boost::property_tree::ptree& tree, fmi::Variability& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Causality& out);
void operator>>(const boost::property_tree::ptree& tree, fmi::Alias& out);


void operator>>(const boost::property_tree::ptree& tree, fmi::Xml& xml)
{
    for (auto v: tree.get_child("fmiModelDescription"))
    {
        if (v.first == "<xmlattr>") v.second >> xml.attributes;
        if (v.first == "UnitDefinitions") xml.UnitDefinitions = parse_vector<fmi::BaseUnit>(v.second, "BaseUnit");
        if (v.first == "DefaultExperiment") v.second >> xml.default_experiment;
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

fmi::Xml fmi::parse(const std::string& xml)
{
    ptree pt;
    std::istringstream is(xml);
    read_xml(is, pt);
    Xml ans;
    pt >> ans;
    return ans;
}


/*
 * parse_output.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */

#include <boost/algorithm/string/predicate.hpp>
#include "yaml.h"
#include "InvalidInputException.hpp"
#include "parse_address.hpp"
#include "parse_output.hpp"

void operator >> (const YAML::Node& node, YamlOutput& f);
void operator >> (const YAML::Node& node, YamlOutput& f)
{
    if(const YAML::Node *pName = node.FindValue("filename"))
    {
        *pName >> f.filename;
    }
    if(const YAML::Node *pName = node.FindValue("address"))
    {
        *pName >> f.address;
    }
    if(const YAML::Node *pName = node.FindValue("port"))
    {
        *pName >> f.port;
    }
    node["format"]   >> f.format;
    node["data"]     >> f.data;
    if (node.FindValue("full output"))
    {
        node["full output"] >> f.full_output;
    }
}

std::vector<YamlOutput> parse_output(const std::string& yaml)
{
    std::vector<YamlOutput> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    try
    {
        node["output"] >> ret;
    }
    catch(std::exception& ) // Nothing to do: 'output' section is not mandatory
    {
    }
    return ret;
}

std::string get_format_for_wave_observer(const std::string& filename)
{
    const size_t n = filename.size();
    if (n<=3)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid file format for wave output file. Expected string should be at least 4 characters");
    }
    if (filename.substr(n-3,3)==".h5")   return "hdf5";
    if (filename.substr(n-5,5)==".hdf5") return "hdf5";
    if (filename.substr(n-3,3)==".H5")   return "hdf5";
    if (filename.substr(n-5,5)==".HDF5") return "hdf5";
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid file format for wave output file. Expected filename extensions are h5, hdf5");
    }
}

std::string get_format(const std::string& filename)
{
    const size_t n = filename.size();
    if (!n)                              return "tsv";
    if (n<=3)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid file format for output file. E");
    }
    if (filename.substr(n-3,3)==".h5")   return "hdf5";
    if (filename.substr(n-5,5)==".hdf5") return "hdf5";
    if (filename.substr(n-4,4)==".csv")  return "csv";
    if (filename.substr(n-4,4)==".tsv")  return "tsv";
    if (filename.substr(n-5,5)==".json") return "json";
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Could not recognize the format of specified output file '" << filename << "': expected filename extensions are .tsv, .csv, .h5, .hdf5 or .json");
    }
}

YamlOutput build_YamlOutput_from_filename(const std::string& filename)
{
    YamlOutput out;
    if ((filename.empty()) or (filename=="tsv"))
    {
        out.format = "tsv";
        out.filename = "";
    }
    else if (filename=="csv")
    {
        out.format = "csv";
        out.filename = "";
    }
    else if (filename=="json")
    {
        out.format = "json";
        out.filename = "";
    }
    else if ((filename=="h5") or (filename=="hdf5"))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Need to specify an input filename for HDF5 export from command line");
    }
    else if (boost::algorithm::starts_with(filename,"ws") or
             boost::algorithm::starts_with(filename,"wss"))
    {
        out = build_YamlOutput_from_WS_URL(filename);
    }
    else
    {
        out.format = get_format(filename);
        out.filename = filename;
    }
    return out;
}

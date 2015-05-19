/*
 * FMI.cpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#include <functional> // std::hash
#include <sstream>

#include <ssc/text_file_reader.hpp>

#include "calculate_hash.hpp"
#include <boost/functional/hash.hpp>
#include "FMI.hpp"
#include "FMIException.hpp"
#include "SimulatorYamlParser.hpp"
#include "simulator_api.hpp"
#include "WaveModel.hpp"

const char* fmiGetModelTypesPlatform()
{
    return fmiModelTypesPlatform;
}

const char* fmiGetVersion()
{
    return fmiVersion;
}

fmiComponent fmiInstantiateModel (fmiString            instanceName,
                                  fmiString            GUID,
                                  fmiCallbackFunctions functions,
                                  fmiBoolean           loggingOn)
{
    const ssc::text_file_reader::TextFileReader yaml_reader("simulator_conf.yml");
    return (fmiComponent)new FMI(instanceName, GUID, functions, loggingOn, yaml_reader.get_contents());
}

std::string sha(const std::string& s, const YamlSimulatorInput& i);
std::string sha(const std::string& s, const YamlSimulatorInput& i)
{
    std::stringstream ss;
    boost::hash<YamlSimulatorInput> hash;
    std::size_t seed = hash(i);
    boost::hash_combine(seed, s);
    ss << seed;
    return ss.str();
}

FMI::FMI(const std::string& instance_name_,
         const std::string& GUID,
         const fmiCallbackFunctions& callbacks,
         const bool logging_on_,
         const std::string& yaml) :
      instance_name(instance_name_),
      logging_on(logging_on_),
      log([callbacks](fmiComponent c,fmiString instanceName,fmiStatus status,fmiString category,fmiString message){callbacks.logger(c, instanceName, status, category, message);}),
      allocate(callbacks.allocateMemory),
      free_memory(callbacks.freeMemory),
      input(SimulatorYamlParser(yaml).parse()),
      sim(get_system(input, 0))
{

    const std::string expected_GUID = sha("@GIT_SHA1@", input);
    if (GUID != expected_GUID)
    {
        std::stringstream ss;
        ss << "Invalid GUID: expected " << expected_GUID << ", but got " << GUID;
        THROW(__PRETTY_FUNCTION__, FMIException, ss.str());
    }
}

FMI::FMI(const std::string& instance_name_,
         const fmiCallbackFunctions& callbacks,
         const bool logging_on_,
         const std::string& yaml) :
      instance_name(instance_name_),
      logging_on(logging_on_),
      log([callbacks](fmiComponent c,fmiString instanceName,fmiStatus status,fmiString category,fmiString message){callbacks.logger(c, instanceName, status, category, message);}),
      allocate(callbacks.allocateMemory),
      free_memory(callbacks.freeMemory),
      input(SimulatorYamlParser(yaml).parse()),
      sim(get_system(input, 0))
{
}

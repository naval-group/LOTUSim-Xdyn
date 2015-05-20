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

#define ERROR(msg) std::stringstream ss;\
                   ss << msg;\
                   ((FMI*)c)->error(ss.str());

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
    fmiComponent ret = NULL;
    try
    {
        const ssc::text_file_reader::TextFileReader yaml_reader("simulator_conf.yml");
        ret = (fmiComponent)new FMI(instanceName, GUID, functions, loggingOn, yaml_reader.get_contents());
    }
    catch(const std::exception& e)
    {
        functions.logger(ret, instanceName, fmiFatal, "ERROR", e.what());
    }
    return ret;
}

void fmiFreeModelInstance(fmiComponent c)
{
    if (c) delete (FMI*)c;
}

fmiStatus fmiSetDebugLogging  (fmiComponent , fmiBoolean )
{
    return fmiOK;
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

fmiStatus fmiSetTime (fmiComponent c, fmiReal time)
{
    ((FMI*)c)->set_time(time);
    return fmiOK;
}

void FMI::error(const std::string& msg) const
{
    if (logging_on) this->log((fmiComponent)this, instance_name.c_str(), fmiError, "ERROR", msg.c_str());
}

void FMI::set_time(const double t_)
{
    t = t_;
}

void FMI::set_continuous_states(const std::vector<double>& new_states)
{
    states = new_states;
}

std::vector<std::string> FMI::get_command_names() const
{
    return command_names;
}

fmiStatus fmiCompletedIntegratorStep(fmiComponent , fmiBoolean* callEventUpdate)
{
    if (callEventUpdate) *callEventUpdate = fmiFalse;
    return fmiOK;
}

fmiStatus fmiSetContinuousStates    (fmiComponent c, const fmiReal x[], size_t nx)
{
    if (nx != 13)
    {
        ERROR("Invalid number of states: expected 13 but got " << nx);
        return fmiFatal;
    }
    std::vector<double> vx(x, x+nx);
    ((FMI*)c)->set_continuous_states(vx);
    return fmiOK;
}

fmiStatus fmiSetReal (fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiReal    value[])
{
    const std::vector<size_t> value_references(vr, vr+nvr);
    const std::vector<double> values(value, value+nvr);
    ((FMI*)c)->set_real(value_references, values);
    return fmiOK;
}

void FMI::set_real(const std::vector<size_t>& value_references, const std::vector<double>& values)
{
    ssc::data_source::DataSource& ds = sim.get_command_listener();
    for (size_t i = 0 ; i < value_references.size() ; ++i)
    {
        const size_t idx = value_references.at(i);
        ds.set(command_names.at(idx), values.at(i));
    }
}

fmiStatus fmiSetInteger(fmiComponent, const fmiValueReference[], size_t, const fmiInteger[])
{
    return fmiOK;
}

fmiStatus fmiSetBoolean(fmiComponent, const fmiValueReference[], size_t, const fmiBoolean[])
{
    return fmiOK;
}

fmiStatus fmiSetString(fmiComponent, const fmiValueReference[], size_t, const fmiString[])
{
    return fmiOK;
}

fmiStatus fmiInitialize(fmiComponent , fmiBoolean , fmiReal , fmiEventInfo* eventInfo)
{
    eventInfo->iterationConverged          = fmiTrue;
    eventInfo->stateValueReferencesChanged = fmiTrue;
    eventInfo->stateValuesChanged          = fmiTrue;
    eventInfo->terminateSimulation         = fmiFalse;
    eventInfo->upcomingTimeEvent           = fmiFalse;
    return fmiOK;
}

fmiStatus fmiGetDerivatives(fmiComponent c, fmiReal derivatives[], size_t nx)
{
    try
    {
        const std::vector<double> dx_dt = ((FMI*)c)->get_derivatives();
        for (size_t i = 0 ; i < nx ; ++i) derivatives[i] = dx_dt.at(i);
        return fmiOK;
    }
    catch(const std::exception& e)
    {
        ((FMI*)c)->error(e.what());
    }
    return fmiError;
}

fmiStatus fmiGetEventIndicators(fmiComponent, fmiReal[], size_t)
{
    return fmiOK;
}

std::vector<double> FMI::get_derivatives()
{
    std::vector<double> dxdt;
    sim(states, dxdt, t);
    return dxdt;
}

fmiStatus fmiGetInteger(fmiComponent, const fmiValueReference[], size_t, fmiInteger[])
{
    return fmiOK;
}

fmiStatus fmiGetBoolean(fmiComponent, const fmiValueReference[], size_t, fmiBoolean[])
{
    return fmiOK;
}

fmiStatus fmiGetString(fmiComponent, const fmiValueReference[], size_t, fmiString[])
{
    return fmiOK;
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
      sim(get_system(input, 0)),
      t(0),
      states(),
      command_names(sim.get_command_names())
{

    const std::string expected_GUID = sha("@GIT_SHA1@", input);
    if (GUID != expected_GUID)
    {
        std::stringstream ss;
        ss << "Invalid GUID: expected " << expected_GUID << ", but got " << GUID;
        THROW(__PRETTY_FUNCTION__, FMIException, ss.str());
    }
}

fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal value[])
{
    try
    {
        const std::vector<size_t> idx(vr, vr+nvr);
        const std::vector<double> r = ((FMI*)c)->get_real(idx);
        for (size_t i = 0 ; i < nvr ; ++i)
        {
            value[i] = r.at(i);
        }
        return fmiOK;
    }
    catch(const std::exception& e)
    {
        ((FMI*)c)->error(e.what());
    }
    return fmiError;
}

std::vector<double> FMI::get_real(const std::vector<size_t>& value_references)
{
    std::vector<double> ret;
    ssc::data_source::DataSource& ds = sim.get_command_listener();
    for (auto idx:value_references) ret.push_back(ds.get<double>(command_names.at(idx)));
    return ret;
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
      sim(get_system(input, 0)),
      t(0),
      states(),
      command_names(sim.get_command_names())
{
}

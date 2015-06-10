/*
 * FMI::Generator.cpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#include <functional> // std::hash
#include <sstream>

#include <ssc/text_file_reader.hpp>

#include "get_sha.hpp"
#include "FMI.hpp"
#include "FMIException.hpp"
#include "SimulatorYamlParser.hpp"
#include "simulator_api.hpp"
#include "WaveModel.hpp"

#include <ssc/macros.hpp>

#define ERROR(msg) std::stringstream ss;\
                   ss << msg;\
                   ((fmi::API*)c)->error(ss.str());

#define CHECK_COMPONENT(c) if (!c) return fmiFatal;
#define CHECK_POINTER(p) if (!p) {ERROR("Null pointer received for " #p);return fmiFatal;}
#define CHECK_VALUE(x, target) if (x!=target) {ERROR("Invalid value of " << #x << ": expected " << target);return fmiFatal;}

const char* fmiGetModelTypesPlatform()
{
    return fmiModelTypesPlatform;
}

const char* fmiGetVersion()
{
    return fmiVersion;
}

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

std::string get_working_directory();
std::string get_working_directory()
{
     char cCurrentPath[FILENAME_MAX];

     if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
     {
         return "";
     }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    return std::string(cCurrentPath);
}

fmiComponent fmiInstantiateModel (fmiString            instanceName,
                                  fmiString            GUID,
                                  fmiCallbackFunctions functions,
                                  fmiBoolean           loggingOn)
{
    fmiComponent ret = NULL;
    try
    {
        const ssc::text_file_reader::TextFileReader yaml_reader("resources/simulator_conf.yml");
        ret = (fmiComponent)new fmi::API(instanceName, GUID, functions, loggingOn, yaml_reader.get_contents());
    }
    catch(const std::exception& e)
    {
        functions.logger(ret, instanceName, fmiFatal, "ERROR", e.what());
    }
    return ret;
}

void fmiFreeModelInstance(fmiComponent c)
{
    if (c) delete (fmi::API*)c;
}

fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean )
{
    CHECK_COMPONENT(c);
    return fmiOK;
}

fmiStatus fmiSetTime(fmiComponent c, fmiReal time)
{
    CHECK_COMPONENT(c);
    ((fmi::API*)c)->set_time(time);
    return fmiOK;
}

void fmi::API::error(const std::string& msg) const
{
    if (logging_on) this->log((fmiComponent)this, instance_name.c_str(), fmiError, "ERROR", msg.c_str());
}

void fmi::API::set_time(const double t_)
{
    t = t_;
}

void fmi::API::set_continuous_states(const std::vector<double>& new_states)
{
    sim.state = new_states;
}

std::vector<std::string> fmi::API::get_command_names() const
{
    return command_names;
}

fmiStatus fmiCompletedIntegratorStep(fmiComponent c, fmiBoolean* callEventUpdate)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(callEventUpdate);
    if (callEventUpdate) *callEventUpdate = fmiFalse;
    return fmiOK;
}

fmiStatus fmiSetContinuousStates(fmiComponent c, const fmiReal x[], size_t nx)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(x);
    CHECK_VALUE(nx, ((fmi::API*)c)->get_nb_of_states());
    std::vector<double> vx(x, x+nx);
    ((fmi::API*)c)->set_continuous_states(vx);
    return fmiOK;
}

fmiStatus fmiSetReal (fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiReal value[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(value);
    CHECK_VALUE(nvr, ((fmi::API*)c)->get_nb_of_real_variables());
    const std::vector<size_t> value_references(vr, vr+nvr);
    const std::vector<double> values(value, value+nvr);
    ((fmi::API*)c)->set_real(value_references, values);
    return fmiOK;
}

void fmi::API::set_real(const std::vector<size_t>& value_references, const std::vector<double>& values)
{
    ssc::data_source::DataSource& ds = sim.get_command_listener();
    for (size_t i = 0 ; i < value_references.size() ; ++i)
    {
        const size_t idx = value_references.at(i);
        ds.set(command_names.at(idx), values.at(i));
    }
}

fmiStatus fmiSetInteger(fmiComponent c, const fmiValueReference vr[], size_t n, const fmiInteger i[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(i);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiSetBoolean(fmiComponent c, const fmiValueReference vr[], size_t n, const fmiBoolean b[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(b);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiSetString(fmiComponent c, const fmiValueReference vr[], size_t n, const fmiString s[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(s);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiInitialize(fmiComponent c, fmiBoolean , fmiReal , fmiEventInfo* eventInfo)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(eventInfo);
    eventInfo->iterationConverged          = fmiTrue;
    eventInfo->stateValueReferencesChanged = fmiTrue;
    eventInfo->stateValuesChanged          = fmiTrue;
    eventInfo->terminateSimulation         = fmiFalse;
    eventInfo->upcomingTimeEvent           = fmiFalse;
    return fmiOK;
}

fmiStatus fmiGetDerivatives(fmiComponent c, fmiReal derivatives[], size_t nx)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(derivatives);
    CHECK_VALUE(nx, ((fmi::API*)c)->get_nb_of_states());
    try
    {
        const std::vector<double> dx_dt = ((fmi::API*)c)->get_derivatives();
        for (size_t i = 0 ; i < nx ; ++i) derivatives[i] = dx_dt.at(i);
        return fmiOK;
    }
    catch(const std::exception& e)
    {
        ((fmi::API*)c)->error(e.what());
    }
    return fmiError;
}

fmiStatus fmiGetEventIndicators(fmiComponent c, fmiReal r[], size_t n)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(r);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

std::vector<double> fmi::API::get_derivatives()
{
    std::vector<double> dxdt(get_nb_of_states(),0);
    sim(sim.state, dxdt, t);
    return dxdt;
}

fmiStatus fmiGetInteger(fmiComponent c, const fmiValueReference vr[], size_t n, fmiInteger i[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(i);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiGetBoolean(fmiComponent c, const fmiValueReference vr[], size_t n, fmiBoolean b[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(b);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiGetString(fmiComponent c, const fmiValueReference vr[], size_t n, fmiString s[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(s);
    CHECK_VALUE(n, 0);
    return fmiOK;
}

fmiStatus fmiEventUpdate(fmiComponent c, fmiBoolean, fmiEventInfo* eventInfo)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(eventInfo);
    eventInfo->iterationConverged          = fmiTrue;
    eventInfo->stateValueReferencesChanged = fmiFalse;
    eventInfo->stateValuesChanged          = fmiFalse;
    eventInfo->terminateSimulation         = fmiFalse;
    eventInfo->upcomingTimeEvent           = fmiFalse;
    return fmiOK;
}

fmiStatus fmiGetContinuousStates(fmiComponent c, fmiReal states[], size_t nx)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(states);
    CHECK_VALUE(nx, ((fmi::API*)c)->get_nb_of_states());
    const std::vector<double> s = ((fmi::API*)c)->get_continuous_states();
    CHECK_VALUE(s.size(), ((fmi::API*)c)->get_nb_of_states());
    for (size_t i = 0 ; i < nx ; ++i) states[i] = s.at(i);
    return fmiOK;
}

fmiStatus fmiGetNominalContinuousStates(fmiComponent c, fmiReal x_nominal[], size_t nx)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(x_nominal);
    CHECK_VALUE(nx, ((fmi::API*)c)->get_nb_of_states());
    for (size_t i = 0 ; i < nx ; ++i) x_nominal[i] = 1.0;
    return fmiOK;
}

fmiStatus fmiGetStateValueReferences(fmiComponent c, fmiValueReference vrx[], size_t nx)
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vrx);
    CHECK_VALUE(nx, ((fmi::API*)c)->get_nb_of_states());
    for (size_t i = 0 ; i < nx ; ++i) vrx[i] = (fmiValueReference)i;
    return fmiOK;
}

fmiStatus fmiTerminate(fmiComponent c)
{
    CHECK_COMPONENT(c);
    return fmiOK;
}

std::vector<double> fmi::API::get_continuous_states() const
{
    return sim.state;
}

size_t fmi::API::get_nb_of_states() const
{
    return 13;
}

size_t fmi::API::get_nb_of_real_variables() const
{
    return command_names.size() + 13;
}

void fmi::API::check_guid(const std::string& GUID, const YamlSimulatorInput& input) const
{
    const std::string expected_GUID = get_sha(input);
    if (not(GUID.empty()) and (GUID != expected_GUID))
    {
        std::stringstream ss;
        ss << "Invalid GUID: expected " << expected_GUID << ", but got " << GUID;
        THROW(__PRETTY_FUNCTION__, Exception, ss.str());
    }
}

Sim get_sim(const YamlSimulatorInput& input);
Sim get_sim(const YamlSimulatorInput& input)
{
    if (input.bodies.empty()) return get_system(input, 0);
    const std::string mesh = input.bodies.front().mesh;
    if (mesh.empty())         return get_system(input, 0);
    const std::string stl_file = ssc::text_file_reader::TextFileReader(std::string("resources/") + mesh).get_contents();
                              return get_system(input, stl_file, 0);
}

fmi::API::API(const std::string& instance_name_,
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
      sim(get_sim(input)),
      t(0),
      command_names(sim.get_command_names())
{
    check_guid(GUID, input);
}

fmi::API::API(const std::string& instance_name_,
         const std::string& GUID,
         const fmiCallbackFunctions& callbacks,
         const bool logging_on_,
         const std::string& yaml,
         const std::string& stl) :
      instance_name(instance_name_),
      logging_on(logging_on_),
      log([callbacks](fmiComponent c,fmiString instanceName,fmiStatus status,fmiString category,fmiString message){callbacks.logger(c, instanceName, status, category, message);}),
      allocate(callbacks.allocateMemory),
      free_memory(callbacks.freeMemory),
      input(SimulatorYamlParser(yaml).parse()),
      sim(get_system(input, stl, 0)),
      t(0),
      command_names(sim.get_command_names())
{
    check_guid(GUID, input);
}

fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal value[])
{
    CHECK_COMPONENT(c);
    CHECK_POINTER(vr);
    CHECK_POINTER(value);
    CHECK_VALUE(nvr, ((fmi::API*)c)->get_nb_of_real_variables());
    try
    {
        const std::vector<size_t> idx(vr, vr+nvr);
        const std::vector<double> r = ((fmi::API*)c)->get_real(idx);
        for (size_t i = 0 ; i < nvr ; ++i)
        {
            value[i] = r.at(i);
        }
        return fmiOK;
    }
    catch(const std::exception& e)
    {
        ((fmi::API*)c)->error(e.what());
    }
    return fmiError;
}

std::vector<double> fmi::API::get_real(const std::vector<size_t>& value_references)
{
    std::vector<double> ret;
    ssc::data_source::DataSource& ds = sim.get_command_listener();
    for (auto idx:value_references)
    {
        if (idx < 13) ret.push_back(sim.state.at(idx));
        else          ret.push_back(ds.get<double>(command_names.at(idx)));
    }
    return ret;
}

fmi::API::API(const std::string& instance_name_,
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
      command_names(sim.get_command_names())
{
}

fmi::API::API(const std::string& instance_name_,
         const fmiCallbackFunctions& callbacks,
         const bool logging_on_,
         const std::string& yaml,
         const std::string& stl) :
      instance_name(instance_name_),
      logging_on(logging_on_),
      log([callbacks](fmiComponent c,fmiString instanceName,fmiStatus status,fmiString category,fmiString message){callbacks.logger(c, instanceName, status, category, message);}),
      allocate(callbacks.allocateMemory),
      free_memory(callbacks.freeMemory),
      input(SimulatorYamlParser(yaml).parse()),
      sim(get_system(input, stl, 0)),
      t(0),
      command_names(sim.get_command_names())
{
}

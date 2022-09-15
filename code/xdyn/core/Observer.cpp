/*
 * Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */


#include <algorithm>
#include "xdyn/core/Observer.hpp"
#include "exceptions/InvalidInputException.hpp"
#include "xdyn/core/Sim.hpp"
#include "xdyn/core/SurfaceElevationGrid.hpp"

Observer::Observer()
    : requested_serializations()
    , initialized(false)
    , output_everything(true)
    , serializers_called_before_solver_step()
    , serializers_called_after_solver_step()
    , initialize()
{
}

Observer::Observer(const std::vector<std::string>& data_)
    : requested_serializations(data_)
    , initialized(false)
    , output_everything(false)
    , serializers_called_before_solver_step()
    , serializers_called_after_solver_step()
    , initialize()
{
}

std::function<void()> Observer::get_serializer(const SurfaceElevationGrid& , const DataAddressing& )
{
    return [](){};
}

std::function<void()> Observer::get_initializer(const SurfaceElevationGrid& , const DataAddressing& )
{
    return [](){};
}

void Observer::before_write()
{
}

std::vector<std::string> Observer::all_variables(std::map<std::string, std::function<void()> >& map) const
{
    std::vector<std::string> ret;
    std::transform(map.begin(), map.end(), std::back_inserter(ret), [](const std::pair<std::string, std::function<void()> >& p){return p.first;});
    return ret;
}

void Observer::collect_available_serializations(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    write_before_solver_step(t, DataAddressing(std::vector<std::string>(1,"t"), "t"));
    sys.output(sys.state,*this, t, discrete_systems);
}

void Observer::observe_before_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    collect_available_serializations(sys, t, discrete_systems);
    if (output_everything)
    {
        const auto all_vars = all_variables(initialize);
        requested_serializations = all_vars;
        initialize_serialization_of_requested_variables(all_vars);
        serialize_before_solver_step(all_vars);
    }
    else
    {
        initialize_serialization_of_requested_variables(requested_serializations);
        serialize_before_solver_step(requested_serializations);
    }
}

void Observer::observe_after_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    sys.output(sys.state,*this, t, discrete_systems);
    if(output_everything)
    {
        const auto all_vars = all_variables(initialize);
        initialize_serialization_of_requested_variables(all_vars);
        serialize_after_solver_step(all_vars);
    }
    else
    {
        initialize_serialization_of_requested_variables(requested_serializations);
        serialize_after_solver_step(requested_serializations);
    }
}

size_t levenshtein_distance(const std::string& s, const std::string& t);
size_t levenshtein_distance(const std::string& s, const std::string& t)
{
    const size_t n = s.size() + 1;
    const size_t m = t.size() + 1;
    std::vector<size_t> d(n*m, 0);
    for (size_t i = 1, im = 0; i < m; ++i, ++im)
    {
        for (size_t j = 1, jn = 0; j < n; ++j, ++jn)
        {
            if (s[jn] == t[im])
            {
                d[(i * n) + j] = d[((i - 1) * n) + (j - 1)];
            }
            else
            {
                d[(i * n) + j] = std::min(d[(i - 1) * n + j] + 1, // Deletion
                                 std::min(d[i * n + (j - 1)] + 1, // Insertion
                                 d[(i - 1) * n + (j - 1)] + 1));  // Substitution
            }
        }
    }
    return d[n * m - 1];
}

std::string closest_string(const std::string& string_to_match, const std::vector<std::string>& strings_to_test);
std::string closest_string(const std::string& string_to_match, const std::vector<std::string>& strings_to_test)
{
    if (strings_to_test.empty()) return "";
    size_t min_idx = 0;
    size_t min_dist = INT_MAX;
    size_t k = 0;
    for (const auto& s:strings_to_test)
    {
        const size_t levenshtein = levenshtein_distance(string_to_match, s);
        if (levenshtein < min_dist)
        {
            min_idx = k;
            min_dist = levenshtein;
        }
        k++;
    }
    return strings_to_test[min_idx];
}

std::string helpful_error_message_on_missing_requested_serialization(const std::string& missing_variable, const std::vector<std::string>& available_variables);
std::string helpful_error_message_on_missing_requested_serialization(const std::string& missing_variable, const std::vector<std::string>& available_variables)
{
    std::stringstream ss;
    ss << "Available output variables are: ";
    if (!available_variables.empty()) ss << available_variables.front();
    for (size_t i = 1 ; i < available_variables.size() ; ++i)
    {
        ss << ", '" << available_variables[i] << "'";
    }
    if (!available_variables.empty())
    {
        ss << ". Did you mean '" << closest_string(missing_variable, available_variables) << "'?";
    }
    return ss.str();
}

void Observer::initialize_serialization_of_requested_variables(const std::vector<std::string>& variables_to_serialize)
{
    if (not(initialized))
    {
        const size_t n = variables_to_serialize.size();
        size_t i = 0;
        for (const auto& stuff: variables_to_serialize)
        {
            auto initialize_stuff = initialize.find(stuff);
            if (initialize_stuff == initialize.end())
            {
                std::vector<std::string> available_variables;
                for(const auto& s: initialize) available_variables.push_back(s.first);
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "In the 'outputs' section of the YAML file, you asked for '" << stuff << "', but it is not computed: maybe it is misspelt or the corresponding model is not in the YAML. " << helpful_error_message_on_missing_requested_serialization(stuff, available_variables));
            }
            initialize_stuff->second();
            if (i<(n-1)) flush_value_during_initialization();
            ++i;
        }
        flush_after_initialization();
    }
    initialized = true;
}

void Observer::serialize_requested_variables(const std::vector<std::string>& variables_to_serialize, const std::map<std::string, std::function<void()> >& serialize)
{
    const size_t n = variables_to_serialize.size();
    size_t i = 0;
    before_write();
    for (auto variable_name:variables_to_serialize)
    {
        auto serialization_functor = serialize.find(variable_name);
        if (serialization_functor != serialize.end())
        {
            serialization_functor->second();
            if (i<(n-1)) flush_value_during_write();
            ++i;
        }
    }
}

void Observer::check_variables_to_serialize_are_available() const
{
    for (const auto& variable_name:requested_serializations)
    {
        const bool serialized_before_solver_step = serializers_called_before_solver_step.find(variable_name) != serializers_called_before_solver_step.end();
        const bool serialized_after_solver_step = serializers_called_after_solver_step.find(variable_name) != serializers_called_after_solver_step.end();
        if (not(serialized_before_solver_step) && not(serialized_after_solver_step))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, __LINE__ << " In the 'outputs' section of the YAML file, you asked for '" << variable_name << "', but it is not computed: maybe it is misspelt or the corresponding model is not in the YAML.");
        }
    }
}

void Observer::serialize_before_solver_step(const std::vector<std::string>& variables_to_serialize)
{
    serialize_requested_variables(variables_to_serialize, serializers_called_before_solver_step);
}

void Observer::serialize_after_solver_step(const std::vector<std::string>& variables_to_serialize)
{
    serialize_requested_variables(variables_to_serialize, serializers_called_after_solver_step);
}

Observer::~Observer()
{
}

void Observer::flush()
{
    flush_after_write();
}

void Observer::flush_value_during_initialization()
{
    flush_value_during_write();
}

void Observer::write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& , const DataAddressing& )
{
    remove_variable("spectra");
}

void Observer::write_before_simulation(const MeshPtr, const DataAddressing&)
{
    remove_variable("mesh");
}

void Observer::write_before_simulation(const std::string&, const DataAddressing&)
{}

bool Observer::should_serialize(const std::string& variable) const
{
    return std::find(requested_serializations.begin(), requested_serializations.end(), variable) != requested_serializations.end();
}

void Observer::remove_variable(const std::string& variable_to_remove)
{
    requested_serializations.erase(std::remove_if(requested_serializations.begin(), requested_serializations.end(), [variable_to_remove](const std::string& v){return v == variable_to_remove;}), requested_serializations.end());
}

void Observer::write_command_line_before_simulation(const std::string& )
{
    remove_variable("command line");
}

void Observer::write_yaml_before_simulation(const std::string& )
{
    remove_variable("yaml");
}

void Observer::write_matlab_script_before_simulation()
{
    remove_variable("matlab scripts");
}

void Observer::write_python_script_before_simulation()
{
    remove_variable("python scripts");
}

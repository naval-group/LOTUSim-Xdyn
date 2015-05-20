/*
 * FMI.hpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#ifndef FMI_HPP_
#define FMI_HPP_

#include <functional>
#include <string>

#define MODEL_IDENTIFIER Simulator
extern "C"
{
    #include "fmiModelFunctions.h"
    #include "fmiModelTypes.h"
}

#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

class FMI
{
    public:
        FMI(const std::string& instance_name,
            const std::string& GUID,
            const fmiCallbackFunctions& callbacks,
            const bool logging_on,
            const std::string& yaml
            );
        FMI(const std::string& instance_name,
            const fmiCallbackFunctions& callbacks,
            const bool logging_on,
            const std::string& yaml
            );

        void error(const std::string& msg) const;
        void set_time(const double t);
        void set_continuous_states(const std::vector<double>& new_states);
        std::vector<std::string> get_command_names() const;
        void set_real(const std::vector<size_t>& value_references, const std::vector<double>& values);
        std::vector<double> get_derivatives();
        std::vector<double> get_real(const std::vector<size_t>& value_references);
        std::vector<double> get_continuous_states() const;

    private:
        FMI(); // Disabled
        std::string instance_name;
        bool logging_on;
        std::function<void(fmiComponent,fmiString,fmiStatus,fmiString,fmiString)> log;
        std::function<void*(size_t,size_t)> allocate;
        std::function<void(void*)> free_memory;
        YamlSimulatorInput input;
        Sim sim;
        double t;
        std::vector<double> states;
        std::vector<std::string> command_names;
};



#endif  /* FMI_HPP_ */

/*
 * Observer.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <ssc/macros.hpp>
#include <ssc/solver/DiscreteSystem.hpp>
#include TR1INC(memory)

#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "Mesh.hpp"

class Sim;
class SurfaceElevationGrid;

struct DataAddressing
{
    std::string name;
    std::vector<std::string> address;
    DataAddressing():name(),address(){};
    DataAddressing(
            const std::vector<std::string>& address_,
            const std::string& name_):
        name(name_),address(address_){};
};

class Observer
{
    public:
        Observer(); // Outputs everything by default
        Observer(const std::vector<std::string>& data);
        void check_variables_to_serialize_are_available() const;
        virtual void observe_before_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems); // Writes before calling the solver. Cf. solve.hpp Only what was requested by the user in the YAML file
        virtual void observe_after_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems); // Writes after calling the solver. Cf. solve.hpp Only what was requested by the user in the YAML file
        virtual ~Observer();
        void flush();
        // Makes sure the observers know about the variables the system makes available for serialization (so we can run check_variables_to_serialize_are_available solve.hpp)
        void collect_available_serializations(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems);

        /**
         * @brief For variables such as forces & discrete states, which should only be observed after a solver step
         */
        template <typename T> void write_before_solver_step(
                const T& val,
                const DataAddressing& address)
        {
            initialize[address.name] = get_initializer(val, address);
            serializers_called_before_solver_step[address.name] = get_serializer(val, address);
        }

        /**
         * @brief For variables such as continuous states, which should only be observed after a solver step
         */
        template <typename T> void write_after_solver_step(
                const T& val,
                const DataAddressing& address)
        {
            initialize[address.name] = get_initializer(val, address);
            serializers_called_after_solver_step[address.name] = get_serializer(val, address);
        }

        virtual void write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& val, const DataAddressing& address);
        virtual void write_before_simulation(const MeshPtr mesh, const DataAddressing& address);
        virtual void write_before_simulation(const std::string& data, const DataAddressing& address);
        virtual void write_command_line_before_simulation(const std::string& command_line);
        virtual void write_yaml_before_simulation(const std::string& yaml);
        virtual void write_matlab_script_before_simulation();
        virtual void write_python_script_before_simulation();

    protected:

        virtual std::function<void()> get_serializer(const double val, const DataAddressing& address) = 0;
        virtual std::function<void()> get_initializer(const double val, const DataAddressing& address) = 0;

        virtual std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address);
        virtual std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address);

        virtual void flush_after_initialization() = 0;
        virtual void before_write();
        virtual void flush_after_write() = 0;
        virtual void flush_value_during_write() = 0;
        virtual void flush_value_during_initialization();

        void remove_variable(const std::string& variable_to_remove);
        bool should_serialize(const std::string& variable) const;
        std::vector<std::string> requested_serializations;

    protected:
        void initialize_serialization_of_requested_variables(const std::vector<std::string>& variables_to_serialize);
        void serialize_before_solver_step(const std::vector<std::string>& variables_to_serialize);
        void serialize_after_solver_step(const std::vector<std::string>& variables_to_serialize);
        void serialize_requested_variables(const std::vector<std::string>& variables_to_serialize, const std::map<std::string, std::function<void()> >& serialize);
        std::vector<std::string> all_variables(std::map<std::string, std::function<void()> >& map) const;

    private:
        bool initialized;
        bool output_everything;
        std::map<std::string, std::function<void()> > serializers_called_before_solver_step;
        std::map<std::string, std::function<void()> > serializers_called_after_solver_step;

    protected:
        std::map<std::string, std::function<void()> > initialize;
};

typedef TR1(shared_ptr)<Observer> ObserverPtr;

#endif  /* OBSERVER_HPP_ */

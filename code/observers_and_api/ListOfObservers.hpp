/*
 * ListOfObservers.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef LISTOFOBSERVERS_HPP_
#define LISTOFOBSERVERS_HPP_

#include "xdyn/core/Observer.hpp"

struct YamlOutput;

class ListOfObservers
{
    public:
        ListOfObservers(const std::vector<YamlOutput>& yaml);
        ListOfObservers(const std::vector<ObserverPtr>& observers);
        void check_variables_to_serialize_are_available() const;
        void observe_before_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems);
        void observe_after_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems);
        std::vector<ObserverPtr> get() const;
        bool empty() const;
        void flush();

        void add_observer(const ObserverPtr& observer);

        static ObserverPtr parse_observer(const YamlOutput& output);

        template <typename T> void write_before_solver_step(
                const T& val,
                const DataAddressing& address)
        {
            for (auto observer:observers)
            {
                observer->write_before_solver_step(val, address);
            }
        }

        template <typename T> void write_after_solver_step(
                const T& val,
                const DataAddressing& address)
        {
            for (auto observer:observers)
            {
                observer->write_after_solver_step(val, address);
            }
        }

        template <typename T> void write_before_simulation(
                        const T& val,
                        const DataAddressing& address)
        {
            for (auto observer:observers)
            {
                observer->write_before_simulation(val, address);
            }
        }
        // Makes sure the observers know about the variables the system makes available for serialization (so we can run check_variables_to_serialize_are_available solve.hpp)
        void collect_available_serializations(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems);

    private:

        std::vector<ObserverPtr> observers;
};

#endif /* LISTOFOBSERVERS_HPP_ */

/*
 * EverythingObserver.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_
#define OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_

#include "MapObserver.hpp"
#include "Res.hpp"

class EverythingObserver : public MapObserver
{
    public:
        EverythingObserver();
        void observe_before_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems); // Writes before calling the solver. Cf. solve.hpp Only what was requested by the user in the YAML file
        void observe_after_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems); // Writes after calling the solver. Cf. solve.hpp Only what was requested by the user in the YAML file
        std::vector<Res> get() const;

    private:
        std::function<void(Res&, const double)> get_inserter(const std::string& body_name, const std::string& var_name) const;
        void fill_res();
        void add(const std::function<void(Res&, const double)>& inserter, const std::vector<double>& values);
        void observe_everything(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems, const bool before_solver_step);
        std::vector<Res> res;
};



#endif /* OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_ */

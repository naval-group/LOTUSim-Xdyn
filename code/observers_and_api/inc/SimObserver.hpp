/*
 * SimObserver.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SIMOBSERVER_HPP_
#define SIMOBSERVER_HPP_

#include "Res.hpp"
#include <ssc/solver/DiscreteSystem.hpp>
#include <memory>
#include <vector>

class Sim;

/** \author cec
 *  \date Jun 17, 2014, 8:23:16 AM
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/SimObserverTest.cpp SimObserverTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/SimObserverTest.cpp SimObserverTest expected output
 */
class SimObserver
{
    public:
        SimObserver();
        void observe(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems);
        void check_variables_to_serialize_are_available() const;
        std::vector<Res> get() const;
        void flush();
        void collect_available_serializations(const Sim& ss, const double , const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& );

    private:
        std::vector<Res> res;
};

#endif /* SIMOBSERVER_HPP_ */

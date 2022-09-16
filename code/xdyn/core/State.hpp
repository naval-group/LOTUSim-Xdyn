#ifndef CORE_INC_STATE_HPP_
#define CORE_INC_STATE_HPP_

#include "xdyn/core/StateMacros.hpp"
#include "xdyn/external_data_structures/AbstractStates.hpp"
#include "xdyn/hdb_interpolators/History.hpp"

class State : public AbstractStates<History>
{
    public :
        State(const double Tmax=0);
        State(const AbstractStates<History>& h);
        StateType get_StateType(const size_t index) const;
        State(const AbstractStates<double>& vals,
              const double t0);
        State(const StateType& state_values, const double t);

    private :
        State();
};

#endif /* CORE_INC_STATE_HPP_ */

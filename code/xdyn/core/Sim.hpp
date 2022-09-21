/*
 * Sim.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIM_HPP_
#define SIM_HPP_

#include <vector>
#include <ssc/data_source.hpp>
#include <ssc/kinematics.hpp>
#include "xdyn/core/Body.hpp"
#include "xdyn/core/StateMacros.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "xdyn/core/ForceModel.hpp"
#include "xdyn/core/SurfaceElevationGrid.hpp"
#include "xdyn/core/State.hpp"
#include <ssc/solver/ContinuousSystem.hpp>
#include <ssc/solver/DiscreteSystem.hpp>

typedef std::map<std::string, std::map< std::string,ssc::kinematics::Vector6d > > OuputtedForces;
typedef std::vector<std::pair<std::string,std::vector<std::string> > > VectorOfStringModelForEachBody;

class Observer;

class Sim : public ssc::solver::ContinuousSystem
{
    public:
        Sim(const std::vector<BodyPtr>& bodies,
            const std::vector<ListOfForces>& forces,
            const EnvironmentAndFrames& env,
            const StateType& x,
            const ssc::data_source::DataSource& command_listener);
        void dx_dt(const StateType& x, StateType& dxdt, const double t);
        void force_states(StateType& x, const double t) const;

        /**  \brief Serialize wave data on mesh for an ASCII observer
          *  \details Called by SimCsvObserver at each time step. The aim is to
          *  calculate the wave data on a mesh expressed in a particular frame of
          *  reference (eg. NED or body). For example we might want to calculate the
          *  wave data on a mesh surrounding the ship for visualization purposes.
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters)
          *  \snippet simulator/unit_tests/SimTest.cpp SimTest get_waves_example
          */
        ssc::kinematics::PointMatrix get_waves(const double t            //!< Current instant
                                              ) const;

        void output(const StateType& x, Observer& obs, double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems) const;
        ssc::data_source::DataSource& get_command_listener() const;

        void set_bodystates(const State& state_history);

        std::map<std::string,std::vector<ForcePtr> > get_forces() const;
        std::vector<BodyPtr> get_bodies() const;
        EnvironmentAndFrames get_env() const;

        void set_command_listener(const std::map<std::string, double>& new_commands);

        /** \brief Sets the value of one of the system's discrete states. In our case, these discrete states are the command values calculated by the controllers. This method is used by the controllers to store the updated command values in the DataSource, for use by controlled forces (e.g. propellers).
         */
        void set_discrete_state(const std::string &state_name, const double value);
        /** \brief Gets the value of the given input from the datasource
         *
         * Used by controllers to get the inputs they need (setpoints or commands) to compute a command.
         */
        double get_input_value(const std::string &name) const;
        /** \brief Gets the value of a Sim state ("x", "u", "qr", "phi", ...)
         *
         * Used by controllers to get the states they need to compute a command.
         */
        double get_state_value(const std::string &name) const;

        void reset_history();
        std::vector<std::string> get_command_names() const;

        /** \brief This function calls all force models from the current body states.
          * \detail This should be called before the first call to Sim::output if the initial state (at construction) is to be recorded.
          */
        void initialize_system_outputs_before_first_observation();

    private:
        ssc::kinematics::UnsafeWrench sum_of_forces(const StateType& x, const BodyPtr& body, const double t);

        /**  \brief Make sure quaternions can be converted to Euler angles
          *  \details Normalization takes place at each time step, which is not
          *  ideal because it means the model does not see the state values set
          *  by the stepper.
          */
        StateType normalize_quaternions(const StateType& all_states
                                       ) const;

        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* SIM_HPP_ */

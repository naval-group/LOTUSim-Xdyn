/*
 * GrpcController.hpp
 *
 *  Created on: Apr, 19 2021
 *      Author: cady
 */

#ifndef GRPCCONTROLLER_HPP_
#define GRPCCONTROLLER_HPP_

#include "Controller.hpp"
#include "GrpcControllerInterface.hpp"
class GrpcController : public Controller
{
  public:
    GrpcController (const double tstart, const double dt,
                    const std::string &yaml);

  private:
    /**
     * @brief Updates the controller output value in the datasource.
     *
     * This method will be called by the
     * "ssc::solver::DiscreteSystem::callback" method. It is the only method we
     * must override from the Controller class.
     * @param time Current simulation time (in seconds).
     * @param system The continuous system. Used to retrieve the continuous
     * states.
     */
    void update_discrete_states (const double time,
                                 ssc::solver::ContinuousSystem *system);
};

#endif /* GRPCCONTROLLER_HPP_ */

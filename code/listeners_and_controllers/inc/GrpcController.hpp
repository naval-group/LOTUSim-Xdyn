/*
 * GrpcController.hpp
 *
 *  Created on: Apr, 19 2021
 *      Author: cady
 */

#ifndef GRPCCONTROLLER_HPP_
#define GRPCCONTROLLER_HPP_

#include "Controller.hpp"

class GrpcController : public Controller
{
  public:
    struct Input
    {
        Input ();
        std::string url;
        std::string name;
        std::string yaml;
    };
    static Input parse (const std::string &yaml);

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

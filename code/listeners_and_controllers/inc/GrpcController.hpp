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
    static GrpcController* build(const double tstart, const std::string& yaml);
    std::vector<std::string> get_command_names() const;

  private:
    GrpcController() = delete;
    GrpcController (const double tstart, const std::shared_ptr<GrpcControllerInterface>& grpc);
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

    std::shared_ptr<GrpcControllerInterface> grpc;
};

#endif /* GRPCCONTROLLER_HPP_ */

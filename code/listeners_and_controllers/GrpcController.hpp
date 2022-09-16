/*
 * GrpcController.hpp
 *
 *  Created on: Apr, 19 2021
 *      Author: cady
 */

#ifndef GRPCCONTROLLER_HPP_
#define GRPCCONTROLLER_HPP_

#include "Controller.hpp"
#include "xdyn/grpc/GrpcControllerInterface.hpp"
class GrpcController : public Controller
{
  public:
    static GrpcController *build (const double tstart,
                                  const std::string& name,
                                  const std::string &yaml, ssc::solver::ContinuousSystem& sys);
    std::vector<std::string> get_command_names () const;

  private:
    GrpcController () = delete;
    GrpcController (const double tstart,
                    const std::string& name,
                    const std::shared_ptr<GrpcControllerInterface> &grpc);
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
                                 ssc::solver::ContinuousSystem& system);
    void add_controller_outputs_to_data_source (
        const double time, ssc::solver::ContinuousSystem& sys,
        const std::vector<double> &setpoints);

    std::shared_ptr<GrpcControllerInterface> grpc;
    std::string name;
};

#endif /* GRPCCONTROLLER_HPP_ */

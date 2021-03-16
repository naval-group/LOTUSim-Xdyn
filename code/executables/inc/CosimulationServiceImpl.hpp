/*
 * XdynForCSgrpc.hpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_XDYNFORCSGRPC_HPP_
#define EXECUTABLES_INC_XDYNFORCSGRPC_HPP_

#include <grpcpp/grpcpp.h>
#include "cosimulation.grpc.pb.h"
#include "cosimulation.pb.h"
#include "XdynForCS.hpp"
#include "ErrorOutputter.hpp"

template <typename Request> grpc::Status check_states_size(ErrorOutputter& error, const Request* request);
template <> grpc::Status check_states_size<CosimulationRequestEuler>(ErrorOutputter& error, const CosimulationRequestEuler* request);
template <> grpc::Status check_states_size<CosimulationRequestQuaternion>(ErrorOutputter& error, const CosimulationRequestQuaternion* request);

class CosimulationServiceImpl final : public Cosimulation::Service {
    public:
        explicit CosimulationServiceImpl(const XdynForCS& simserver);
        grpc::Status step_quaternion(grpc::ServerContext* context, const CosimulationRequestQuaternion* request, CosimulationResponse* response) override;
        grpc::Status step_euler_321(grpc::ServerContext* context, const CosimulationRequestEuler* request, CosimulationResponse* response) override;

    private:
        XdynForCS simserver;
        ErrorOutputter error;
};

#endif /* EXECUTABLES_INC_XDYNFORCSGRPC_HPP_ */

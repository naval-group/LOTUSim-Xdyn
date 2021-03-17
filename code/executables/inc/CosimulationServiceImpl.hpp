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
#include "gRPCChecks.hpp"
#include "report_xdyn_exceptions_to_user.hpp"

template <typename Request> grpc::Status check_states_size(ErrorOutputter& error, const Request* request);
template <> grpc::Status check_states_size<CosimulationRequestEuler>(ErrorOutputter& error, const CosimulationRequestEuler* request);
template <> grpc::Status check_states_size<CosimulationRequestQuaternion>(ErrorOutputter& error, const CosimulationRequestQuaternion* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestEuler* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestQuaternion* request);
grpc::Status to_grpc(grpc::ServerContext* context, const std::vector<YamlState>& res, CosimulationResponse* response);

class CosimulationServiceImpl final : public Cosimulation::Service {
    public:
        explicit CosimulationServiceImpl(const XdynForCS& simserver);
        grpc::Status step_quaternion(grpc::ServerContext* context, const CosimulationRequestQuaternion* request, CosimulationResponse* response) override;
        grpc::Status step_euler_321(grpc::ServerContext* context, const CosimulationRequestEuler* request, CosimulationResponse* response) override;

    private:
        template <typename Request> grpc::Status step(
                grpc::ServerContext* context,
                const Request* request,
                CosimulationResponse* response)
        {
            const grpc::Status precond = check_states_size(error, request);
            if (not precond.ok())
            {
                return precond;
            }
            std::vector<YamlState> output;
            const std::function<void()> f = [&context, this, &request, &output]()
                {
                    const YamlSimServerInputs inputs = from_grpc(context, request);
                    output = simserver.handle(inputs);
                };
            grpc::Status run_status(grpc::Status::OK);
            const std::function<void(const std::string&)> error_outputter = [this](const std::string& error_message)
                {
                    this->error.simulation_error(error_message);
                };
            report_xdyn_exceptions_to_user(f, error_outputter);
            if (error.contains_errors())
            {
                return to_gRPC_status(error);
            }
            return to_grpc(context, output, response);
        }
        XdynForCS simserver;
        ErrorOutputter error;
};

#endif /* EXECUTABLES_INC_XDYNFORCSGRPC_HPP_ */

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
#include <memory>

template <typename Request> grpc::Status check_states_size(ErrorOutputter& error, const Request* request);
template <> grpc::Status check_states_size<CosimulationRequestEuler>(ErrorOutputter& error, const CosimulationRequestEuler* request);
template <> grpc::Status check_states_size<CosimulationRequestQuaternion>(ErrorOutputter& error, const CosimulationRequestQuaternion* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestEuler* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestQuaternion* request);
grpc::Status to_grpc(grpc::ServerContext* context, const std::vector<YamlState>& res, CosimulationResponse* response);

class CosimulationServiceImpl final : public Cosimulation::Service {
    public:
        explicit CosimulationServiceImpl(const XdynForCS& simserver, std::shared_ptr<ErrorOutputter>& outputter);
        grpc::Status step_quaternion(grpc::ServerContext* context, const CosimulationRequestQuaternion* request, CosimulationResponse* response) override;
        grpc::Status step_euler_321(grpc::ServerContext* context, const CosimulationRequestEuler* request, CosimulationResponse* response) override;

    private:
        template <typename Request> grpc::Status step(
                grpc::ServerContext* context,
                const Request* request,
                CosimulationResponse* response)
        {
            const grpc::Status precond = check_states_size(*error_outputter, request);
            if (not precond.ok())
            {
                return precond;
            }
            std::vector<YamlState> output;
            grpc::Status run_status;
            const std::function<void()> f = [&context, this, &request, &output, &run_status, &response]()
                {
                    const YamlSimServerInputs inputs = from_grpc(context, request);
                    output = simserver.handle(inputs);
                    run_status = to_grpc(context, output, response);
                };
            error_outputter->run_and_report_errors(f);
            return to_gRPC_status(*error_outputter);
        }
        XdynForCS simserver;
        std::shared_ptr<ErrorOutputter> error_outputter;
};

#endif /* EXECUTABLES_INC_XDYNFORCSGRPC_HPP_ */

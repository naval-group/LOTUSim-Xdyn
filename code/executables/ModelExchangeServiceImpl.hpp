/*
 * ModelExchangeServiceImpl.hpp
 *
 *  Created on: Oct 22, 2020
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_
#define EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_

#include "xdyn/external_data_structures/YamlSimServerInputs.hpp"
#include "model_exchange.grpc.pb.h"
#include "model_exchange.pb.h"
#include "XdynForME.hpp"
#include "ErrorReporter.hpp"
#include "gRPCChecks.hpp"
#include <grpcpp/grpcpp.h>


YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestEuler* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestQuaternion* request);
grpc::Status to_grpc(grpc::ServerContext* context, const YamlState& state_derivatives, ModelExchangeResponse* response);
template <typename Request> void check_states_size(const Request* request);
template <> void check_states_size<ModelExchangeRequestEuler>(const ModelExchangeRequestEuler* request);
template <> void check_states_size<ModelExchangeRequestQuaternion>(const ModelExchangeRequestQuaternion* request);

class ModelExchangeServiceImpl final : public ModelExchange::Service {
    public:
        explicit ModelExchangeServiceImpl(const XdynForME& xdyn, ErrorReporter& error_outputter);
        grpc::Status dx_dt_quaternion(grpc::ServerContext* context, const ModelExchangeRequestQuaternion* request, ModelExchangeResponse* response) override;
        grpc::Status dx_dt_euler_321(grpc::ServerContext* context, const ModelExchangeRequestEuler* request, ModelExchangeResponse* response) override;

    private:
        template <typename Request> grpc::Status dx_dt(
                grpc::ServerContext* context,
                const Request* request,
                ModelExchangeResponse* response)
        {
            YamlState output;
            grpc::Status run_status;
            const std::function<void()> f = [&context, this, &request, &output, &response, &run_status]()
                {
                    check_states_size(request);
                    const YamlSimServerInputs inputs = from_grpc(context, request);
                    output = simserver.handle(inputs);
                    run_status = to_grpc(context, output, response);
                };
            error_outputter.run_and_report_errors_without_yaml_dump(f);
            return to_gRPC_status(error_outputter);
        }
        XdynForME simserver;
        ErrorReporter& error_outputter;
};

#endif /* EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_ */

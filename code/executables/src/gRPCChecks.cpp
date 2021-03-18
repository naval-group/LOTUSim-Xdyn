#include "gRPCChecks.hpp"

grpc::Status to_gRPC_status(const ErrorReporter& error_outputter)
{
    // Cf. https://gitlab.com/sirehna_naval_group/xdyn/-/merge_requests/10#note_525827152
    switch (error_outputter.get_status())
    {
        case ErrorReporter::Status::OK:
            return grpc::Status::OK;
            break;
        case ErrorReporter::Status::INVALID_INPUT:
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,error_outputter.get_message());
        case ErrorReporter::Status::SIMULATION_ERROR:
            return grpc::Status(grpc::StatusCode::ABORTED, error_outputter.get_message());
        case ErrorReporter::Status::INTERNAL_ERROR:
            return grpc::Status(grpc::StatusCode::ABORTED, error_outputter.get_message());
        case ErrorReporter::Status::NETWORK_ERROR:
            return grpc::Status(grpc::StatusCode::ABORTED, error_outputter.get_message());
    }
    return grpc::Status(grpc::StatusCode::UNKNOWN, std::string("Reached the end of a switch case in ") + std::string(__PRETTY_FUNCTION__));
}
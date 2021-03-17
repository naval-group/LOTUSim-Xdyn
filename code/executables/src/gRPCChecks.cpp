#include "gRPCChecks.hpp"

grpc::Status to_gRPC_status(const ErrorOutputter& error)
{
    // Cf. https://gitlab.com/sirehna_naval_group/xdyn/-/merge_requests/10#note_525827152
    switch (error.get_status())
    {
        case ErrorOutputter::Status::OK:
            return grpc::Status::OK;
            break;
        case ErrorOutputter::Status::INVALID_INPUT:
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, error.get_message());
        case ErrorOutputter::Status::SIMULATION_ERROR:
            return grpc::Status(grpc::StatusCode::ABORTED, error.get_message());
    }
    return grpc::Status(grpc::StatusCode::UNKNOWN, std::string("Reached the end of a switch case in ") + std::string(__PRETTY_FUNCTION__));
}
#ifndef GRPCERROROUTPUTTERHPP
#define GRPCERROROUTPUTTERHPP

#include <string>
#include <grpcpp/grpcpp.h>
#include "GRPCError.hpp"

template <typename ServiceType> std::string get_type_of_service();

template <typename Input, typename ServiceType> void throw_if_invalid_status(const Input& input, const std::string& rpc_method, const grpc::Status& status)
{
    const std::string base_error_msg = std::string("an error occurred when using the distant ") + get_type_of_service<ServiceType>() + " '" + input.name + "' defined via gRPC (method '" + rpc_method + "').\n";
    const std::string yaml_msg = "The YAML you provided for this gRPC model is:\n\n" + input.yaml + "\n";
    const std::string contact_team_msg = "support team with the following error code (cf. https://grpc.github.io/grpc/cpp/grpc_2impl_2codegen_2status_8h.html): ";
    const std::string user_error_msg = "Check that the server is running and accessible from the URL defined in the YAML file: " + input.url;
    const std::string dev_error_msg = "This is a problem with xdyn and should never happen: please contact xdyn's " + contact_team_msg;
    const std::string network_error_msg = std::string("We couldn't reach the gRPC ") + get_type_of_service<ServiceType>() + " server at this URL: " + input.url + " Maybe the server is temporarily inaccessible or hasn't started yet? Either wait and give it another try or change the 'url' key in the YAML file. If all else fails, contact xdyn's " + contact_team_msg;
    const std::string server_error_msg = std::string("The gRPC ") + get_type_of_service<ServiceType>() + " server responded with the following error code: " + status.error_message()  + "\nMaybe the parameters you defined in xdyn's YAML file are incorrect? " + yaml_msg + "\nPlease check the " + get_type_of_service<ServiceType>() + " server's documentation! If the problem persists, try contacting the " + get_type_of_service<ServiceType>() + " server's " + contact_team_msg;
    const std::string authentication_error_msg = std::string("This ") + get_type_of_service<ServiceType>() + " server requires authentication and xdyn does not support this yet.";
    const std::string unimplemented = std::string("This ") + get_type_of_service<ServiceType>() + " server does not implement gRPC method '" + rpc_method + "': implement it or use another model. Otherwise, contact the " + get_type_of_service<ServiceType>() + "'s " + contact_team_msg;
    switch(status.error_code())
    {
        case grpc::StatusCode::OK:
        {
            return;
            break;
        }
        case grpc::StatusCode::ABORTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "ABORTED");
            break;
        }
        case grpc::StatusCode::ALREADY_EXISTS:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "ALREADY_EXISTS");
            break;
        }
        case grpc::StatusCode::CANCELLED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "CANCELLED");
            break;
        }
        case grpc::StatusCode::DATA_LOSS:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + network_error_msg + "DATA_LOSS");
            break;
        }
        case grpc::StatusCode::DEADLINE_EXCEEDED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + network_error_msg + "DEADLINE_EXCEEDED");
            break;
        }
        case grpc::StatusCode::DO_NOT_USE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "DO_NOT_USE");
            break;
        }
        case grpc::StatusCode::FAILED_PRECONDITION:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "FAILED_PRECONDITION");
            break;
        }
        case grpc::StatusCode::INTERNAL:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "INTERNAL");
            break;
        }
        case grpc::StatusCode::INVALID_ARGUMENT:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "INVALID_ARGUMENT");
            break;
        }
        case grpc::StatusCode::NOT_FOUND:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "NOT_FOUND");
            break;
        }
        case grpc::StatusCode::OUT_OF_RANGE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "OUT_OF_RANGE");
            break;
        }
        case grpc::StatusCode::PERMISSION_DENIED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + authentication_error_msg);
            break;
        }
        case grpc::StatusCode::RESOURCE_EXHAUSTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "RESOURCE_EXHAUSTED");
            break;
        }
        case grpc::StatusCode::UNAUTHENTICATED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + authentication_error_msg);
            break;
        }
        case grpc::StatusCode::UNAVAILABLE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg  + network_error_msg + "UNAVAILABLE");
            break;
        }
        case grpc::StatusCode::UNIMPLEMENTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + unimplemented + "UNIMPLEMENTED");
            break;
        }
        case grpc::StatusCode::UNKNOWN:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "UNKNOWN");
            break;
        }
        default:
        {
            return; // Do nothing: should never happen
            break;
        }
    }
}

#endif
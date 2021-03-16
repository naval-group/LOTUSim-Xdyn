#include "ErrorOutputter.hpp"


ErrorOutputter::ErrorOutputter() : ss(), status(Status::OK)
{}

void ErrorOutputter::invalid_request(const std::string &function, const int line)
{
    ss << "'request' is a NULL pointer in "
       << function
       << ", line "
       << line
       << ": this is an implementation error in xdyn. You should contact xdyn's support team."
       << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::empty_history()
{
    ss << "The state history provided to xdyn is empty ('t' has size 0): we need at least one value for each state to set the initial conditions.";
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::invalid_state_size(const std::string& state_name, const size_t state_size, const size_t t_size)
{
    ss << "State '"
       << state_name
       << "' has size "
       << state_size
       << ", whereas 't' has size "
       << t_size
       << ": this is a problem in the client code (caller of xdyn's gRPC server), not a problem with xdyn. Please ensure that '"
       << state_name
       << "' and 't' have the same size in the request's 'States' type."
       << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::simulation_error(const std::string& error_message)
{
    status = Status::SIMULATION_ERROR;
    ss << error_message;
}

bool ErrorOutputter::contains_errors() const
{
    return status != Status::OK;
}

std::string ErrorOutputter::get_message() const
{
    return ss.str();
}

grpc::Status ErrorOutputter::get_grpc_status() const
{
    // Cf. https://gitlab.com/sirehna_naval_group/xdyn/-/merge_requests/10#note_525827152
    switch (status)
    {
        case Status::OK:
            return grpc::Status::OK;
            break;
        case Status::INVALID_INPUT:
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, get_message());
        case Status::SIMULATION_ERROR:
            return grpc::Status(grpc::StatusCode::ABORTED, get_message());
    }
    return grpc::Status(grpc::StatusCode::UNKNOWN, "Reached the end of a switch case in ErrorOutputter::get_grpc_status()");
}
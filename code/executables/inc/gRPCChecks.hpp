#ifndef GRPCSIZECHECKERSHPP
#define GRPCSIZECHECKERSHPP

#include <grpcpp/grpcpp.h>

#include "InternalErrorException.hpp"
#include "InvalidInputException.hpp"
#include "ErrorReporter.hpp"

grpc::Status to_gRPC_status(const ErrorReporter& error_outputter);


#define SIZE size()
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define STATE_SIZE(state) EVALUATOR(state, SIZE)
#define CHECK_SIZE(state) if (states.STATE_SIZE(state) != states.t_size())\
{\
    const size_t n1 = states.STATE_SIZE(state);\
    const size_t n2 = states.t_size();\
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "State '"\
                                                      << #state\
                                                      << "' has size "\
                                                      << n1\
                                                      << ", whereas 't' has size "\
                                                      << n2\
                                                      << ": this is a problem in the client code (caller of xdyn), not a problem with xdyn. Please ensure that '"\
                                                      << #state\
                                                      << "' and 't' have the same size in the request's 'States' type."\
                                                      << std::endl);\
}

template <typename Request> void check_euler_states_size(const Request* request)
{
    if (!request)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "'request' is a NULL pointer.");
    }
    else
    {
        const auto states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(phi);
        CHECK_SIZE(theta);
        CHECK_SIZE(psi);
        if (states.t_size() == 0)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "The state history provided to xdyn is empty ('t' has size 0): we need at least one value for each state to set the initial conditions.");
        }
    }
}

template <typename Request> void check_quaternion_states_size(const Request* request)
{
    if (!request)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "'request' is a NULL pointer.");
    }
    else
    {
        const auto states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(qr);
        CHECK_SIZE(qi);
        CHECK_SIZE(qj);
        CHECK_SIZE(qk);
        if (states.t_size() == 0)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "The state history provided to xdyn is empty ('t' has size 0): we need at least one value for each state to set the initial conditions.");
        }
    }
}

#endif
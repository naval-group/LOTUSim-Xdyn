#ifndef ERROROUTPUTTER_HPP
#define ERROROUTPUTTER_HPP

#include <sstream>
#include <string>

#include <grpcpp/grpcpp.h>

class ErrorOutputter
{
    public:
        ErrorOutputter();
        void invalid_request(const std::string &function, const int line);
        void empty_history();
        void invalid_state_size(const std::string& state_name, const size_t state_size, const size_t t_size);
        void simulation_error(const std::string& error_message);

        bool contains_errors() const;
        std::string get_message() const;
        grpc::Status get_grpc_status() const;

      private:
          std::stringstream ss;
          enum class Status
          {
              OK,
              INVALID_INPUT,
              SIMULATION_ERROR
          };
          Status status;

};


#define SIZE size()
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define STATE_SIZE(state) EVALUATOR(state, SIZE)
#define CHECK_SIZE(err, state) if (states.STATE_SIZE(state) != states.t_size())\
{\
    const size_t n1 = states.STATE_SIZE(state);\
    const size_t n2 = states.t_size();\
    err.invalid_state_size(#state, n1, n2);\
}

template <typename Request> grpc::Status check_euler_states_size(ErrorOutputter& error, const Request* request)
{
    if (!request)
    {
        error.invalid_request(__PRETTY_FUNCTION__, __LINE__);
    }
    else
    {
        const auto states = request->states();
        CHECK_SIZE(error, x);
        CHECK_SIZE(error, y);
        CHECK_SIZE(error, z);
        CHECK_SIZE(error, u);
        CHECK_SIZE(error, v);
        CHECK_SIZE(error, w);
        CHECK_SIZE(error, p);
        CHECK_SIZE(error, q);
        CHECK_SIZE(error, r);
        CHECK_SIZE(error, phi);
        CHECK_SIZE(error, theta);
        CHECK_SIZE(error, psi);
        if (states.t_size() == 0)
        {
            error.empty_history();
        }
    }
    return error.get_grpc_status();
}

template <typename Request> grpc::Status check_quaternion_states_size(ErrorOutputter& error, const Request* request)
{
    if (!request)
    {
        error.invalid_request(__PRETTY_FUNCTION__, __LINE__);
    }
    else
    {
        const auto states = request->states();
        CHECK_SIZE(error, x);
        CHECK_SIZE(error, y);
        CHECK_SIZE(error, z);
        CHECK_SIZE(error, u);
        CHECK_SIZE(error, v);
        CHECK_SIZE(error, w);
        CHECK_SIZE(error, p);
        CHECK_SIZE(error, q);
        CHECK_SIZE(error, r);
        CHECK_SIZE(error, qr);
        CHECK_SIZE(error, qi);
        CHECK_SIZE(error, qj);
        CHECK_SIZE(error, qk);
        if (states.t_size() == 0)
        {
            error.empty_history();
        }
    }
    return error.get_grpc_status();
}

#endif
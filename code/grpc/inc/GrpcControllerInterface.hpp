#ifndef GRPCCONTROLLERINTERFACEHPP
#define GRPCCONTROLLERINTERFACEHPP

#include "BodyStates.hpp"
#include "EnvironmentAndFrames.hpp"

enum class AngleRepresentation
{
    QUATERNION,    EULER_321
};

struct GrpcSetParametersResponse
{
    GrpcSetParametersResponse();
    double                   date_of_first_callback; // Date at which the controller should be called for the first time. Will often be equal to just t0.
    std::vector<std::string> setpoint_names;         // Name of the controller inputs (setpoints) which xdyn must supply.
    AngleRepresentation      angle_representation;   // Does the controller need to be called with get_commands_quaternion or with get_commands_euler_321?
    bool                     has_extra_observations; // If set to true, the controller's get_extra_observations will be called.
};

struct GrpcControllerResponse
{
    GrpcControllerResponse();
    std::map<std::string,double> commands; // Commands computed by the controller
    double next_call; // Date at which the solver should call the controller again
};


class GrpcControllerInterface
{
    public:
        virtual ~GrpcControllerInterface();
        struct Input
        {
            Input ();
            std::string url;  //!< URL at which the gRPC controller may be reached,
                              //!< e.g. pid:9002
            std::string name; //!< Name used to disambiguate commands created by
                              //!< the controller
            std::string yaml; //!< The whole YAML node as a string, passed to the
                              //!< controller's set_parameters gRPC method
        };
        double get_date_of_first_callback() const;

        static std::shared_ptr<GrpcControllerInterface> build(const Input& input, const double t0);
        static Input parse (const std::string &yaml);
        virtual GrpcControllerResponse get_commands(const double t, const BodyStates& states, const BodyStates& dstates_dt, const std::vector<double>& setpoints, const EnvironmentAndFrames& env) = 0;
        std::map<std::string, double> get_extra_observations();
        bool has_extra_observations() const;
        std::vector<std::string> get_setpoint_names() const;

    protected:
        struct Impl;
        std::shared_ptr<Impl> pimpl;
        GrpcControllerInterface(const std::shared_ptr<Impl>& pimpl, const GrpcSetParametersResponse& set_parameters_response);

    private:
        GrpcControllerInterface() = delete;
        GrpcSetParametersResponse set_parameters_response;

};

class Euler321GrpcController : public GrpcControllerInterface
{
    public:
        Euler321GrpcController(const std::shared_ptr<Impl>& pimpl, const GrpcSetParametersResponse& set_parameters_response);
        GrpcControllerResponse get_commands(const double t, const BodyStates& states, const BodyStates& dstates_dt, const std::vector<double>& setpoints, const EnvironmentAndFrames& env);
    private:
        Euler321GrpcController() = delete;

};

class QuaternionGrpcController : public GrpcControllerInterface
{
    public:
        QuaternionGrpcController(const std::shared_ptr<Impl>& pimpl, const GrpcSetParametersResponse& set_parameters_response);
        GrpcControllerResponse get_commands(const double t, const BodyStates& states, const BodyStates& dstates_dt, const std::vector<double>& setpoints, const EnvironmentAndFrames& env);
    private:
        QuaternionGrpcController() = delete;
};


#endif
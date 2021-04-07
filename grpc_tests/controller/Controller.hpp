#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <map>
#include <string>
#include <vector>

enum class AngleRepresentation_
{
    QUATERNION,
    EULER_321
};

struct SetParametersResponse_
{
    std::vector<std::string> setpoint_names; // Name of the controller inputs (setpoints) which xdyn must supply.
    bool has_extra_observations; // If set to true, the controller's get_extra_observations will be called.
};

struct ControllerResponse_
{
    // Commands computed by the controller
    std::map<std::string,double> commands;
    // Date at which the solver should call the controller again
    double next_call;
};

struct EulerStates_
{
    double t;  // Simulation time (in seconds).
    double x;  // Projection on axis X of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double y;  // Projection on axis Y of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double z;  // Projection on axis Z of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double u;  // Projection on axis X of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double v;  // Projection on axis Y of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double w;  // Projection on axis Z of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double p;  // Projection on axis X of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double q;  // Projection on axis Y of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double r; // Projection on axis Z of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double phi; // First Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)
    double theta; // Second Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)
    double psi; // Third Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)

};

struct QuaternionStates_
{
    double t;  // Simulation time (in seconds).
    double x;  // Projection on axis X of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double y;  // Projection on axis Y of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double z;  // Projection on axis Z of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double u;  // Projection on axis X of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double v;  // Projection on axis Y of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double w;  // Projection on axis Z of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double p;  // Projection on axis X of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double q;  // Projection on axis Y of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double r; // Projection on axis Z of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double qr; // Real part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qi; // First imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qj; // Second imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qk; // Third imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
};

class Controller_
{
    public:
        Controller_() = delete;
        Controller_(const double dt);
        // Initialize the controller with YAML parameters
        virtual SetParametersResponse_ set_parameters(const std::string& yaml) = 0;
        // Calculate the commands using quaternions instead of angles
        virtual std::map<std::string,double> get_commands_quaternion(const QuaternionStates_& states, const QuaternionStates_& dstates_dt, const std::vector<double>& setpoints) = 0;
        // Calculate the commands using angles in Rned2body = Rz(ψ).Ry(θ).Rx(ϕ) rotation convention
        virtual std::map<std::string,double> get_commands_euler_321(const EulerStates_& states, const EulerStates_& dstates_dt, const std::vector<double>& setpoints) = 0;
        // Return the extra observations this controller may give
        virtual std::map<std::string, double> get_extra_observations() const;
        virtual AngleRepresentation_ get_angle_representation() const = 0;
        void set_t0(const double t0);
        virtual double get_date_of_next_call(const double t) const;
        double get_dt() const;
        double get_t0() const;
    private:
        double dt;
        double t0;
};

class EulerController_ : public Controller_
{
    public:
        EulerController_() = delete;
        EulerController_(const double dt);
        std::map<std::string,double> get_commands_quaternion(const QuaternionStates_& states, const QuaternionStates_& dstates_dt, const std::vector<double>& setpoints);
        AngleRepresentation_ get_angle_representation() const;

};

class QuaternionController_ : public Controller_
{
    public:
        QuaternionController_() = delete;
        QuaternionController_(const double dt);
        std::map<std::string,double> get_commands_euler_321(const EulerStates_& states, const EulerStates_& dstates_dt, const std::vector<double>& setpoints);
        AngleRepresentation_ get_angle_representation() const;
};

void run(Controller_& controller);

#endif

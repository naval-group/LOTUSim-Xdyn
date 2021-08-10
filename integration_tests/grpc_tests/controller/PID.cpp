#include "Controller.hpp"
#include <iostream>

#define QUOTE_ME(x) #x
#define QUOTE(x) QUOTE_ME(x)
#define COUT(val) std::cout << "in file " << __FILE__ << ":" << __LINE__ << " " << #val << " = " << val << std::endl;

class PID : public EulerController_
{
    public:
        PID(const double dt_) : EulerController_(dt_), Kp(2.5), Ki(0.1), Kd(0.314), can_use_integrator_and_derivative(false), previous_error(0), integral(0)
        {
        }

        SetParametersResponse_ set_parameters(const std::string&)
        {
            SetParametersResponse_ ret;
            ret.setpoint_names = {"beta"};
            return ret;
        }

        std::map<std::string,double> get_commands_euler_321(const EulerStates_& states, const EulerStates_& , const std::vector<double>& setpoints)
        {
            const double psi_co = setpoints.front();
            const double error = psi_co - (states.x - states.y);
            // Proportional term
            const double proportional_term = Kp * error;

            double integral_term = 0;
            double derivative_term = 0;
            if (can_use_integrator_and_derivative)
            {
                // Integral term
                integral += error * get_dt();
                integral_term = Ki * integral;

                // Derivative term
                derivative_term = Kd * (error - previous_error) / get_dt();
            }

            // Store error for next time step
            previous_error = error;

            // As integrator and derivative have been initialized, we can now use them
            can_use_integrator_and_derivative = true;
            std::map<std::string,double> ret;
            ret["beta"] = proportional_term + integral_term + derivative_term;
            return ret;
        }
    private:
        double Kp;
        double Ki;
        double Kd;
        bool can_use_integrator_and_derivative;
        double previous_error;
        double integral;
};

int main(int , char** )
{
    PID pid(0.5);
    run(pid);
}

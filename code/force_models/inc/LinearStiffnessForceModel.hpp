#ifndef LINEARSTIFFNESSFORCEMODEL_HPP_
#define LINEARSTIFFNESSFORCEMODEL_HPP_

#include <Eigen/Dense>
#include "ForceModel.hpp"

class LinearStiffnessForceModel : public ForceModel
{
    public:
        struct Input
        {
            std::string name;
            Eigen::Matrix<double,6,6> K;
        };
        
        LinearStiffnessForceModel(const Input input, const std::string& body_name, const EnvironmentAndFrames& env);
        LinearStiffnessForceModel() = delete;
        static Input parse(const std::string& yaml);
        static std::string model_name();
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;

    private:
        Eigen::Matrix<double,6,6> K; //!< 6x6 linear stiffness matrix expressed in the body frame
};

#endif /* LINEARSTIFFNESSFORCEMODEL_HPP_ */
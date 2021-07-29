#ifndef FORCE_MODELS_INC_MMGMANOEUVRINGFORCEMODEL_HPP_
#define FORCE_MODELS_INC_MMGMANOEUVRINGFORCEMODEL_HPP_

#include "YamlCoordinates.hpp"
#include "external_data_structures_parsers.hpp"

#include "ForceModel.hpp"

class MMGManeuveringForceModel : public ForceModel
{
public:
    struct Input
    {
        Input();
        YamlCoordinates application_point;
        double Lpp;
        double T;
        double Xvv;
        double Xrr;
        double Xvr;
        double Xvvvv;
        double Yv;
        double Yr;
        double Yvvv;
        double Yrvv;
        double Yvrr;
        double Yrrr;
        double Nv;
        double Nr;
        double Nvvv;
        double Nrvv;
        double Nvrr;
        double Nrrr;
    };
    MMGManeuveringForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
    MMGManeuveringForceModel() = delete;
    Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;
    static Input parse(const std::string& yaml);
    static std::string model_name();

private:
    double Lpp;
    double T;
    double Xvv;
    double Xrr;
    double Xvr;
    double Xvvvv;
    double Yv;
    double Yr;
    double Yvvv;
    double Yrvv;
    double Yvrr;
    double Yrrr;
    double Nv;
    double Nr;
    double Nvvv;
    double Nrvv;
    double Nvrr;
    double Nrrr;

};

#endif /* FORCE_MODELS_INC_MMGMANOEUVRINGFORCEMODEL_HPP_ */

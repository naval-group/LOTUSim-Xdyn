#ifndef FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_
#define FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_

#include <functional>
#include <boost/optional.hpp>
#include <ssc/kinematics.hpp>

#include "ForceModel.hpp"

class HoltropMennenForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            double Lwl;
            double Lpp;
            double B;
            double Ta;
            double Tf;
            double Vol;
            double lcb;
            boost::optional<double> S;
            double Abt;
            double hb;
            double Cm;
            double Cwp;
            boost::optional<double> iE; // WARNING: iE must be in degrees
            double At;
            double Sapp;
            double Cstern;
            boost::optional<double> hull_form_coeff;
            double app_form_coeff;
            bool apply_on_ship_speed_direction;
        };
        HoltropMennenForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const override;
        static Input parse(const std::string& yaml);
        static std::string model_name();

        double get_Rf(const BodyStates& states) const;   // Frictional resistance over the hull
        double get_Rapp(const BodyStates& states) const; // Frictional resistance over the appendages
        double get_Rw(const BodyStates& states) const;   // Wave-making resistance
        double get_Rb(const BodyStates& states) const;   // Bulbous bow influence
        double get_Rtr(const BodyStates& states) const;  // Immersed transom stern influence
        double get_Ra(const BodyStates& states) const;   // Correlation term between model and full scale

        class Impl;

    private:
        const bool apply_on_ship_speed_direction;
        const TR1(shared_ptr)<Impl> pimpl;
};

#endif /* FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_ */

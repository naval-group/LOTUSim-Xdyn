#ifndef FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_
#define FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_

#include <functional>
#include <boost/optional.hpp>
#include <ssc/kinematics.hpp>

#include "ControllableForceModel.hpp"
#include "EnvironmentAndFrames.hpp"

class HoltropMennenForceModel : public ControllableForceModel
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
        struct DerivedData
        {
                DerivedData(const Input& base_data);
                double c17;
                double c15;
                double c7;
                double c4;
                double T;
                double Pb;
                double c14;
                // Dependencies of rank 1
                double c5;
                double m3;
                double Cb;
                double Cp;
                double c3;
                // Dependencies of rank 2
                double Ca;
                double S;
                double c16;
                double Lr;
                double lambda;
                double c2;
                double iE;
                // Dependencies of rank 3
                double c1;
                double m1;
                double hull_form_coeff;

            private:
                DerivedData(); // Deactivated
        };

        HoltropMennenForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;
        static Input parse(const std::string& yaml);
        static std::string model_name();

        double Rf(const BodyStates& states) const;   // Frictional resistance over the hull
        double Rapp(const BodyStates& states) const; // Frictional resistance over the appendages
        double Rw(const BodyStates& states) const;   // Wave-making resistance
        double Rb(const BodyStates& states) const;   // Bulbous bow influence
        double Rtr(const BodyStates& states) const;  // Immersed transom stern influence
        double Ra(const BodyStates& states) const;   // Correlation term between model and full scale

    private:
        const EnvironmentAndFrames env;
        const double d;
        const Input input;
        const DerivedData derived;
        bool apply_on_ship_speed_direction; // This parameter is used to apply the force on the ship's speed direction rather than the X-axis.
        const double gravity_force;
        double Rw_a(const double Fn, const double m4) const;
        double Rw_b(const double Fn, const double m4) const;
};

#endif /* FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_ */

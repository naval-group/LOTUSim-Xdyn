/*
 * HoltropMennenForceModel.hpp
 *
 *  Created on: 16 janv. 2020
 *      Author: mcharlou2016
 */

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
        boost::optional<double> form_coeff_hull;
        double form_coeff_app;
        bool apply_on_ship_speed_direction;
    };
    HoltropMennenForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env);
    ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const override;
    static Input parse(const std::string& yaml);
    static std::string model_name();

    double Rf(const BodyStates& states) const;
    double Rapp(const BodyStates& states) const;
    double Rw(const BodyStates& states) const;
    double Rb(const BodyStates& states) const;
    double Rtr(const BodyStates& states) const;
    double Ra(const BodyStates& states) const;

    // Getters for optional input values that can be computed by empirical formulae inside the model
    double get_iE() const;
    double get_S() const;
    double get_hull_form_coeff() const;

private:
    const double rho;
    const double nu;
    const double g;
    bool apply_on_ship_speed_direction;
    const double d;
    const double L;
    const double Lpp;
    const double B;
    const double c7;
    const double Ta;
    const double Tf;
    const double c4;
    const double T;
    const double m3;
    const double Vol;
    const double c15;
    const double lcb;
    const double Abt;
    const double hb;
    const double Pb;
    const double c3;
    const double c2;
    double Ca;
    const double Cm;
    double S;
    const double c17;
    const double Cp;
    const double c16;
    const double m1;
    const double Lr;
    const double lambda;
    const double Cwp;
    const double iE;
    const double c1;
    const double At;
    const double c5;
    const double Sapp;
    const double Cstern;
    const double c14;
    const double form_coeff_hull;
    const double form_coeff_app;
    const double Cb;
    const std::function<double(double, double)> Rw_a;
    const std::function<double(double, double)> Rw_b;
};

#endif /* FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_ */

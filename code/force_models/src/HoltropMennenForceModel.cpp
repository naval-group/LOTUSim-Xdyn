#include <cmath>
#include "yaml.h"
#include <ssc/yaml_parser.hpp>
#include <ssc/exception_handling.hpp>

#include "EnvironmentAndFrames.hpp"

#include "HoltropMennenForceModel.hpp"

std::string HoltropMennenForceModel::model_name()
{
    return "Holtrop & Mennen";
}

HoltropMennenForceModel::Input::Input() :
        Lwl(),
        Lpp(),
        B(),
        Ta(),
        Tf(),
        Vol(),
        lcb(),
        S(),
        Abt(),
        hb(),
        Cm(),
        Cwp(),
        iE(),
        At(),
        Sapp(),
        Cstern(),
        hull_form_coeff(),
        app_form_coeff(),
        apply_on_ship_speed_direction(false)
{
}

HoltropMennenForceModel::Input HoltropMennenForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;

    ssc::yaml_parser::parse_uv(node["Lwl"], ret.Lwl);
    ssc::yaml_parser::parse_uv(node["Lpp"], ret.Lpp);
    ssc::yaml_parser::parse_uv(node["B"], ret.B);
    ssc::yaml_parser::parse_uv(node["Ta"], ret.Ta);
    ssc::yaml_parser::parse_uv(node["Tf"], ret.Tf);
    ssc::yaml_parser::parse_uv(node["Vol"], ret.Vol);
    node["lcb"] >> ret.lcb;
    if(node.FindValue("S"))
    {
        double S;
        ssc::yaml_parser::parse_uv(node["S"], S);
        ret.S = S;
    }
    ssc::yaml_parser::parse_uv(node["Abt"], ret.Abt);
    ssc::yaml_parser::parse_uv(node["hb"], ret.hb);
    node["Cm"] >> ret.Cm;
    node["Cwp"] >> ret.Cwp;
    if(node.FindValue("iE"))
    {
        double iE;
        ssc::yaml_parser::parse_uv(node["iE"], iE);
        ret.iE = iE*180/M_PI;
    }
    ssc::yaml_parser::parse_uv(node["At"], ret.At);
    ssc::yaml_parser::parse_uv(node["Sapp"], ret.Sapp);
    node["Cstern"] >> ret.Cstern;
    if(node.FindValue("1+k1"))
    {
        double hull_form_coeff;
        node["1+k1"] >> hull_form_coeff;
        ret.hull_form_coeff = hull_form_coeff;
    }
    node["1+k2"] >> ret.app_form_coeff;
    if(node.FindValue("apply on ship speed direction")) node["apply on ship speed direction"] >> ret.apply_on_ship_speed_direction;
    return ret;
}

HoltropMennenForceModel::DerivedData::DerivedData(const Input& base_data) :
        c17(6919.3 * std::pow(base_data.Cm, -1.3346) * std::pow(base_data.Vol / std::pow(base_data.Lwl, 3.), 2.00977) * std::pow(base_data.Lwl / base_data.B - 2, 1.40692)),
        c15(std::pow(base_data.Lwl, 3) / base_data.Vol < 512 ? -1.69385 : (std::pow(base_data.Lwl, 3) / base_data.Vol > 1726.91 ? 0 : -1.69385 + (base_data.Lwl / std::pow(base_data.Vol, 1 / 3.) - 8) / 2.36)),
        c7(base_data.B / base_data.Lwl < 0.11 ? 0.229577 * std::pow(base_data.B / base_data.Lwl, 0.33333) : (base_data.B / base_data.Lwl > 0.25 ? 0.5 - 0.0625 * base_data.Lwl / base_data.B : base_data.B / base_data.Lwl)),
        c4(base_data.Tf / base_data.Lwl > 0.04 ? 0.04 : base_data.Tf / base_data.Lwl),
        T((base_data.Ta + base_data.Tf) / 2),
        Pb(0.56 * std::sqrt(base_data.Abt) / (base_data.Tf - 1.5 * base_data.hb)),
        c14(1 + 0.011 * base_data.Cstern),
        // Dependencies of rank 1
        c5(1 - 0.8 * base_data.At / (base_data.B * T * base_data.Cm)),
        m3(-7.2035 * std::pow(base_data.B / base_data.Lwl, 0.326869) * std::pow(T / base_data.B, 0.605375)),
        Cb(base_data.Vol / (base_data.Lwl * base_data.B * T)),
        Cp(base_data.Vol / (base_data.Cm * base_data.B * T * base_data.Lwl)),
        c3(0.56 * std::pow(base_data.Abt, 1.5) / (base_data.B * T * (0.31 * std::sqrt(base_data.Abt) + base_data.Tf - base_data.hb))),
        // Dependencies of rank 2
        Ca(0.006 * std::pow(base_data.Lwl + 100., -0.16) - 0.00205 + 0.003 * std::sqrt(base_data.Lwl / 7.5) * std::pow(Cb, 4.) * c2 * (0.04 - c4)),
        S(base_data.S.is_initialized() ? base_data.S.get() : base_data.Lwl * (2 * T + base_data.B) * std::sqrt(base_data.Cm) * (0.453 + 0.4425 * Cb - 0.2862 * base_data.Cm - 0.003467 * base_data.B / T + 0.3696 * base_data.Cwp) + 2.38 * base_data.Abt / Cb),
        c16(Cp < 0.8 ? 8.07981 * Cp - 13.8673 * std::pow(Cp, 2.) + 6.984388 * std::pow(Cp, 3.) : 1.73014 - 0.7067 * Cp),
        Lr(base_data.Lwl * (1 - Cp + 0.06 * Cp * base_data.lcb / (4 * Cp - 1))),
        lambda(base_data.Lwl / base_data.B < 12 ? 1.446 * Cp - 0.03 * base_data.Lwl / base_data.B : 1.446 * Cp - 0.36),
        c2(std::exp(-1.89 * std::sqrt(c3))),
        iE(base_data.iE.is_initialized() ? base_data.iE.get() : 1 + 89 * std::exp(-std::pow(base_data.Lwl / base_data.B, 0.80856) * std::pow(1 - base_data.Cwp, 0.30484) * std::pow(1 - Cp - 0.0225 * base_data.lcb, 0.6367) * std::pow(Lr / base_data.B, 0.34574) * std::pow(100 * base_data.Vol / std::pow(base_data.Lwl, 3.), 0.16302))),
        // Dependencies of rank 3
        c1(2223105 * std::pow(c7, 3.78613) * std::pow(T / base_data.B, 1.07961) * std::pow(90 - iE, -1.37565)),
        m1(0.0140407 * base_data.Lwl / T - 1.75254 * std::pow(base_data.Vol, 1 / 3.) / base_data.Lwl - 4.79323 *base_data. B / base_data.Lwl - c16),
        hull_form_coeff(base_data.hull_form_coeff.is_initialized() ? base_data.hull_form_coeff.get() : 0.93 + 0.487118 * c14 * std::pow(base_data.B / base_data.Lpp, 1.06806) * std::pow(T / base_data.Lwl, 0.46106) * std::pow(base_data.Lwl / Lr, 0.121563) * std::pow(std::pow(base_data.Lwl, 3.) / base_data.Vol, 0.36486) * std::pow(1 - Cp, -0.604247))
{
}

HoltropMennenForceModel::HoltropMennenForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env_) :
        ForceModel(HoltropMennenForceModel::model_name(), body_name),
        d(-0.9),
        env(env_),
        input(data),
        derived(data),
        gravity_force(input.Vol*env.rho*env.g)
{
}

double HoltropMennenForceModel::Rw_a(const double Fn, const double m4) const
{
    return derived.c1*derived.c2*derived.c5*gravity_force*std::exp(derived.m1*std::pow(Fn,d)+m4*cos(derived.lambda*std::pow(Fn,-2.)));
}

double HoltropMennenForceModel::Rw_b(const double Fn, const double m4) const
{
    return derived.c17*derived.c2*derived.c5*gravity_force*std::exp(derived.m3*std::pow(Fn,d)+m4*cos(derived.lambda*std::pow(Fn,-2.)));
}

ssc::kinematics::Wrench HoltropMennenForceModel::operator()(const BodyStates& states, const double t) const
{
    ssc::kinematics::Wrench tau(states.G);
    if(states.u() > 0)
    {
        double R = Rf(states) + Rapp(states) + Rw(states) + Rb(states) + Rtr(states) + Ra(states);
        if(!apply_on_ship_speed_direction)
        {
            tau.X() = -R;
        }
        else
        {
            Eigen::Vector3d dir( { states.u(), states.v(), states.w() });
            dir.normalize();
            tau.X() = -dir(0) * R;
            tau.Y() = -dir(1) * R;
            tau.Z() = -dir(2) * R;
        }
    }
    return tau;
}

double HoltropMennenForceModel::Rf(const BodyStates& states) const
{
    const double Re = states.u() * input.Lwl / env.nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    return derived.hull_form_coeff *  Cf * 0.5 * env.rho * std::pow(states.u(), 2.) * derived.S;
}

double HoltropMennenForceModel::Rapp(const BodyStates& states) const
{
    const double Re = states.u() * input.Lwl / env.nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    return input.app_form_coeff * Cf * 0.5 * env.rho * std::pow(states.u(), 2.) * input.Sapp;
}

double HoltropMennenForceModel::Rw(const BodyStates& states) const
{
    const double Fn = states.u() / std::sqrt(env.g * input.Lwl);
    double m4 = derived.c15 * 0.4 * std::exp(-0.034 * std::pow(Fn, -3.29));
    double Rw;

    if(Fn == 0)
    {
        return 0;
    }
    if(Fn < 0.4)
    {
        return Rw_a(Fn, m4);
    }
    if(Fn > 0.55)
    {
        return Rw_b(Fn, m4);
    }
    return Rw_a(0.4, m4) + (Fn - 0.4) * (Rw_b(0.55, m4) - Rw_a(0.4, m4)) / 0.15;
}

double HoltropMennenForceModel::Rb(const BodyStates& states) const
{
    if(input.Abt != 0)
    {
        const double Fni = states.u() / std::sqrt(env.g * (input.Tf - input.hb - 0.25 * std::sqrt(input.Abt)) + 0.15 * std::pow(states.u(), 2.));
        return 0.11 * std::exp(-3 * std::pow(derived.Pb, -2)) * std::pow(Fni, 3.) * std::pow(input.Abt, 1.5) * env.rho * env.g / (1 + std::pow(Fni, 2.));
    }
    return 0;
}

double HoltropMennenForceModel::Rtr(const BodyStates& states) const
{
    const double FnT = states.u() / std::sqrt(2 * env.g * input.At / (input.B + input.B * input.Cwp));
    const double c6 = (FnT < 5 ? 0.2 * (1 - 0.2 * FnT) : 0);
    return 0.5 * env.rho * std::pow(states.u(), 2.) * input.At * c6;
}

double HoltropMennenForceModel::Ra(const BodyStates& states) const
{
    return 0.5 * env.rho * std::pow(states.u(), 2.) * derived.S * derived.Ca;
}

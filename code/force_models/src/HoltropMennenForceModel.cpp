/*
 * HoltropMennenForceModel.cpp
 *
 *  Created on: 16 janv. 2020
 *      Author: mcharlou2016
 */

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

HoltropMennenForceModel::HoltropMennenForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env) :
        ForceModel(HoltropMennenForceModel::model_name(), body_name),
        rho(env.rho),
        nu(env.nu),
        g(env.g),
        apply_on_ship_speed_direction(data.apply_on_ship_speed_direction),
        d(-0.9),
        L(data.Lwl),
        Lpp(data.Lpp),
        B(data.B),
        c7((B / L < 0.11 ? 0.229577 * std::pow(B / L, 0.33333) : (B / L > 0.25 ? 0.5 - 0.0625 * L / B : B / L))),
        Ta(data.Ta),
        Tf(data.Tf),
        c4((Tf / L > 0.04 ? 0.04 : Tf / L)),
        T((data.Ta + data.Tf) / 2),
        m3(-7.2035 * std::pow(B / L, 0.326869) * std::pow(T / B, 0.605375)),
        Vol(data.Vol),
        Cb(Vol / (L * B * T)),
        c15((std::pow(L, 3) / Vol < 512 ? -1.69385 : (std::pow(L, 3) / Vol > 1726.91 ? 0 : -1.69385 + (L / std::pow(Vol, 1 / 3.) - 8) / 2.36))),
        lcb(data.lcb),
        Abt(data.Abt),
        hb(data.hb),
        Pb(0.56 * std::sqrt(Abt) / (Tf - 1.5 * hb)),
        c3(0.56 * std::pow(Abt, 1.5) / (B * T * (0.31 * std::sqrt(Abt) + Tf - hb))),
        c2(std::exp(-1.89 * std::sqrt(c3))),
        Ca(),
        Cm(data.Cm),
        S(),
        c17(6919.3 * std::pow(Cm, -1.3346) * std::pow(Vol / std::pow(L, 3.), 2.00977) * std::pow(L / B - 2, 1.40692)),
        Cp(Vol / (Cm * B * T * L)),
        c16((Cp < 0.8 ? 8.07981 * Cp - 13.8673 * std::pow(Cp, 2.) + 6.984388 * std::pow(Cp, 3.) : 1.73014 - 0.7067 * Cp)),
        m1(0.0140407 * L / T - 1.75254 * std::pow(Vol, 1 / 3.) / L - 4.79323 * B / L - c16),
        Lr(L * (1 - Cp + 0.06 * Cp * lcb / (4 * Cp - 1))),
        lambda((L / B < 12 ? 1.446 * Cp - 0.03 * L / B : 1.446 * Cp - 0.36)),
        Cwp(data.Cwp),
        iE(data.iE.is_initialized() ? data.iE.get() : 1 + 89 * std::exp(-std::pow(L / B, 0.80856) * std::pow(1 - Cwp, 0.30484) * std::pow(1 - Cp - 0.0225 * lcb, 0.6367) * std::pow(Lr / B, 0.34574) * std::pow(100 * Vol / std::pow(L, 3.), 0.16302))),
        c1(2223105 * std::pow(c7, 3.78613) * std::pow(T / B, 1.07961) * std::pow(90 - iE, -1.37565)),
        At(data.At),
        c5(1 - 0.8 * At / (B * T * Cm)),
        Sapp(data.Sapp),
        Cstern(data.Cstern),
        c14(1 + 0.011 * Cstern),
        hull_form_coeff(data.hull_form_coeff.is_initialized() ? data.hull_form_coeff.get() : 0.93 + 0.487118 * c14 * std::pow(B / Lpp, 1.06806) * std::pow(T / L, 0.46106) * std::pow(L / Lr, 0.121563) * std::pow(std::pow(L, 3.) / Vol, 0.36486) * std::pow(1 - Cp, -0.604247)),
        app_form_coeff(data.app_form_coeff),
        Rw_a([&](double Fn, double m4) {return c1*c2*c5*Vol*env.rho*env.g*std::exp(m1*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));}),
        Rw_b([&](double Fn, double m4) {return c17*c2*c5*Vol*env.rho*env.g*std::exp(m3*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));})
{
    Ca = 0.006 * std::pow(L + 100., -0.16) - 0.00205 + 0.003 * std::sqrt(L / 7.5) * std::pow(Cb, 4.) * c2 * (0.04 - c4);
    if(data.S.is_initialized()) S = data.S.get();
    else S = L * (2 * T + B) * std::sqrt(Cm) * (0.453 + 0.4425 * Cb - 0.2862 * Cm - 0.003467 * B / T + 0.3696 * Cwp) + 2.38 * Abt / Cb;
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
    const double Re = states.u() * L / nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    const double Rf = Cf * 0.5 * rho * std::pow(states.u(), 2.) * S;
    return hull_form_coeff * Rf;
}

double HoltropMennenForceModel::Rapp(const BodyStates& states) const
{
    const double Re = states.u() * L / nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    const double Rapp = Cf * 0.5 * rho * std::pow(states.u(), 2.) * Sapp;
    return app_form_coeff * Rapp;
}

double HoltropMennenForceModel::Rw(const BodyStates& states) const
{
    const double Fn = states.u() / std::sqrt(g * L);
    double m4 = c15 * 0.4 * std::exp(-0.034 * std::pow(Fn, -3.29));
    double Rw;

    if(Fn == 0)
    {
        Rw = 0;
    }
    else if(Fn < 0.4)
    {
        Rw = Rw_a(Fn, m4);
    }
    else if(Fn > 0.55)
    {
        Rw = Rw_b(Fn, m4);
    }
    else
    {
        Rw = Rw_a(0.4, m4) + (Fn - 0.4) * (Rw_b(0.55, m4) - Rw_a(0.4, m4)) / 0.15;
    }
    return Rw;
}

double HoltropMennenForceModel::Rb(const BodyStates& states) const
{
    double Rb;
    if(Abt != 0)
    {
        const double Fni = states.u() / std::sqrt(g * (Tf - hb - 0.25 * std::sqrt(Abt)) + 0.15 * std::pow(states.u(), 2.));
        Rb = 0.11 * std::exp(-3 * std::pow(Pb, -2)) * std::pow(Fni, 3.) * std::pow(Abt, 1.5) * rho * g / (1 + std::pow(Fni, 2.));
    }
    else Rb = 0;
    return Rb;
}

double HoltropMennenForceModel::Rtr(const BodyStates& states) const
{
    const double FnT = states.u() / std::sqrt(2 * g * At / (B + B * Cwp));
    const double c6 = (FnT < 5 ? 0.2 * (1 - 0.2 * FnT) : 0);
    const double Rtr = 0.5 * rho * std::pow(states.u(), 2.) * At * c6;
    return Rtr;
}

double HoltropMennenForceModel::Ra(const BodyStates& states) const
{
    const double Ra = 0.5 * rho * std::pow(states.u(), 2.) * S * Ca;
    return Ra;
}

double HoltropMennenForceModel::get_iE() const
{
    return iE;
}

double HoltropMennenForceModel::get_S() const
{
    return S;
}

double HoltropMennenForceModel::get_hull_form_coeff() const
{
    return hull_form_coeff;
}

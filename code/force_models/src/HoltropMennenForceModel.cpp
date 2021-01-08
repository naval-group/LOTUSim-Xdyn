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

HoltropMennenForceModel::HoltropMennenForceModel(const Input& input_, const std::string& body_name, const EnvironmentAndFrames& env):
        ForceModel(HoltropMennenForceModel::model_name(), body_name),
        apply_on_ship_speed_direction(input_.apply_on_ship_speed_direction),
        Rf(input_, env),
        Rapp(input_, env),
        Rw(input_, env),
        Rb(input_, env),
        Rtr(input_, env),
        Ra(input_, env)
{
}

ssc::kinematics::Wrench HoltropMennenForceModel::operator()(const BodyStates& states, const double t) const
{
    ssc::kinematics::Wrench tau(states.G);
    if(states.u() > 0)
    {
        double R = Rf.compute(states) + Rapp.compute(states) + Rw.compute(states) + Rb.compute(states) + Rtr.compute(states) + Ra.compute(states);
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

HoltropMennenForceModel::Computer::Computer(const Input& input_, const EnvironmentAndFrames& env) : input(input_), rho(env.rho), nu(env.nu), g(env.g)
{
}

double HoltropMennenForceModel::get_Rf(const BodyStates& states) const
{
    return Rf.compute(states);
}

HoltropMennenForceModel::RfComputer::RfComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env),
        S(get_S(input, get_T(input), get_Cb(input, get_T(input)))),
        hull_form_coeff(get_hull_form_coeff(input, get_T(input), get_Lr(input, get_Cp(input, get_T(input))), get_Cp(input, get_T(input)), get_c14(input)))
{
}

double HoltropMennenForceModel::RfComputer::compute(const BodyStates& states) const
{
    const double Re = states.u() * input.Lwl / nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    const double Rf = Cf * 0.5 * rho * std::pow(states.u(), 2.) * S;
    return hull_form_coeff * Rf;
}

double HoltropMennenForceModel::get_Rapp(const BodyStates& states) const
{
    return Rapp.compute(states);
}

HoltropMennenForceModel::RappComputer::RappComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env)
{
}

double HoltropMennenForceModel::RappComputer::compute(const BodyStates& states) const
{
    const double Re = states.u() * input.Lwl / nu;
    const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
    const double Rapp = Cf * 0.5 * rho * std::pow(states.u(), 2.) * input.Sapp;
    return input.app_form_coeff * Rapp;
}

double HoltropMennenForceModel::get_Rw(const BodyStates& states) const
{
    return Rw.compute(states);
}

HoltropMennenForceModel::RwComputer::RwComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env),
        c15(get_c15(input)),
        c1(get_c1(input, get_T(input), get_c7(input), get_iE(input, get_Cp(input, get_T(input)), get_Lr(input, get_Cp(input, get_T(input)))))),
        c2(get_c2(get_c3(input, get_T(input)))),
        c5(get_c5(input, get_T(input))),
        c17(get_c17(input)),
        m1(get_m1(input, get_T(input), get_c16(get_Cp(input, get_T(input))))),
        m3(get_m3(input, get_T(input))),
        d(-0.9),
        lambda(get_lambda(input, get_Cp(input, get_T(input)))),
        Rw_a([&](double Fn, double m4) {return c1*c2*c5*input.Vol*rho*g*std::exp(m1*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));}),
        Rw_b([&](double Fn, double m4) {return c17*c2*c5*input.Vol*rho*g*std::exp(m3*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));})
{
}

double HoltropMennenForceModel::RwComputer::compute(const BodyStates& states) const
{
    const double Fn = states.u() / std::sqrt(g * input.Lwl);
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

double HoltropMennenForceModel::get_Rb(const BodyStates& states) const
{
    return Rb.compute(states);
}

HoltropMennenForceModel::RbComputer::RbComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env),
        Pb(get_Pb(input))
{
}

double HoltropMennenForceModel::RbComputer::compute(const BodyStates& states) const
{
    double Rb;
    if(input.Abt != 0)
    {
        const double Fni = states.u() / std::sqrt(g * (input.Tf - input.hb - 0.25 * std::sqrt(input.Abt)) + 0.15 * std::pow(states.u(), 2.));
        Rb = 0.11 * std::exp(-3 * std::pow(Pb, -2)) * std::pow(Fni, 3.) * std::pow(input.Abt, 1.5) * rho * g / (1 + std::pow(Fni, 2.));
    }
    else Rb = 0;
    return Rb;
}

double HoltropMennenForceModel::get_Rtr(const BodyStates& states) const
{
    return Rtr.compute(states);
}

HoltropMennenForceModel::RtrComputer::RtrComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env)
{
}

double HoltropMennenForceModel::RtrComputer::compute(const BodyStates& states) const
{
    const double FnT = states.u() / std::sqrt(2 * g * input.At / (input.B + input.B * input.Cwp));
    const double c6 = (FnT < 5 ? 0.2 * (1 - 0.2 * FnT) : 0);
    const double Rtr = 0.5 * rho * std::pow(states.u(), 2.) * input.At * c6;
    return Rtr;
}

double HoltropMennenForceModel::get_Ra(const BodyStates& states) const
{
    return Ra.compute(states);
}

HoltropMennenForceModel::RaComputer::RaComputer(const Input& input, const EnvironmentAndFrames& env):
        Computer(input, env),
        S(get_S(input, get_T(input), get_Cb(input, get_T(input)))),
        Ca(get_Ca(input, get_Cb(input, get_T(input)), get_c2(get_c3(input, get_T(input))), get_c4(input)))
{
}

double HoltropMennenForceModel::RaComputer::compute(const BodyStates& states) const
{
    const double Ra = 0.5 * rho * std::pow(states.u(), 2.) * S * Ca;
    return Ra;
}

double HoltropMennenForceModel::get_iE(const Input& input, const double Cp, const double Lr)
{
    double iE;
    if(input.iE.is_initialized())
        {
        iE = input.iE.get();
        }
    else
    {
        iE = 1 + 89 * std::exp(-std::pow(input.Lwl / input.B, 0.80856) * std::pow(1 - input.Cwp, 0.30484) * std::pow(1 - Cp - 0.0225 * input.lcb, 0.6367) * std::pow(Lr / input.B, 0.34574) * std::pow(100 * input.Vol / std::pow(input.Lwl, 3.), 0.16302));
    }
    return iE;
}

double HoltropMennenForceModel::get_S(const Input& input, const double T, const double Cb)
{
    double S;
    if(input.S.is_initialized())
    {
        S = input.S.get();
    }
    else
    {
        S = input.Lwl * (2 * T + input.B) * std::sqrt(input.Cm) * (0.453 + 0.4425 * Cb - 0.2862 * input.Cm - 0.003467 * input.B / T + 0.3696 * input.Cwp) + 2.38 * input.Abt / Cb;
    }
    return S;
}

double HoltropMennenForceModel::get_hull_form_coeff(const Input& input, const double T, const double Lr, const double Cp, const double c14)
{
    double hull_form_coeff;
    if(input.hull_form_coeff.is_initialized())
        {
        hull_form_coeff = input.hull_form_coeff.get();
        }
    else
    {
        hull_form_coeff = 0.93 + 0.487118 * c14 * std::pow(input.B / input.Lpp, 1.06806) * std::pow(T / input.Lwl, 0.46106) * std::pow(input.Lwl / Lr, 0.121563) * std::pow(std::pow(input.Lwl, 3.) / input.Vol, 0.36486) * std::pow(1 - Cp, -0.604247);
    }
    return hull_form_coeff;
}

double HoltropMennenForceModel::get_c7(const Input& input)
{
    double c7;
    if(input.B / input.Lwl < 0.11)
    {
        c7 = 0.229577 * std::pow(input.B / input.Lwl, 0.33333);
    }
    else if(input.B / input.Lwl > 0.25)
    {
        c7 = 0.5 - 0.0625 * input.Lwl / input.B;
    }
    else
    {
        c7 = input.B / input.Lwl;
    }
    return c7;
}

double HoltropMennenForceModel::get_c4(const Input& input)
{
    double c4;
    if(input.Tf / input.Lwl > 0.04)
        {
        c4 = 0.04;
        }
    else
    {
        c4 = input.Tf / input.Lwl;
    }
    return c4;
}

double HoltropMennenForceModel::get_T(const Input& input)
{
    return (input.Ta + input.Tf) / 2;
}

double HoltropMennenForceModel::get_Cb(const Input& input, const double T)
{
    return input.Vol / (input.Lwl * input.B * T);
}

double HoltropMennenForceModel::get_Cp(const Input& input, const double T)
{
    return input.Vol / (input.Cm * input.B * T * input.Lwl);
}

double HoltropMennenForceModel::get_m3(const Input& input, const double T)
{
    return -7.2035 * std::pow(input.B / input.Lwl, 0.326869) * std::pow(T / input.B, 0.605375);
}

double HoltropMennenForceModel::get_c15(const Input& input)
{
    double c15;
    if(std::pow(input.Lwl, 3) / input.Vol < 512)
    {
        c15 = -1.69385;
    }
    else if(std::pow(input.Lwl, 3) / input.Vol > 1726.91)
    {
        c15 = 0;
    }
    else
    {
        c15 = -1.69385 + (input.Lwl / std::pow(input.Vol, 1 / 3.) - 8) / 2.36;
    }
    return c15;
}

double HoltropMennenForceModel::get_Pb(const Input& input)
{
    return 0.56 * std::sqrt(input.Abt) / (input.Tf - 1.5 * input.hb);
}

double HoltropMennenForceModel::get_c3(const Input& input, const double T)
{
    return 0.56 * std::pow(input.Abt, 1.5) / (input.B * T * (0.31 * std::sqrt(input.Abt) + input.Tf - input.hb));
}

double HoltropMennenForceModel::get_c2(const double c3)
{
    return std::exp(-1.89 * std::sqrt(c3));
}

double HoltropMennenForceModel::get_Ca(const Input& input, const double Cb, const double c2, const double c4)
{
    return 0.006 * std::pow(input.Lwl + 100., -0.16) - 0.00205 + 0.003 * std::sqrt(input.Lwl / 7.5) * std::pow(Cb, 4.) * c2 * (0.04 - c4);
}

double HoltropMennenForceModel::get_c17(const Input& input)
{
    return 6919.3 * std::pow(input.Cm, -1.3346) * std::pow(input.Vol / std::pow(input.Lwl, 3.), 2.00977) * std::pow(input.Lwl / input.B - 2, 1.40692);
}

double HoltropMennenForceModel::get_c16(const double Cp)
{
    double c16;
    if(Cp < 0.8)
    {
        c16 = 8.07981 * Cp - 13.8673 * std::pow(Cp, 2.) + 6.984388 * std::pow(Cp, 3.);
    }
    else
    {
        c16 = 1.73014 - 0.7067 * Cp;
    }
    return c16;
}

double HoltropMennenForceModel::get_m1(const Input& input, const double T, const double c16)
{
    return 0.0140407 * input.Lwl / T - 1.75254 * std::pow(input.Vol, 1 / 3.) / input.Lwl - 4.79323 * input.B / input.Lwl - c16;
}

double HoltropMennenForceModel::get_Lr(const Input& input, const double Cp)
{
    return input.Lwl * (1 - Cp + 0.06 * Cp * input.lcb / (4 * Cp - 1));
}

double HoltropMennenForceModel::get_lambda(const Input& input, const double Cp)
{
    double lambda;
    if(input.Lwl / input.B < 12)
        {
        lambda = 1.446 * Cp - 0.03 * input.Lwl / input.B;
        }
    else
    {
        lambda = 1.446 * Cp - 0.36;
    }
    return lambda;
}

double HoltropMennenForceModel::get_c1(const Input& input, const double T, const double c7, const double iE)
{
    return 2223105 * std::pow(c7, 3.78613) * std::pow(T / input.B, 1.07961) * std::pow(90 - iE, -1.37565);
}

double HoltropMennenForceModel::get_c5(const Input& input, const double T)
{
    return 1 - 0.8 * input.At / (input.B * T * input.Cm);
}

double HoltropMennenForceModel::get_c14(const Input& input)
{
    return 1 + 0.011 * input.Cstern;
}


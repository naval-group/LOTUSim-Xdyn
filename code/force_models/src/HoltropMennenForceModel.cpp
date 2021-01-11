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

double get_T(const HoltropMennenForceModel::Input& input);
double get_T(const HoltropMennenForceModel::Input& input)
{
    return (input.Ta + input.Tf) / 2;
}

double get_Cp(const HoltropMennenForceModel::Input& input);
double get_Cp(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    return input.Vol / (input.Cm * input.B * T * input.Lwl);
}

double get_Lr(const HoltropMennenForceModel::Input& input);
double get_Lr(const HoltropMennenForceModel::Input& input)
{
    const double Cp = get_Cp(input);
    return input.Lwl * (1 - Cp + 0.06 * Cp * input.lcb / (4 * Cp - 1));
}

double get_iE(const HoltropMennenForceModel::Input& input);
double get_iE(const HoltropMennenForceModel::Input& input)
{
    if(input.iE.is_initialized())
    {
        return input.iE.get();
    }
    const double Cp = get_Cp(input);
    const double Lr = get_Lr(input);
    return 1 + 89 * std::exp(-std::pow(input.Lwl / input.B, 0.80856) * std::pow(1 - input.Cwp, 0.30484) * std::pow(1 - Cp - 0.0225 * input.lcb, 0.6367) * std::pow(Lr / input.B, 0.34574) * std::pow(100 * input.Vol / std::pow(input.Lwl, 3.), 0.16302));
}

double get_Cb(const HoltropMennenForceModel::Input& input);
double get_Cb(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    return input.Vol / (input.Lwl * input.B * T);
}

double get_S(const HoltropMennenForceModel::Input& input);
double get_S(const HoltropMennenForceModel::Input& input)
{
    if(input.S.is_initialized())
    {
        return input.S.get();
    }
    const double T = get_T(input);
    const double Cb = get_Cb(input);
    return input.Lwl * (2 * T + input.B) * std::sqrt(input.Cm) * (0.453 + 0.4425 * Cb - 0.2862 * input.Cm - 0.003467 * input.B / T + 0.3696 * input.Cwp) + 2.38 * input.Abt / Cb;
}

double get_c14(const HoltropMennenForceModel::Input& input);
double get_c14(const HoltropMennenForceModel::Input& input)
{
    return 1 + 0.011 * input.Cstern;
}

double get_hull_form_coeff(const HoltropMennenForceModel::Input& input);
double get_hull_form_coeff(const HoltropMennenForceModel::Input& input)
{
    if(input.hull_form_coeff.is_initialized())
    {
        return input.hull_form_coeff.get();
    }
    const double T = get_T(input);
    const double Lr = get_Lr(input);
    const double Cp = get_Cp(input);
    const double c14 = get_c14(input);
    return 0.93 + 0.487118 * c14 * std::pow(input.B / input.Lpp, 1.06806) * std::pow(T / input.Lwl, 0.46106) * std::pow(input.Lwl / Lr, 0.121563) * std::pow(std::pow(input.Lwl, 3.) / input.Vol, 0.36486) * std::pow(1 - Cp, -0.604247);
}

double get_c7(const HoltropMennenForceModel::Input& input);
double get_c7(const HoltropMennenForceModel::Input& input)
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

double get_c4(const HoltropMennenForceModel::Input& input);
double get_c4(const HoltropMennenForceModel::Input& input)
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

double get_m3(const HoltropMennenForceModel::Input& input);
double get_m3(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    return -7.2035 * std::pow(input.B / input.Lwl, 0.326869) * std::pow(T / input.B, 0.605375);
}

double get_c15(const HoltropMennenForceModel::Input& input);
double get_c15(const HoltropMennenForceModel::Input& input)
{
    double c15;
    if(std::pow(input.Lwl, 3) / input.Vol < 512)
    {
        return -1.69385;
    }
    if(std::pow(input.Lwl, 3) / input.Vol > 1726.91)
    {
        return 0;
    }
    return -1.69385 + (input.Lwl / std::pow(input.Vol, 1 / 3.) - 8) / 2.36;
}

double get_Pb(const HoltropMennenForceModel::Input& input);
double get_Pb(const HoltropMennenForceModel::Input& input)
{
    return 0.56 * std::sqrt(input.Abt) / (input.Tf - 1.5 * input.hb);
}

double get_c3(const HoltropMennenForceModel::Input& input);
double get_c3(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    return 0.56 * std::pow(input.Abt, 1.5) / (input.B * T * (0.31 * std::sqrt(input.Abt) + input.Tf - input.hb));
}

double get_c2(const HoltropMennenForceModel::Input& input);
double get_c2(const HoltropMennenForceModel::Input& input)
{
    const double c3 = get_c3(input);
    return std::exp(-1.89 * std::sqrt(c3));
}

double get_Ca(const HoltropMennenForceModel::Input& input);
double get_Ca(const HoltropMennenForceModel::Input& input)
{
    const double Cb = get_Cb(input);
    const double c2 = get_c2(input);
    const double c4 = get_c4(input);
    return 0.006 * std::pow(input.Lwl + 100., -0.16) - 0.00205 + 0.003 * std::sqrt(input.Lwl / 7.5) * std::pow(Cb, 4.) * c2 * (0.04 - c4);
}

double get_c17(const HoltropMennenForceModel::Input& input);
double get_c17(const HoltropMennenForceModel::Input& input)
{
    return 6919.3 * std::pow(input.Cm, -1.3346) * std::pow(input.Vol / std::pow(input.Lwl, 3.), 2.00977) * std::pow(input.Lwl / input.B - 2, 1.40692);
}

double get_c16(const HoltropMennenForceModel::Input& input);
double get_c16(const HoltropMennenForceModel::Input& input)
{
    const double Cp = get_Cp(input);
    if(Cp < 0.8)
    {
        return 8.07981 * Cp - 13.8673 * std::pow(Cp, 2.) + 6.984388 * std::pow(Cp, 3.);
    }
    return 1.73014 - 0.7067 * Cp;
}

double get_m1(const HoltropMennenForceModel::Input& input);
double get_m1(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    const double c16 = get_c16(input);
    return 0.0140407 * input.Lwl / T - 1.75254 * std::pow(input.Vol, 1 / 3.) / input.Lwl - 4.79323 * input.B / input.Lwl - c16;
}

double get_lambda(const HoltropMennenForceModel::Input& input);
double get_lambda(const HoltropMennenForceModel::Input& input)
{
    const double Cp = get_Cp(input);
    if(input.Lwl / input.B < 12)
    {
        return 1.446 * Cp - 0.03 * input.Lwl / input.B;
    }
    return 1.446 * Cp - 0.36;
}

double get_c1(const HoltropMennenForceModel::Input& input);
double get_c1(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    const double c7 = get_c7(input);
    const double iE = get_iE(input);
    return 2223105 * std::pow(c7, 3.78613) * std::pow(T / input.B, 1.07961) * std::pow(90 - iE, -1.37565);
}

double get_c5(const HoltropMennenForceModel::Input& input);
double get_c5(const HoltropMennenForceModel::Input& input)
{
    const double T = get_T(input);
    return 1 - 0.8 * input.At / (input.B * T * input.Cm);
}

class RfComputer
{
    public:
        RfComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            S(get_S(input)),
            hull_form_coeff(get_hull_form_coeff(input)),
            rho(env.rho),
            Lwl(input.Lwl),
            nu(env.nu)
        {
        }

        double compute(const BodyStates& states) const
        {
            const double Re = states.u() * Lwl / nu;
            const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
            const double Rf = Cf * 0.5 * rho * std::pow(states.u(), 2.) * S;
            return hull_form_coeff * Rf;
        }

    private:
        const double S;
        const double hull_form_coeff;
        const double rho;
        const double Lwl;
        const double nu;
};

class RappComputer
{
    public:
        RappComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            app_form_coeff(input.app_form_coeff),
            rho(env.rho),
            Lwl(input.Lwl),
            nu(env.nu),
            Sapp(input.Sapp)
        {}
        double compute(const BodyStates& states) const
        {
            const double Re = states.u() * Lwl / nu;
            const double Cf = 0.075 / std::pow(std::log10(Re) - 2, 2.);
            const double Rapp = Cf * 0.5 * rho * std::pow(states.u(), 2.) * Sapp;
            return app_form_coeff * Rapp;
        }

    private:
        const double app_form_coeff;
        const double rho;
        const double Lwl;
        const double nu;
        const double Sapp;
};

class RwComputer
{
    public:
        RwComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            c15(get_c15(input)),
            c1(get_c1(input)),
            c2(get_c2(input)),
            c5(get_c5(input)),
            c17(get_c17(input)),
            m1(get_m1(input)),
            m3(get_m3(input)),
            d(-0.9),
            lambda(get_lambda(input)),
            Lwl(input.Lwl),
            g(env.g),
            gravitational_force(input.Vol*env.rho*g)
        {
        }

        double compute(const BodyStates& states) const
        {
            const double Fn = states.u() / std::sqrt(g * Lwl);
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
        double Rw_a(const double Fn, const double m4) const
        {
            return c1*c2*c5*gravitational_force*std::exp(m1*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));

        }
        double Rw_b(const double Fn, const double m4) const
        {
            return c17*c2*c5*gravitational_force*std::exp(m3*std::pow(Fn,d)+m4*cos(lambda*std::pow(Fn,-2.)));
        }

        private:
            const double c15;
            const double c1;
            const double c2;
            const double c5;
            const double c17;
            const double m1;
            const double m3;
            const double d;
            const double lambda;
            const double Lwl;
            const double g;
            const double gravitational_force;
};

class RbComputer
{
    public:
        RbComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            Pb(get_Pb(input)),
            Abt(input.Abt),
            Tf(input.Tf),
            g(env.g),
            rho(env.rho),
            hb(input.hb)
        {
        }
        double compute(const BodyStates& states) const
        {
            double Rb = 0;
            if(Abt != 0)
            {
                const double Fni = states.u() / std::sqrt(g * (Tf - hb - 0.25 * std::sqrt(Abt)) + 0.15 * std::pow(states.u(), 2.));
                Rb = 0.11 * std::exp(-3 * std::pow(Pb, -2)) * std::pow(Fni, 3.) * std::pow(Abt, 1.5) * rho * g / (1 + std::pow(Fni, 2.));
            }
            return Rb;
        }

    private:
        const double Pb;
        const double Abt;
        const double Tf;
        const double g;
        const double rho;
        const double hb;
};

class RtrComputer
{
    public:
        RtrComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            rho(env.rho),
            g(env.g),
            At(input.At),
            B(input.B),
            Cwp(input.Cwp)
        {}
        double compute(const BodyStates& states) const
        {
            const double FnT = states.u() / std::sqrt(2 * g * At / (B + B * Cwp));
            const double c6 = (FnT < 5 ? 0.2 * (1 - 0.2 * FnT) : 0);
            return 0.5 * rho * std::pow(states.u(), 2.) * At * c6;
        }
    private:
        const double rho;
        const double g;
        const double At;
        const double B;
        const double Cwp;
};

class RaComputer
{
    public:
        RaComputer(const HoltropMennenForceModel::Input& input, const EnvironmentAndFrames& env) :
            S(get_S(input)),
            Ca(get_Ca(input)),
            rho(env.rho)
        {}
        double compute(const BodyStates& states) const
        {
            return 0.5 * rho * std::pow(states.u(), 2.) * S * Ca;
        }
    private:
        const double S;
        const double Ca;
        const double rho;
};

struct HoltropMennenForceModel::Impl
{
        Impl(const Input& input_, const EnvironmentAndFrames& env):
                Rf(input_, env),
                Rapp(input_, env),
                Rw(input_, env),
                Rb(input_, env),
                Rtr(input_, env),
                Ra(input_, env)
                {}
    const RfComputer Rf;
    const RappComputer Rapp;
    const RwComputer Rw;
    const RbComputer Rb;
    const RtrComputer Rtr;
    const RaComputer Ra;
};

HoltropMennenForceModel::HoltropMennenForceModel(const Input& input_, const std::string& body_name, const EnvironmentAndFrames& env):
        ForceModel(HoltropMennenForceModel::model_name(), body_name),
        apply_on_ship_speed_direction(input_.apply_on_ship_speed_direction),
        pimpl(TR1(shared_ptr)<Impl>(new HoltropMennenForceModel::Impl(input_, env)))
{
}


double HoltropMennenForceModel::get_Rf(const BodyStates& states) const
{
    return pimpl->Rf.compute(states);
}

double HoltropMennenForceModel::get_Rapp(const BodyStates& states) const
{
    return pimpl->Rapp.compute(states);
}


double HoltropMennenForceModel::get_Rw(const BodyStates& states) const
{
    return pimpl->Rw.compute(states);
}

double HoltropMennenForceModel::get_Rb(const BodyStates& states) const
{
    return pimpl->Rb.compute(states);
}

double HoltropMennenForceModel::get_Rtr(const BodyStates& states) const
{
    return pimpl->Rtr.compute(states);
}

double HoltropMennenForceModel::get_Ra(const BodyStates& states) const
{
    return pimpl->Ra.compute(states);
}

ssc::kinematics::Wrench HoltropMennenForceModel::operator()(const BodyStates& states, const double t) const
{
    ssc::kinematics::Wrench tau(states.G);
    if(states.u() > 0)
    {
        double R = pimpl->Rf.compute(states) + pimpl->Rapp.compute(states) + pimpl->Rw.compute(states) + pimpl->Rb.compute(states) + pimpl->Rtr.compute(states) + pimpl->Ra.compute(states);
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

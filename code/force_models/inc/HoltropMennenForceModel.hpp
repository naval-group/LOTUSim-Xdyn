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

    static double get_iE(const Input& input, const double Cp, const double Lr);
    static double get_S(const Input& input, const double T, const double Cb);
    static double get_hull_form_coeff(const Input& input, const double T, const double Lr, const double Cp, const double c14);
    static double get_c7(const Input& input);
    static double get_c4(const Input& input);
    static double get_T(const Input& input);
    static double get_Cb(const Input& input, const double T);
    static double get_Cp(const Input& input, const double T);
    static double get_m3(const Input& input, const double T);
    static double get_c15(const Input& input);
    static double get_Pb(const Input& input);
    static double get_c3(const Input& input, const double T);
    static double get_c2(const double c3);
    static double get_Ca(const Input& input, const double Cb, const double c2, const double c4);
    static double get_c17(const Input& input);
    static double get_c16(const double Cp);
    static double get_m1(const Input& input, const double T, const double c16);
    static double get_Lr(const Input& input, const double Cp);
    static double get_lambda(const Input& input, const double Cp);
    static double get_c1(const Input& input, const double T, const double c7, const double iE);
    static double get_c5(const Input& input, const double T);
    static double get_c14(const Input& input);

    class Computer
    {
    public:
        virtual double compute(const BodyStates& states) const = 0;
        Computer(const Input& input, const EnvironmentAndFrames& env);
        virtual ~Computer(){}

    protected:
        const Input input;
        const double rho;
        const double nu;
        const double g;
    };

    class RfComputer : public Computer
    {
    public:
        RfComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RfComputer(){}

    private:
        RfComputer(); // Deactivated
        const double S;
        const double hull_form_coeff;
    };

    class RappComputer : public Computer
    {
    public:
        RappComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RappComputer(){}

    private:
        RappComputer(); // Deactivated
    };

    class RwComputer : public Computer
    {
    public:
        RwComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RwComputer(){}

    private:
        RwComputer(); // Deactivated
        const double c15;
        const double c1;
        const double c2;
        const double c5;
        const double c17;
        const double m1;
        const double m3;
        const double d;
        const double lambda;
        const std::function<double(double, double)> Rw_a;
        const std::function<double(double, double)> Rw_b;
    };

    class RbComputer : public Computer
    {
    public:
        RbComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RbComputer(){}

    private:
        RbComputer(); // Deactivated
        const double Pb;
    };

    class RtrComputer : public Computer
    {
    public:
        RtrComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RtrComputer(){}

    private:
        RtrComputer(); // Deactivated
    };

    class RaComputer : public Computer
    {
    public:
        RaComputer(const Input& input, const EnvironmentAndFrames& env);
        double compute(const BodyStates& states) const override;
        virtual ~RaComputer(){}

    private:
        RaComputer(); // Deactivated
        const double S;
        const double Ca;
    };

private:
    const bool apply_on_ship_speed_direction;
    const RfComputer Rf;
    const RappComputer Rapp;
    const RwComputer Rw;
    const RbComputer Rb;
    const RtrComputer Rtr;
    const RaComputer Ra;
};

#endif /* FORCE_MODELS_INC_HOLTROPMENNENFORCEMODEL_HPP_ */

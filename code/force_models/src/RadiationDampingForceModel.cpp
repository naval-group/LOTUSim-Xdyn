/*
 * RadiationDampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include "RadiationDampingForceModel.hpp"

#include "Body.hpp"
#include "HDBParser.hpp"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "RadiationDampingBuilder.hpp"
#include "external_data_structures_parsers.hpp"

#include <ssc/macros.hpp>
#include <ssc/text_file_reader.hpp>

#include <ssc/yaml_parser.hpp>

#include "yaml.h"

#include <Eigen/Dense>
#include <cassert>
#include <array>
#include <functional>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

std::string RadiationDampingForceModel::model_name() {return "radiation damping";}

class CSVWriter
{
    public:
        CSVWriter(std::ostream& os_, const std::string& x_name_, const std::vector<double>& x) : os(os_), x_name(x_name_), xs(x), functions(), function_names()
        {
        }
        CSVWriter& operator=(const CSVWriter& rhs);
        ~CSVWriter(){};
        CSVWriter(const CSVWriter&);

        void add(const std::string& name, const std::function<double(const double)>& f, const size_t i, const size_t j)
        {
            std::stringstream ss;
            ss << name << '_' << i << j;
            function_names.push_back(ss.str());
            functions.push_back(f);
        }

        void print()
        {
            print_title();
            print_values();

        }

    private:
        CSVWriter();

        void print_title()
        {
            os << x_name;
            for (auto f:function_names)
            {
                os << ',' << f;
            }
            os << std::endl;
        }

        void print_values()
        {
            for (auto x:xs)
            {
                os << x;
                for (auto f:functions)
                {
                    os << ',' << f(x);
                }
                os << std::endl;
            }
        }
        std::ostream& os;
        std::string x_name;
        std::vector<double> xs;
        std::vector<std::function<double(const double)> > functions;
        std::vector<std::string> function_names;

};

std::function<double(double)> operator*(const double a, const std::function<double(double)>& f)
{
    return [a, f](double x){return a*f(x);};
}

std::function<double(double)> operator+(const std::function<double(double)>& f, const std::function<double(double)>& g)
{
    return [f, g](double x){return f(x) + g(x);};
}

std::function<double(double)> operator-(const std::function<double(double)>& f, const std::function<double(double)>& g)
{
    return [f, g](double x){return f(x) - g(x);};
}

class RadiationDampingForceModel::Impl
{
    public:
        Impl(const TR1(shared_ptr)<HDBParser>& parser, const YamlRadiationDamping& yaml) : hdb{parser}, builder(RadiationDampingBuilder(yaml.type_of_quadrature_for_convolution, yaml.type_of_quadrature_for_cos_transform)),
        A(), Ka(), Kb(), omega(parser->get_angular_frequencies()), taus(),
        n(yaml.nb_of_points_for_retardation_function_discretization), Tmin(yaml.tau_min), Tmax(yaml.tau_max),
        H0(yaml.calculation_point_in_body_frame.x,yaml.calculation_point_in_body_frame.y,yaml.calculation_point_in_body_frame.y), forward_speed_correction(yaml.forward_speed_correction)
        {
            CSVWriter omega_writer(std::cerr, "omega", omega);
            taus = builder.build_regular_intervals(Tmin,Tmax,n);
            CSVWriter tau_writer(std::cerr, "tau", taus);

            if (forward_speed_correction && fabs(hdb->get_forward_speed()) > 1e-3)
            {
                std::cout << "WARNING: You chose to apply a forward speed correction in the 'Radiation Damping' force model, but the forward velocity specified in the HDB file is not zero." << std::endl;
            }

            A = parser->get_added_mass();

            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    const auto Ma = get_Ma(i,j);
                    Ka[i][j] = [this, Ma, i, j](double tau){return get_K(Ma)(tau) - A(i,j);};
                    const auto Br = get_Br(i,j);
                    Kb[i][j] = get_K(Br);
                    if (yaml.output_Br_and_K)
                    {
                        omega_writer.add("Br",Br,i+1,j+1);
                        tau_writer.add("K",Kb[i][j],i+1,j+1);
                    }
                }
            }
            if (yaml.output_Br_and_K)
            {
                std::cerr << "Debugging information for damping functions Br:" << std::endl;
                omega_writer.print();
                std::cerr << std::endl << "Debugging information for retardation functions K:" << std::endl;
                tau_writer.print();
            }
        }

        std::function<double(double)> get_Ma(const size_t i, const size_t j) const
        {
            return builder.build_interpolator(omega, hdb->get_added_mass_coeff(i, j));
        }

        std::function<double(double)> get_Br(const size_t i, const size_t j) const
        {
            return builder.build_interpolator(omega,hdb->get_radiation_damping_coeff(i,j));
        }

        std::function<double(double)> get_K(const std::function<double(double)>& Br) const
        {
            return builder.build_retardation_function(Br,taus,1E-3,omega.front(),omega.back());
        }

        /* This function does the operation Kb - Ka.Ls(Ubar) if forward_speed_correction, with
         *
         * Ls(Ubar) = ⎡ 0   0   0   0   0  -V ⎤  where Ubar = ⎡ U ⎤
         *            ⎢                       ⎥               ⎢   ⎥
         *            ⎢ 0   0   0   0   0   U ⎥               ⎢ V ⎥
         *            ⎢                       ⎥               ⎢   ⎥
         *            ⎢ 0   0   0   V  -U   0 ⎥               ⎢ W ⎥
         *            ⎢                       ⎥               ⎢   ⎥
         *            ⎢ 0   0   0   0   0   0 ⎥               ⎢ P ⎥
         *            ⎢                       ⎥               ⎢   ⎥
         *            ⎢ 0   0   0   0   0   0 ⎥               ⎢ Q ⎥
         *            ⎢                       ⎥               ⎢   ⎥
         *            ⎣ 0   0   0   0   0   0 ⎦               ⎣ R ⎦
         *
         * The capitalized velocities are the average velocities (over Tmax), Ubar is 'average_velocities'
         * Because the matrix is well known and almost empty, it is not worth it to compute it as an entire matrix product.
         *
         * Ka.Ls(Ubar) = ⎡ 0   0   0   V*Ka₁₃  -U*Ka₁₃  U*Ka₁₂-V*Ka₁₁ ⎤
         *               ⎢                                            ⎥
         *               ⎢ 0   0   0   V*Ka₂₃  -U*Ka₂₃  U*Ka₂₂-V*Ka₂₁ ⎥
         *               ⎢                                            ⎥
         *               ⎢ 0   0   0   V*Ka₃₃  -U*Ka₃₃  U*Ka₃₂-V*Ka₃₁ ⎥
         *               ⎢                                            ⎥
         *               ⎢ 0   0   0   V*Ka₄₃  -U*Ka₄₃  U*Ka₄₂-V*Ka₄₁ ⎥
         *               ⎢                                            ⎥
         *               ⎢ 0   0   0   V*Ka₅₃  -U*Ka₅₃  U*Ka₅₂-V*Ka₅₁ ⎥
         *               ⎢                                            ⎥
         *               ⎣ 0   0   0   V*Ka₆₃  -U*Ka₆₃  U*Ka₆₂-V*Ka₆₁ ⎦
         */

        std::function<double(double)> get_K(const size_t i, const size_t j, const std::array<double, 6>& Ubar)
        {
            if (forward_speed_correction && j < 3) // 3 last column
            {
                if (j == 3) // Column 4
                {
                    return Kb[i][j] + Ubar[1]*Ka[i][2]; // Kb(i,j) + V*Ka(i,3)
                }
                else if (j == 4) // Column 5
                {
                    return Kb[i][j] - Ubar[0]*Ka[i][2]; // Kb(i,j) - U*Ka(i,3)
                }
                else // Column 6
                {
                    return Kb[i][j] + Ubar[0]*Ka[i][1] - Ubar[1]*Ka[i][0]; // Kb(i,j) + U*Ka(i,2) - V*Ka(i,1)
                }
            }
            else
            {
                return Kb[i][j];
            }
        }

        Eigen::Matrix<double, 6, 6> get_Ls(const std::array<double, 6>& Ubar) const
        {
            Eigen::Matrix<double, 6, 6> Ls = Eigen::Matrix<double, 6, 6>::Zero();
            Ls(1,5)=Ubar[0];
            Ls(2,4)=-Ubar[0];
            Ls(0,5)=-Ubar[1];
            Ls(2,3)=Ubar[1];
            return Ls;
        }

        double get_convolution(const size_t i, const size_t j, const BodyStates& states, const std::array<double, 6>& average_velocities)
        {
            const History his = get_velocity_history_from_index(j, states);
            if(his.get_duration() >= Tmin)
            {
                // Removing the average velocity to get only the oscillation velocity
                std::function<double(double)> reverse_history = [&his, &average_velocities, j](double tau)
                    {
                        return his(tau) - average_velocities[j];
                    };
                // Integrate up to Tmax if possible, but never exceed the history length
                return builder.convolution(reverse_history, get_K(i, j, average_velocities), Tmin, std::min(Tmax, his.get_duration()));
            }
            else
            {
                return 0;
            }
        }

        ssc::kinematics::Vector6d get_convoluted_matrix_product(const BodyStates& states, const std::array<double, 6>& average_velocities)
        {
            ssc::kinematics::Vector6d ret = ssc::kinematics::Vector6d::Zero();
            for (size_t i = 0 ; i < 6 ; i++)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    ret(i) += get_convolution(i, j, states, average_velocities);
                }
            }
            return ret;
        }

        std::array<double, 6> get_average_velocities(const BodyStates& states)
        {
            std::array<double, 6> ret;
            for (size_t i = 0 ; i < 6 ; i++)
            {
                ret[i] = get_velocity_history_from_index(i, states).average(Tmax);
            }
            return ret;
        }

        Eigen::Matrix<double, 6, 1> get_oscillation_velocities(const BodyStates& states, const std::array<double, 6>& average_velocities)
        {
            Eigen::Matrix<double, 6, 1> ret;
            for (size_t i = 0 ; i < 6 ; i++)
            {
                ret(i) = get_velocity_history_from_index(i, states)() - average_velocities[i];
            }
            return ret;
        }

        History get_velocity_history_from_index(const size_t i, const BodyStates& states) const
        {
            switch(i)
            {
                case 0: return states.u;
                case 1: return states.v;
                case 2: return states.w;
                case 3: return states.p;
                case 4: return states.q;
                case 5: return states.r;
                default: return History();
            }
        }

        Wrench get_wrench(const BodyStates& states)
        {

            const ssc::kinematics::Point H(states.name,H0);
            const auto average_velocities = get_average_velocities(states);

            ssc::kinematics::Vector6d W = -get_convoluted_matrix_product(states, average_velocities);

            if (forward_speed_correction)
            {
                W += A*get_Ls(average_velocities)*get_oscillation_velocities(states, average_velocities);
            }

            return Wrench(H, states.name, W);
        }

        double get_Tmax() const
        {
            return Tmax;
        }

    private:
        Impl();
        TR1(shared_ptr)<HDBParser> hdb;
        RadiationDampingBuilder builder;
        Eigen::Matrix<double, 6, 6> A;
        std::array<std::array<std::function<double(double)>,6>, 6> Ka;
        std::array<std::array<std::function<double(double)>,6>, 6> Kb;
        std::vector<double> omega;
        std::vector<double> taus;
        size_t n;
        double Tmin;
        double Tmax;
        Eigen::Vector3d H0;
        bool forward_speed_correction;
};


RadiationDampingForceModel::RadiationDampingForceModel(const RadiationDampingForceModel::Input& input, const std::string& body_name, const EnvironmentAndFrames& env) :
        ForceModel("radiation damping", {}, body_name, env),
        pimpl(new Impl(input.hdb, input.yaml))
{
}

double RadiationDampingForceModel::get_Tmax() const
{
    return pimpl->get_Tmax();
}

Wrench RadiationDampingForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames&, const std::map<std::string,double>&) const
{
    return pimpl->get_wrench(states);
}

TypeOfQuadrature parse_type_of_quadrature_(const std::string& s);
TypeOfQuadrature parse_type_of_quadrature_(const std::string& s)
{
    if      (s == "gauss-kronrod")   return TypeOfQuadrature::GAUSS_KRONROD;
    else if (s == "rectangle")       return TypeOfQuadrature::RECTANGLE;
    else if (s == "simpson")         return TypeOfQuadrature::SIMPSON;
    else if (s == "trapezoidal")     return TypeOfQuadrature::TRAPEZOIDAL;
    else if (s == "burcher")         return TypeOfQuadrature::BURCHER;
    else if (s == "clenshaw-curtis") return TypeOfQuadrature::CLENSHAW_CURTIS;
    else if (s == "filon")           return TypeOfQuadrature::FILON;
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unkown quadrature type: " << s << ". Should be one of 'gauss-kronrod', 'rectangle', ' simpson', 'trapezoidal', 'burcher', 'clenshaw-curtis' or 'filon'.";);
    }
    return TypeOfQuadrature::FILON;
}

RadiationDampingForceModel::Input RadiationDampingForceModel::parse(const std::string& yaml, const bool parse_hdb)
{
    RadiationDampingForceModel::Input ret;
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlRadiationDamping input;
    node["hdb"] >> input.hdb_filename;
    std::string s;
    node["type of quadrature for cos transform"] >> s;
    input.type_of_quadrature_for_cos_transform = parse_type_of_quadrature_(s);
    node["type of quadrature for convolution"] >> s;
    input.type_of_quadrature_for_convolution = parse_type_of_quadrature_(s);
    node["nb of points for retardation function discretization"] >> input.nb_of_points_for_retardation_function_discretization;
    ssc::yaml_parser::parse_uv(node["omega min"], input.omega_min);
    ssc::yaml_parser::parse_uv(node["omega max"], input.omega_max);
    ssc::yaml_parser::parse_uv(node["tau min"], input.tau_min);
    ssc::yaml_parser::parse_uv(node["tau max"], input.tau_max);
    node["output Br and K"] >> input.output_Br_and_K;
    node["calculation point in body frame"] >> input.calculation_point_in_body_frame;
    if (node.FindValue("forward speed correction"))
    {
        node["forward speed correction"] >> input.forward_speed_correction;
    }
    if (parse_hdb)
    {
        const TR1(shared_ptr)<HDBParser> hdb(new HDBParser(ssc::text_file_reader::TextFileReader(std::vector<std::string>(1,input.hdb_filename)).get_contents()));
        ret.hdb = hdb;
    }
    ret.yaml = input;
    return ret;
}

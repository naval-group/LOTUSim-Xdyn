#include "PrecalParser.hpp"
#include "InvalidInputException.hpp"

PrecalParser PrecalParser::from_string(const std::string& precal_file_contents)
{
    return PrecalParser(parse_precal_from_string(precal_file_contents));
}

PrecalParser PrecalParser::from_file(const std::string& path_to_precal_file)
{
    return PrecalParser(parse_precal_from_file(path_to_precal_file));
}

PrecalParser::PrecalParser(const PrecalFile& precal_file_)
    : precal_file(precal_file_)
{
}

void convert_matrix_to_xdyn_frame(Eigen::Matrix<double, 6, 6>& Ma);
void convert_matrix_to_xdyn_frame(Eigen::Matrix<double, 6, 6>& Ma)
{
    // We need to transform the matrix into xdyn's frame (z downwards)
    /*
        *  import sympy

        m =
       sympy.Matrix([[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1]])
        R =
       sympy.Matrix([[1,0,0,0,0,0],[0,-1,0,0,0,0],[0,0,-1,0,0,0],[0,0,0,1,0,0],[0,0,0,0,-1,0],[0,0,0,0,0,-1]])
        for i in range(1,7):
            for j in range(1,7):
            exec("m"+str(i)+str(j)+" = sympy.Symbol('m" + str(i) + str(j) + "')")
        M =
       sympy.Matrix([[m11,m12,m13,m14,m15,m16],[m21,m22,m23,m24,m25,m26],[m31,m32,m33,m34,m35,m36],[m41,m42,m43,m44,m45,m46],[m51,m52,m53,m54,m55,m56],[m61,m62,m63,m64,m65,m66]])
        M
        ⎡m₁₁   -m₁₂  -m₁₃  m₁₄   -m₁₅  -m₁₆⎤
        ⎢                                  ⎥
        ⎢-m₂₁  m₂₂   m₂₃   -m₂₄  m₂₅   m₂₆ ⎥
        ⎢                                  ⎥
        ⎢-m₃₁  m₃₂   m₃₃   -m₃₄  m₃₅   m₃₆ ⎥
        ⎢                                  ⎥
        ⎢m₄₁   -m₄₂  -m₄₃  m₄₄   -m₄₅  -m₄₆⎥
        ⎢                                  ⎥
        ⎢-m₅₁  m₅₂   m₅₃   -m₅₄  m₅₅   m₅₆ ⎥
        ⎢                                  ⎥
        ⎣-m₆₁  m₆₂   m₆₃   -m₆₄  m₆₅   m₆₆ ⎦
        def f(m):
            for i in range(0,6):
                    for j in range(0,6):
                        if m[i, j]<0:
                            print("matrices[k].second["+str(i)+"]["+str(j)+"] =
       -matrices[k].second["+str(i)+"]["+str(j)+"];") f(sympy.transpose(R)*m*R)

        */

    Ma(0, 1) = -Ma(0, 1);
    Ma(0, 2) = -Ma(0, 2);
    Ma(0, 4) = -Ma(0, 4);
    Ma(0, 5) = -Ma(0, 5);
    Ma(1, 0) = -Ma(1, 0);
    Ma(1, 3) = -Ma(1, 3);
    Ma(2, 0) = -Ma(2, 0);
    Ma(2, 3) = -Ma(2, 3);
    Ma(3, 1) = -Ma(3, 1);
    Ma(3, 2) = -Ma(3, 2);
    Ma(3, 4) = -Ma(3, 4);
    Ma(3, 5) = -Ma(3, 5);
    Ma(4, 0) = -Ma(4, 0);
    Ma(4, 3) = -Ma(4, 3);
    Ma(5, 0) = -Ma(5, 0);
    Ma(5, 3) = -Ma(5, 3);
}

std::vector<double> PrecalParser::get_vector_value(const std::string& section_title,
                                                   const std::string& vector_key,
                                                   const std::string& object_name,
                                                   const std::string& not_found_message) const
{
    for (Section section : precal_file.sections)
    {
        if (section.title == section_title)
        {
            if (section.vector_values.find(vector_key) == section.vector_values.end())
            {
                std::stringstream ss;
                for (const auto kv : section.vector_values)
                {
                    ss << kv.first << ", ";
                }
                if (section.vector_values.empty())
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                          "There seems to be something wrong with PRECAL_R's output file: we did "
                          "indeed find section '" << section_title << "' in PRECAL_R's output "
                          "file, but it didn't contain any vectors (in particular, we were not "
                          "able to find the list '" << vector_key << "' which contains the "
                            << object_name << ").");
                }
                else
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                          "There seems to be something wrong with PRECAL_R's output file: we did "
                          "indeed find section '" << section_title << "' in PRECAL_R's output "
                          "file, but we were not able to find the list '" << vector_key << "' "
                          "which contains the " << object_name << ". We found keys " << ss.str()
                            << " but none of them matched.");
                }
            }
            const std::vector<double> values = section.vector_values[vector_key];
            return values;
        }
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException,
        "Unable to find section '" << section_title << "' in PRECAL_R's output file. "
        << not_found_message);
}

Eigen::Matrix<double, 6, 6> PrecalParser::get_added_mass() const
{
    Eigen::Matrix<double, 6, 6> Ma;
    const std::vector<double> values = get_vector_value(
        "added_mass_damping_matrix_inf_freq",
        "total_added_mass_matrix_inf_freq_U1_mu1",
        "matrix's coefficients",
        "Perhaps you didn't set the boolean key 'calcAmasDampCoefInfFreq' to true in PRECAL_R's "
        "input file (section sim > parHYD > calcAmasDampCoefInfFreq)? Cf. PRECAL_R's Theory "
        "Manual, sections 2.3 & 2.4 and PRECAL_R's User Manual, section 3.3.2 p.25."
        );
    for (size_t i = 0; i < 6; ++i)
    {
        for (size_t j = 0; j < 6; ++j)
        {
            Ma(i, j) = values.at(6 * i + j);
        }
    }
    convert_matrix_to_xdyn_frame(Ma);
    return Ma;
}

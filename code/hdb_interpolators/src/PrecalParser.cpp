
#include "PrecalParser.hpp"
#include "InvalidInputException.hpp"
#include <algorithm>
#include <list>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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
    , diffraction_module()
    , diffraction_phase()
{
    init_diffraction_tables();
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

std::string PrecalParser::get_string_value(const std::string& section_title,
                                           const std::string& string_key,
                                           const std::string& object_name,
                                           const std::string& not_found_message) const
{
    for (Section section : precal_file.sections)
    {
        if (section.title == section_title)
        {
            if (section.string_values.find(string_key) == section.string_values.end())
            {
                std::stringstream ss;
                for (const auto kv : section.string_values)
                {
                    ss << kv.first << ", ";
                }
                if (section.string_values.empty())
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                          "There seems to be something wrong with PRECAL_R's output file: we did "
                          "indeed find section '" << section_title << "' in PRECAL_R's output "
                          "file, but it didn't contain any strings (in particular, we were not "
                          "able to find the string '" << string_key << "' which contains the "
                            << object_name << ").");
                }
                else
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                          "There seems to be something wrong with PRECAL_R's output file: we did "
                          "indeed find section '" << section_title << "' in PRECAL_R's output "
                          "file, but we were not able to find the list '" << string_key << "' "
                          "which contains the " << object_name << ". We found keys " << ss.str()
                            << " but none of them matched.");
                }
            }
            const std::string value = section.string_values[string_key];
            return value;
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
        "added_mass_damping_matrix_inf_freq", "total_added_mass_matrix_inf_freq_U1_mu1",
        "matrix's coefficients",
        "Perhaps you didn't set the boolean key 'calcAmasDampCoefInfFreq' to true in PRECAL_R's "
        "input file (section sim > parHYD > calcAmasDampCoefInfFreq)? Cf. PRECAL_R's Theory "
        "Manual, sections 2.3 & 2.4 and PRECAL_R's User Manual, section 3.3.2 p.25.");
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

std::function<bool(double, double)> double_eq
    = [](double d1, double d2) { return fabs(d1 - d2) <= std::numeric_limits<double>::epsilon(); };

bool rao_is_valid_and_corresponds_to_signal_and_direction(const RAO& rao,
                                                          const std::string& signal_name,
                                                          const double& direction_in_degrees,
                                                          const size_t& input_frequencies_size);
bool rao_is_valid_and_corresponds_to_signal_and_direction(const RAO& rao,
                                                          const std::string& signal_name,
                                                          const double& direction_in_degrees,
                                                          const size_t& input_frequencies_size)
{
    if (rao.attributes.name.compare(signal_name) == 0
        && double_eq(rao.attributes.mu, direction_in_degrees))
    {
        if (rao.left_column.size() != input_frequencies_size)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "In PRECAL_R's output file, there isn't the expected number of amplitudes "
                  "in the rao '" << signal_name << "' for direction '" << direction_in_degrees
                  << "'. Expected " << input_frequencies_size << " values (which is the number "
                  "of wave frequencies, as defined in 'Dimensions'>'waveFreq'), but there are "
                  << rao.left_column.size() << " values.");
        }
        if (rao.right_column.size() != input_frequencies_size)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "In PRECAL_R's output file, there isn't the expected number of phases "
                  "in the rao '" << signal_name << "' for direction '" << direction_in_degrees
                  << "'. Expected " << input_frequencies_size << " values (which is the number "
                  "of wave frequencies, as defined in 'Dimensions'>'waveFreq'), but there are "
                  << rao.right_column.size() << " values.");
        }
        return true;
    }
    return false;
}

void PrecalParser::init_diffraction_tables()
{
    RAOData modules;
    RAOData phases;
    try
    {
        // Get the frequencies and directions values for which RAOs will be specified
        const std::vector<double> input_frequencies
            = get_vector_value("Dimensions", "waveFreq", "wave frequencies", "");
        const std::string frequencies_unit
            = get_string_value("Dimensions", "unitWaveFreq", "wave frequencies unit", "");

        const std::vector<double> input_directions
            = get_vector_value("Dimensions", "waveDir", "wave directions", "");
        const std::string directions_unit
            = get_string_value("Dimensions", "unitWaveDir", "wave directions unit", "");

        // Sort frequencies and psi values for which RAOs will be specified
        std::vector<std::pair<size_t, double> > frequencies;
        size_t i = 0;
        std::transform(input_frequencies.begin(), input_frequencies.end(),
                       std::back_inserter(frequencies),
                       [&i](const double omega) { return std::make_pair(i++, omega); });
        std::sort(
            frequencies.begin(), frequencies.end(),
            [](const std::pair<size_t, double>& left, const std::pair<size_t, double>& right) {
                return left.second >= right.second;
            });

        std::list<double> sorted_directions(input_directions.begin(), input_directions.end());
        sorted_directions.sort();
        std::vector<double> directions(sorted_directions.begin(), sorted_directions.end());

        // Convert period and psi values to S.I. units
        if (frequencies_unit == "rad/s")
        {
            for (size_t period_idx = 0; period_idx < frequencies.size(); ++period_idx)
            {
                const double period = 2 * PI / frequencies.at(period_idx).second;
                modules.periods.push_back(period);
                phases.periods.push_back(period);
            }
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "Unknown unit '" << frequencies_unit
                                   << "' for wave frequencies in PRECAL_R's output file. "
                                      "Known units: 'rad/s'.");
        }

        if (directions_unit == "deg")
        {
            for (size_t psi_idx = 0; psi_idx < directions.size(); ++psi_idx)
            {
                const double psi = directions.at(psi_idx) * PI / 180.;
                modules.psi.push_back(psi);
                phases.psi.push_back(psi);
            }
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                  "Unknown unit '" << directions_unit
                                   << "' for wave directions in PRECAL_R's output file. "
                                      "Known units: 'deg'.");
        }

        // Initialize the RAOs coefficients with 0s
        modules.values.fill(std::vector<std::vector<double> >(
            frequencies.size(), std::vector<double>(directions.size(), 0)));
        phases.values.fill(std::vector<std::vector<double> >(
            frequencies.size(), std::vector<double>(directions.size(), 0)));

        // Read the RAOs for each mode and direction.
        for (size_t psi_idx = 0; psi_idx < directions.size(); ++psi_idx)
        {
            for (size_t mode_idx = 0; mode_idx < 6; ++mode_idx)
            {
                const std::string signal_name = "F_dif_m" + std::to_string(mode_idx + 1);
                bool found_rao = false;

                for (RAO rao : precal_file.raos)
                {
                    if (rao_is_valid_and_corresponds_to_signal_and_direction(
                            rao, signal_name, directions.at(psi_idx), input_frequencies.size()))
                    {
                        found_rao = true;
                        if (rao.attributes.amplitude_unit != "kN/m"
                            && rao.attributes.amplitude_unit != "kN.m/m")
                        {
                            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                                "Unknown unit '" << rao.attributes.amplitude_unit << "' for diffraction RAO "
                                "amplitudes in PRECAL_R's output file. Known units: 'kN/m', 'kN.m/m'.");
                        }
                        if (rao.attributes.phase_unit != "deg")
                        {
                            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                                "Unknown unit '" << rao.attributes.phase_unit << "' for diffraction RAO phases "
                                "in PRECAL_R's output file. Known units: 'deg'.");
                        }
                        for (size_t period_idx = 0; period_idx < frequencies.size(); ++period_idx)
                        {
                            const size_t period_idx_in_input_file
                                = frequencies.at(period_idx).first;
                            modules.values.at(mode_idx).at(period_idx).at(psi_idx)
                                = rao.left_column.at(period_idx_in_input_file) * 1e3;
                            phases.values.at(mode_idx).at(period_idx).at(psi_idx)
                                = rao.right_column.at(period_idx_in_input_file) * PI / 180.;
                        }
                        break;
                    }
                }
                if (not(found_rao))
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                        "Unable to find rao '" << signal_name << "' for direction '"
                        << directions.at(psi_idx) << "' in PRECAL_R's output file. "
                        "Perhaps you didn't set the boolean key 'expDifWaveFrc' to true "
                        "in PRECAL_R's input file (section Export > expDifWaveFrc)?");
                }
            }
        }

        diffraction_module = modules;
        diffraction_phase = phases;
    }
    catch (const InvalidInputException& e)
    {
        diffraction_module = e.what();
        diffraction_phase = e.what();
    }
}

std::array<std::vector<std::vector<double> >, 6> PrecalParser::get_diffraction_module_tables() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_module);
    return ret->values;
}

std::array<std::vector<std::vector<double> >, 6> PrecalParser::get_diffraction_phase_tables() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
    return ret->values;
}

std::vector<double> PrecalParser::get_diffraction_module_periods() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_module);
    return ret->periods;
}

std::vector<double> PrecalParser::get_diffraction_phase_periods() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
    return ret->periods;
}

std::vector<double> PrecalParser::get_diffraction_module_psis() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_module);
    return ret->psi;
}

std::vector<double> PrecalParser::get_diffraction_phase_psis() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
    return ret->psi;
}

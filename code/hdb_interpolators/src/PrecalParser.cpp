
#include "PrecalParser.hpp"
#include "InvalidInputException.hpp"
#include <algorithm>
#include <list>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#include <ssc/decode_unit.hpp>
#define DEG2RAD (PI/180.)

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
    , froude_krylov_module()
    , froude_krylov_phase()
{
    init_diffraction_tables();
    init_froude_krylov_tables();
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

void PrecalParser::check_unit(const std::string& section_title, const std::string& vector_key, const std::string& object_name, const std::string& expected_unit) const
{
    const std::string actual_unit
        = get_string_value(section_title, vector_key, object_name, "");
    if (actual_unit != expected_unit)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "Unknown unit '" << actual_unit
                                << "' for " << section_title << ">" <<  vector_key << ">" << object_name << " in PRECAL_R's output file. "
                                    "Expected " << expected_unit);
    }
}

std::vector<std::pair<size_t, double> > PrecalParser::get_sorted_indexed_frequencies() const
{
    const std::vector<double> input_frequencies
        = get_vector_value("Dimensions", "waveFreq", "wave frequencies", "");
    check_unit("Dimensions", "unitWaveFreq", "wave frequencies unit", "rad/s");
    // Sort frequencies and directions values for which RAOs will be specified
    std::vector<std::pair<size_t, double> > frequencies;
    size_t i = 0;
    std::transform(input_frequencies.begin(), input_frequencies.end(),
                    std::back_inserter(frequencies),
                    [&i](const double omega) { return std::make_pair(i++, omega); });
    std::sort(
        frequencies.begin(), frequencies.end(),
        [](const std::pair<size_t, double>& left, const std::pair<size_t, double>& right) {
            // the periods should be sorted in ascendant order, so the frequencies should be sorted in decreasing order.
            return left.second >= right.second;
        });
    return frequencies;
}

std::vector<double> PrecalParser::get_sorted_directions() const
{
    const std::vector<double> input_directions
        = get_vector_value("Dimensions", "waveDir", "wave directions", "");
    check_unit("Dimensions", "unitWaveDir", "wave directions unit", "deg");
    std::list<double> sorted_directions(input_directions.begin(), input_directions.end());
    sorted_directions.sort();
    return std::vector<double>(sorted_directions.begin(), sorted_directions.end());
}

RAO PrecalParser::find_rao(const std::string& signal_name, const std::string& path_to_boolean_parameter, const double direction, const size_t nb_of_frequencies) const
{
    for (RAO rao : precal_file.raos)
    {
        if (rao_is_valid_and_corresponds_to_signal_and_direction(
                rao, signal_name, direction, nb_of_frequencies))
        {
            return rao;
        }
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException,
                    "Unable to find RAO '" << signal_name << "' for direction '"
                    << direction << "' in PRECAL_R's output file. "
                    "Perhaps you didn't set the boolean key '" << path_to_boolean_parameter << "' to true "
                    "in PRECAL_R's input file?");
    return RAO();
}

void PrecalParser::fill_periods_directions_and_values(RAOData& rao, const std::vector<std::pair<size_t, double> >& frequencies, const std::vector<double>& directions) const
{
    // Insert sorted periods in modules and phases vectors
    for (size_t frequency_idx = 0; frequency_idx < frequencies.size(); ++frequency_idx)
    {
        const double period = 2 * PI / frequencies.at(frequency_idx).second;
        rao.periods.push_back(period);
    }

    // Insert sorted directions in modules and phases vectors (converting them to S.I. units)
    for (size_t psi_idx = 0; psi_idx < directions.size(); ++psi_idx)
    {
        const double psi = directions.at(psi_idx) * DEG2RAD;
        rao.psi.push_back(psi);
    }
    // Initialize the RAOs coefficients with 0s
    rao.values.fill(std::vector<std::vector<double> >(
        frequencies.size(), std::vector<double>(directions.size(), 0)));
}

void PrecalParser::fill_module_values(RAOData& ret, const RAO& rao, const std::vector<std::pair<size_t, double> >& frequencies, const size_t mode_idx, const size_t psi_idx) const
{
    for (size_t frequency_idx = 0; frequency_idx < frequencies.size(); ++frequency_idx)
    {
        const size_t frequency_idx_in_input_file
            = frequencies.at(frequency_idx).first;
        ret.values.at(mode_idx).at(frequency_idx).at(psi_idx)
            = rao.left_column.at(frequency_idx_in_input_file) * 1e3;
    }
}

void PrecalParser::fill_phase_values(RAOData& ret, const RAO& rao, const std::vector<std::pair<size_t, double> >& frequencies, const size_t mode_idx, const size_t psi_idx) const
{
    for (size_t frequency_idx = 0; frequency_idx < frequencies.size(); ++frequency_idx)
    {
        const size_t frequency_idx_in_input_file
            = frequencies.at(frequency_idx).first;
        ret.values.at(mode_idx).at(frequency_idx).at(psi_idx)
            = rao.right_column.at(frequency_idx_in_input_file) * DEG2RAD;
    }
}

void check_units(const std::string& pretty_name, const std::string& actual_unit, const std::vector<std::string>& expected_units);
void check_units(const std::string& pretty_name, const std::string& actual_unit, const std::vector<std::string>& expected_units)
{
    std::stringstream ss;
    for (const auto expected_unit:expected_units)
    {
        if (actual_unit == expected_unit)
        {
            return;
        }
        ss << " " << expected_unit;
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException,
        "Unknown unit '" << actual_unit << "' for " << pretty_name << " RAO "
        "in PRECAL_R's output file. Known units:" << ss.str() << ".");
}

RAOData PrecalParser::retrieve_module_tables(const std::string& signal_basename, const std::string& pretty_name, const std::string& path_to_boolean_parameter) const
{
    RAOData ret;
    // Get the frequencies and directions values for which RAOs will be specified
    const auto directions = get_sorted_directions();
    const auto frequencies = get_sorted_indexed_frequencies();
    fill_periods_directions_and_values(ret, frequencies, directions);
    // Read the RAOs for each direction.
    for (size_t psi_idx = 0; psi_idx < directions.size(); ++psi_idx)
    {
        for (size_t mode_idx = 0; mode_idx < 6; ++mode_idx)
        {
            const std::string signal_name = signal_basename + std::to_string(mode_idx + 1);
            const RAO rao = find_rao(signal_name, path_to_boolean_parameter, directions.at(psi_idx), frequencies.size());
            fill_module_values(ret, rao, frequencies, mode_idx, psi_idx);
            check_units(pretty_name, rao.attributes.amplitude_unit, {"kN/m", "kN.m/m"});
        }
    }
    return ret;
}

RAOData PrecalParser::retrieve_phase_tables(const std::string& signal_basename, const std::string& pretty_name, const std::string& path_to_boolean_parameter) const
{
    RAOData ret;
    // Get the frequencies and directions values for which RAOs will be specified
    const auto directions = get_sorted_directions();
    const auto frequencies = get_sorted_indexed_frequencies();
    fill_periods_directions_and_values(ret, frequencies, directions);
    // Read the RAOs for each mode and direction.
    for (size_t psi_idx = 0; psi_idx < directions.size(); ++psi_idx)
    {
        for (size_t mode_idx = 0; mode_idx < 6; ++mode_idx)
        {
            const std::string signal_name = signal_basename + std::to_string(mode_idx + 1);
            const RAO rao = find_rao(signal_name, path_to_boolean_parameter, directions.at(psi_idx), frequencies.size());
            check_units(pretty_name, rao.attributes.phase_unit, {"deg"});
            fill_phase_values(ret, rao, frequencies, mode_idx, psi_idx);
        }
    }
    return ret;
}

void PrecalParser::init_diffraction_tables()
{
    try
    {
        diffraction_module = retrieve_module_tables("F_dif_m", "diffraction", "Export > expDifWaveFrc");
        diffraction_phase = retrieve_phase_tables("F_dif_m", "diffraction", "Export > expDifWaveFrc");
    }
    catch (const InvalidInputException& e)
    {
        diffraction_module = e.what();
        diffraction_phase = e.what();
    }
}

void PrecalParser::init_froude_krylov_tables()
{
    try
    {
        froude_krylov_module = retrieve_module_tables("F_inc_m", "Froude-Krylov", "sim > parRES > expIncWaveFrc");;
        froude_krylov_phase = retrieve_phase_tables("F_inc_m", "Froude-Krylov", "sim > parRES > expIncWaveFrc");;
    }
    catch (const InvalidInputException& e)
    {
        froude_krylov_module = e.what();
        froude_krylov_phase = e.what();
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

std::array<std::vector<std::vector<double> >, 6> PrecalParser::get_froude_krylov_module_tables() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
    return ret->values;
}

std::array<std::vector<std::vector<double> >, 6> PrecalParser::get_froude_krylov_phase_tables() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
    return ret->values;
}

std::vector<double> PrecalParser::get_froude_krylov_module_periods() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
    return ret->periods;
}

std::vector<double> PrecalParser::get_froude_krylov_phase_periods() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
    return ret->periods;
}

std::vector<double> PrecalParser::get_froude_krylov_module_psis() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
    return ret->psi;
}

std::vector<double> PrecalParser::get_froude_krylov_phase_psis() const
{
    if (std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
    return ret->psi;
}

std::vector<double> PrecalParser::get_angular_frequencies() const
{
    for (const auto section : precal_file.sections)
    {
        const auto it = section.vector_values.find("waveFreq");
        if (it != section.vector_values.end())
        {
            return it->second;
        }
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find value 'waveFreq' in PRECAL_R's input file.");
    return std::vector<double>();
}

double PrecalParser::get_forward_speed() const
{
    for (const auto section : precal_file.sections)
    {
        const auto it = section.vector_values.find("shipSpeed");
        if (it != section.vector_values.end())
        {
            if (it->second.empty())
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "We found vector 'shipSpeed' in PRECAL_R's input file but it was empty.");
            }
            const auto it2 = section.string_values.find("unitShipSpeed");
            if (it2 == section.string_values.end())
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find value 'unitShipSpeed' in the same section as 'shipSpeed' in PRECAL_R's input file.");
            }
            // decode_unit does not understand "kn" (which kind of defeats the purpose of using it)
            const double factor = it2->second == "kn" ? ssc::decode_unit::decodeUnit("kt") : ssc::decode_unit::decodeUnit(it2->second);
            return it->second.front()*factor;
        }
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find value 'waveFreq' in PRECAL_R's input file.");
    return std::nan("");
}

std::string coeff_name(const std::string& prefix, const size_t i , const size_t j);
std::string coeff_name(const std::string& prefix, const size_t i , const size_t j)
{
    return prefix + "_m" + std::to_string(i+1) + "m" + std::to_string(j+1);
}

std::vector<double> PrecalParser::extract_matrix_coeff(const std::string& short_name, const std::string& long_name, const size_t i, const size_t j) const
{
    bool found_signal = false;
    double min_speed = 1E300;
    std::string line;
    for (const auto rao : precal_file.raos)
    {
        if (rao.attributes.name == coeff_name(short_name, i, j))
        {
            found_signal = true;
            if (rao.attributes.U == 0)
            {
                return rao.left_column;
            }
            else
            {
                if (min_speed > rao.attributes.U)
                {
                    line = rao.title_line;
                    min_speed = rao.attributes.U;
                }
            }
        }
    }
    if (not(found_signal))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find " << long_name << " coefficient " << coeff_name(short_name, i, j) << " in PRECAL_R's output file. Check the value of the XML node sim > parRES > expAmasDampCoef is set to true/1 in PRECAL_R's input file.");
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "We found " << long_name << " coefficient " << coeff_name(short_name, i, j) << " in PRECAL_R's output file but it is calculated at non-zero velocity (the minimum velocity we found was " << min_speed << "). You can set this list in PRECAL_R's input file, XML node sim > parHYD > shipSpeedInp.");
    }
    return std::vector<double>();
}

std::vector<double> PrecalParser::get_added_mass_coeff(const size_t i, const size_t j) const
{
    return extract_matrix_coeff("A", "added mass", i, j);
}

std::vector<double> PrecalParser::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    return extract_matrix_coeff("B", "damping matrix", i, j);
}

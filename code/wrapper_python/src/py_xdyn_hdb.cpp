#include "py_xdyn_hdb.hpp"
#include "py_pybind_additions.hpp"
#include "force_models/unit_tests/inc/HDBParserForTests.hpp"
#include "hdb_interpolators/inc/History.hpp"
#include "hdb_interpolators/inc/HydroDBParser.hpp"
#include "hdb_interpolators/inc/HDBParser.hpp"
#include "hdb_interpolators/inc/PrecalParser.hpp"
#include "hdb_interpolators/inc/RadiationDampingBuilder.hpp"
#include "hdb_interpolators/inc/RaoInterpolator.hpp"
#include "hdb_interpolators/inc/TimestampedMatrix.hpp"

namespace py = pybind11;


HDBParserForTests::HDBParserForTests(const std::vector<double>& omega_, const std::vector<double>& Ma_, const std::vector<double>& Br_, const bool only_diagonal_terms_)
: omega(omega_)
, Ma(Ma_), Br(Br_), only_diagonal_terms(only_diagonal_terms_)
{
}

std::vector<double> HDBParserForTests::get_angular_frequencies() const
{
    return omega;
}

std::vector<double> HDBParserForTests::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    if (only_diagonal_terms)
    {
        if (i == j)
        {
            return Br;
        }
    }
    else
    {
        std::vector<double> ret;
        ret.reserve(Br.size());
        for (const auto& br:Br)
        {
            ret.push_back((double)(10*(i+1)+(j+1))*br);
        }
        return ret;
    }
    return std::vector<double>(Br.size(),0);
}

std::vector<double> HDBParserForTests::get_added_mass_coeff(const size_t i, const size_t j) const
{
    if (only_diagonal_terms)
    {
        if (i == j)
        {
            return Ma;
        }
    }
    else
    {
        std::vector<double> ret;
        ret.reserve(Ma.size());
        for (const auto& ma:Ma)
        {
            ret.push_back((double)(10*(i+1)+(j+1))*ma);
        }
        return ret;
    }
    return std::vector<double>(Ma.size(),0);
}

Eigen::Matrix<double, 6, 6> HDBParserForTests::get_added_mass() const
{
    Eigen::Matrix<double, 6, 6> ret;
    for (size_t i = 0 ; i < 6 ; i++)
    {
        for(size_t j = 0 ; j < 6 ; j++)
        {
            ret(i, j) = get_added_mass_coeff(i, j).back();
        }
    }
    return ret;
}

void py_add_module_xdyn_hdb(py::module&m)
{
    py::module m_hdb_interpolators = m.def_submodule("hdb");

    py::class_<RAOData>(m_hdb_interpolators, "RAOData")
        .def(py::init<>())
        .def_readwrite("periods", &RAOData::periods)
        .def_readwrite("psi", &RAOData::psi)
        .def_readwrite("values", &RAOData::values);

    py::class_<HydroDBParser>(m_hdb_interpolators, "HydroDBParser");

    py::class_<HDBParser, HydroDBParser>(m_hdb_interpolators, "HDBParser")
        .def_static("from_file", &HDBParser::from_file, py::arg("filename"))
        .def_static("from_string", &HDBParser::from_string, py::arg("file_content"))
        .def("get_forward_speed", &HDBParser::get_forward_speed, "Speed at which the radiation damping matrices were calculated. Used to determine if we can apply a forward-speed correction")
        .def("get_added_mass_array", &HDBParser::get_added_mass_array)
        .def("get_radiation_damping_array", &HDBParser::get_radiation_damping_array)
        .def("get_diffraction_module", &HDBParser::get_diffraction_module)
        .def("get_diffraction_phase", &HDBParser::get_diffraction_phase)
        .def("get_added_mass",
            static_cast<Eigen::Matrix<double,6,6> (HDBParser::*)() const>(&HDBParser::get_added_mass))
        .def("get_added_mass",
            static_cast<Eigen::Matrix<double,6,6> (HDBParser::*)(const double Tp) const>(&HDBParser::get_added_mass),
            py::arg("Tp"), "Period at which to interpolate the added mass")
        .def("get_angular_frequencies", &HDBParser::get_angular_frequencies)
        .def("get_added_mass_coeff", &HDBParser::get_added_mass_coeff, py::arg("i"), py::arg("j"))
        .def("get_radiation_damping_coeff", &HDBParser::get_radiation_damping_coeff, py::arg("i"), py::arg("j"))
        .def("get_diffraction_module_tables", &HDBParser::get_diffraction_module_tables)
        .def("get_diffraction_phase_tables", &HDBParser::get_diffraction_phase_tables)
        .def("get_froude_krylov_module_tables", &HDBParser::get_froude_krylov_module_tables)
        .def("get_froude_krylov_phase_tables", &HDBParser::get_froude_krylov_phase_tables)
        .def("get_total_excitation_force_module_tables", &HDBParser::get_total_excitation_force_module_tables)
        .def("get_total_excitation_force_phase_tables", &HDBParser::get_total_excitation_force_phase_tables)
        .def("get_diffraction_phase_psis", &HDBParser::get_diffraction_phase_psis)
        .def("get_diffraction_phase_periods", &HDBParser::get_diffraction_phase_periods)
        .def("get_diffraction_module_psis", &HDBParser::get_diffraction_module_psis)
        .def("get_diffraction_module_periods", &HDBParser::get_diffraction_module_periods)
        .def("get_total_excitation_force_phase_psis", &HDBParser::get_total_excitation_force_phase_psis)
        .def("get_total_excitation_force_phase_periods", &HDBParser::get_total_excitation_force_phase_periods)
        .def("get_total_excitation_force_module_psis", &HDBParser::get_total_excitation_force_module_psis)
        .def("get_total_excitation_force_module_periods", &HDBParser::get_total_excitation_force_module_periods)
        .def("get_froude_krylov_phase_psis", &HDBParser::get_froude_krylov_phase_psis)
        .def("get_froude_krylov_phase_periods", &HDBParser::get_froude_krylov_phase_periods)
        .def("get_froude_krylov_module_psis", &HDBParser::get_froude_krylov_module_psis)
        .def("get_froude_krylov_module_periods", &HDBParser::get_froude_krylov_module_periods)
        .def("get_wave_drift_tables", &HDBParser::get_wave_drift_tables, "Get the wave drift forces (by angular frequency a and incidence i M[a][i]), in N/(m².s).")
        .def("get_wave_drift_psis", &HDBParser::get_wave_drift_psis, "Get the incidences at which the wave drift forces are expressed (in rad).")
        .def("get_wave_drift_periods", &HDBParser::get_wave_drift_periods, "Periods at which the wave drift forces are expressed (in seconds).")
        ;

    py::class_<PrecalParser, HydroDBParser>(m_hdb_interpolators, "PrecalParser")
        .def_static("from_file", &PrecalParser::from_file)
        .def_static("from_string", &PrecalParser::from_string)
        .def("get_vector_value", &PrecalParser::get_vector_value, py::arg("section_title"),py::arg("vector_key"), py::arg("object_name"),py::arg("not_found_message"))
        .def("get_string_value", &PrecalParser::get_string_value, py::arg("section_title"), py::arg("string_key"), py::arg("object_name"), py::arg("not_found_message"))
        .def("get_added_mass", &PrecalParser::get_added_mass)
        .def("get_diffraction_module_tables", &PrecalParser::get_diffraction_module_tables)
        .def("get_diffraction_phase_tables", &PrecalParser::get_diffraction_phase_tables)
        .def("get_diffraction_module_periods", &PrecalParser::get_diffraction_module_periods)
        .def("get_diffraction_phase_periods", &PrecalParser::get_diffraction_phase_periods)
        .def("get_diffraction_module_psis", &PrecalParser::get_diffraction_module_psis)
        .def("get_diffraction_phase_psis", &PrecalParser::get_diffraction_phase_psis)
        .def("get_froude_krylov_module_tables", &PrecalParser::get_froude_krylov_module_tables)
        .def("get_froude_krylov_phase_tables", &PrecalParser::get_froude_krylov_phase_tables)
        .def("get_froude_krylov_phase_psis", &PrecalParser::get_froude_krylov_phase_psis)
        .def("get_froude_krylov_phase_periods", &PrecalParser::get_froude_krylov_phase_periods)
        .def("get_froude_krylov_module_psis", &PrecalParser::get_froude_krylov_module_psis)
        .def("get_froude_krylov_module_periods", &PrecalParser::get_froude_krylov_module_periods)
        .def("get_angular_frequencies", &PrecalParser::get_angular_frequencies, "Get the angular frequencies for the radiation damping and added mass matrices")
        .def("get_forward_speed", &PrecalParser::get_forward_speed,
         R"(Speed at which the radiation damping matrices were calculated.
            Used to determine if we can apply a forward-speed correction
            to the radiation damping force model..)")
        .def("get_added_mass_coeff", &PrecalParser::get_added_mass_coeff, py::arg("i"), py::arg("j"), "Added mass matrix coefficient Ma(i,j) = f(omega) for all omegas")
        .def("get_radiation_damping_coeff", &PrecalParser::get_radiation_damping_coeff, py::arg("i"), py::arg("j"), "Radiation damping matrix coefficient Br(i,j) = f(omega) for all omegas")
        .def("get_wave_drift_tables", &PrecalParser::get_wave_drift_tables, "Get the wave drift forces (by angular frequency a and incidence i M[a][i]), in N/(m².s).")
        .def("get_wave_drift_psis", &PrecalParser::get_wave_drift_psis, "Get the incidences at which the wave drift forces are expressed (in rad).")
        .def("get_wave_drift_periods", &PrecalParser::get_wave_drift_periods, "Periods at which the wave drift forces are expressed (in seconds).")
        ;

    py::class_<RaoInterpolator>(m_hdb_interpolators, "RaoInterpolator")
        .def(py::init<const HydroDBParser& /*data*/,
                      const std::vector<double>& /*omega*/,
                      const std::vector<double>& /*psi*/,
                      const YamlRAO& /*diffraction_yaml*/>(),
            py::arg("data"),
            py::arg("omega"),
            py::arg("psi"),
            py::arg("diffraction_yaml"),
            R"(
                - `data` (HydroDBParser): Data read from the HDB or Precal_R file
                - `omega` (List[float]): Angular frequencies in the wave spectrum (points at which to interpolate the HDB data)
                - `psi` (List[float]): Wave directions (points at which to interpolate the HDB data)
                - `diffraction_yaml` (YamlRAO): Contents of the force model's parsed YAML data
            )")
        .def(py::init<const HydroDBParser& /*data*/, const YamlRAO& /*diffraction_yaml*/>(),
            py::arg("data"),
            py::arg("diffraction_yaml"),
            R"(
                - `data` (HydroDBParser): Data read from the HDB or Precal_R file
                - `diffraction_yaml` (YamlRAO): Contents of the force model's parsed YAML data
            )")
        .def("get_phases_cartesian", &RaoInterpolator::get_phases_cartesian,
            py::arg("k"),
            R"(Interpolates the RAO's phase for the values of omega & psi in the wave spectrum
             Input:
             - `k` (int): Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
             Return RAO_phase[i][j], where i is frequency index & j direction index
             )")
        .def("interpolate_module", &RaoInterpolator::interpolate_module,
            py::arg("axis"),
            py::arg("Tp"),
            py::arg("beta"),
            R"(Interpolates the RAO's module for a given axis, a wave period (*not* an encounter period)
            and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
            0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
            Return (float) module of the rao for the axis, the frequency and the incidence
             )")
        .def("interpolate_phase", &RaoInterpolator::interpolate_phase,
            py::arg("axis"),
            py::arg("Tp"),
            py::arg("beta"),
            R"(Interpolates the RAO's phase for a given axis, a wave period (*not* an encounter period)
            and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
            0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
            Return (float) module of the rao for the axis, the frequency and the incidence
             )")
        .def("get_module_periods", &RaoInterpolator::get_module_periods)
        .def("get_rao_calculation_point", &RaoInterpolator::get_rao_calculation_point)
        .def("using_encounter_period", &RaoInterpolator::using_encounter_period, "Return boolean")
        ;



    /* Code below expose internal function / class that be should be kept private*/
    /* Maybe inside a submodule named _internal ?*/
    m_hdb_interpolators.def("parse_precal_from_string", &parse_precal_from_string, py::arg("input"));
    m_hdb_interpolators.def("parse_precal_from_file", &parse_precal_from_file, py::arg("filename"));
    m_hdb_interpolators.def("parse_rao_attributes", &parse_rao_attributes, py::arg("input"));

    py::class_<Section>(m_hdb_interpolators, "Section")
        .def(py::init<>())
        .def_readwrite("title", &Section::title)
        .def_readwrite("scalar_values", &Section::scalar_values)
        .def_readwrite("string_values", &Section::string_values)
        .def_readwrite("vector_values", &Section::vector_values)
        ;

    py::class_<RAO>(m_hdb_interpolators, "RAO")
        .def(py::init<>())
        .def_readwrite("title_line", &RAO::title_line)
        .def_readwrite("attributes", &RAO::attributes)
        .def_readwrite("left_column", &RAO::left_column)
        .def_readwrite("right_column", &RAO::right_column)
        ;

    py::class_<PrecalFile>(m_hdb_interpolators, "PrecalFile")
        .def(py::init<>())
        .def_readwrite("sections", &PrecalFile::sections)
        .def_readwrite("raos", &PrecalFile::raos)
        ;

    py::class_<RAOAttributes>(m_hdb_interpolators, "RAOAttributes")
        .def(py::init<>())
        .def_readwrite("name", &RAOAttributes::name)
        .def_readwrite("position", &RAOAttributes::position)
        .def_readwrite("h", &RAOAttributes::h)
        .def_readwrite("h_unit", &RAOAttributes::h_unit)
        .def_readwrite("phi_a", &RAOAttributes::phi_a) // Roll amplitude
        .def_readwrite("phi_a_unit", &RAOAttributes::phi_a_unit)
        .def_readwrite("U", &RAOAttributes::U) // Speed
        .def_readwrite("U_unit", &RAOAttributes::U_unit)
        .def_readwrite("mu", &RAOAttributes::mu) // Waves direction
        .def_readwrite("mu_unit", &RAOAttributes::mu_unit)
        .def_readwrite("amplitude_unit", &RAOAttributes::amplitude_unit)
        .def_readwrite("phase_unit", &RAOAttributes::phase_unit)
        ;

    py::class_<RadiationDampingBuilder>(m_hdb_interpolators, "RadiationDampingBuilder")
        .def(py::init<const TypeOfQuadrature&, const TypeOfQuadrature&>(),
            py::arg("type_of_quadrature_for_convolution"),
            py::arg("type_of_quadrature_for_cos_transform"),
            R"(RadiationDampingBuilder

            Inputs:

            - `type_of_quadrature_for_convolution` (TypeOfQuadrature)
            - `type_of_quadrature_for_cos_transform` (TypeOfQuadrature)
             )")
        .def("build_interpolator", &RadiationDampingBuilder::build_interpolator,
            py::arg("x"),
            py::arg("y"),
            "Build a continuous function from discrete (x,y) points (using interpolation type defined in constructor")
        .def("build_retardation_function", &RadiationDampingBuilder::build_retardation_function,
            py::arg("Br"),
            py::arg("taus"),
            py::arg("eps"),
            py::arg("omega_min"),
            py::arg("omega_ma"),
            R"(Use radiation damping function to compute retardation function

            Radiation damping can come, eg. from the build_interpolator method. This method evaluates the
            integral \f$K(\tau) := \frac{2}{\pi} int_{\omega_{\mbox{min}}}^{\omega_{\mbox{max}}} B_r(\omega)\cos(\omega\cdot \tau)d\tau\f$
            for n different values of \f$\tau\f$ between \f$\frac{2\pi}{\omega{\mbox{max}}} and \frac{2\pi}{\omega{\mbox{min}}}\f$.
            It then uses "build_interpolator" to build a function (lambda).

            Inputs:

            - `Br` (Callable[[float], float]): Radiation damping function
            - `taus` (List[float])
            - `eps` (float): When to truncate (0 for no truncation)
            - `omega_min` (float)
            - `omega_max` (float)
            )")
        .def("convolution", &RadiationDampingBuilder::convolution,
            py::arg("h"),
            py::arg("f"),
            py::arg("Tmin"),
            py::arg("Tmax"),
            R"(Computes the convolution of a function with state history, over a certain time

            Returns \f$\int_0^T h(t-\tau)*f(\tau) d\tau\f$

            Inputs:

            - `h` (Callable[[float], float]): State reverse history
            - `t` (Callable[[float], float]): Function to convolute with
            - `Tmin` (float): Beginning of the convolution (because retardation function may not be defined for T=0)
            - `Tmax` (float): End of the convolution
            )")
        .def("build_regular_intervals", &RadiationDampingBuilder::build_regular_intervals,
            py::arg("first"),
            py::arg("last"),
            py::arg("n"),
            R"(Build a vector of n regularly incremented doubles from xmin to xmax. First value is xmin last is xmax.

            Inputs:

            - `first` (float): First value in vector
            - `last` (float): Last value in vector
            - `n` (int): Number of values to return
            )")
        .def("build_exponential_intervals", &RadiationDampingBuilder::build_exponential_intervals,
            py::arg("first"),
            py::arg("last"),
            py::arg("n"),
            R"(Build a vector of n exponentially incremented doubles from xmin to xmax.

            First value is xmin last is xmax. Spacing is small near xmin and large near xmax

            Inputs:

            - `first` (float): First value in vector
            - `last` (float): Last value in vector
            - `n` (int): Number of values to return

            Return: List[float]
            )")
        .def("build_exponential_intervals_reversed", &RadiationDampingBuilder::build_exponential_intervals_reversed,
            py::arg("first"),
            py::arg("last"),
            py::arg("n"),
            R"(Build a vector of n exponentially incremented doubles from xmin to xmax.

            First value is xmin last is xmax. Spacing is large near xmin and small near xmax

            Inputs:

            - `first` (float): First value in vector
            - `last` (float): Last value in vector
            - `n` (int): Number of values to return

            Return: List[float]
            )")
        .def("find_integration_bound", &RadiationDampingBuilder::find_integration_bound,
            py::arg("f"),
            py::arg("omega_min"),
            py::arg("omega_max"),
            py::arg("eps"),
            R"(Find bound representing a significant amount of the integral

            Uses TOMS Algorithm 748 to compute the minimum bound \f$\omega_0\f$ such that
            \f$int_{\omega_{\mbox{min}}}^{\omega_0} f(\omega) d\omega = (1-eps) int_{\omega_{\mbox{min}}}^{\omega_{\mbox{max}}}\f$

            Inputs:

             - `f` (Callable[[float], float]): Function to integrate
             - `omega_min` (float): Lower bound of the integration (returned omega is necessarily greater than omega_min)
             - `omega_max` (float): Upper bound of the integration (returned omega is necessarily lower than omega_min)
             - `eps` (float): Integration error (compared to full integration from omega_min up to omega_max)

            Returns: float = Upper integration bound

            )")
        .def("find_r_bound", &RadiationDampingBuilder::find_r_bound,
            py::arg("f"),
            py::arg("omega_min"),
            py::arg("omega_max"),
            py::arg("eps"),
            R"(Find r bound

            Inputs:

             - `f` (Callable[[float], float]): Function to integrate
             - `omega_min` (float): Lower bound of the integration (returned omega is necessarily greater than omega_min)
             - `omega_max` (float): Upper bound of the integration (returned omega is necessarily lower than omega_min)
             - `eps` (float): Integration error (compared to full integration from omega_min up to omega_max)

            Returns: float = Upper integration bound

            )")
        .def("cos_transform", &RadiationDampingBuilder::cos_transform,
            py::arg("Br"),
            py::arg("omega_min"),
            py::arg("omega_max"),
            py::arg("tau"),
            R"(Cos transform

            Inputs:

             - `Br` (Callable[[float], float]): Function to integrate
             - `omega_min` (float): Lower bound of the integration (returned omega is necessarily greater than omega_min)
             - `omega_max` (float): Upper bound of the integration (returned omega is necessarily lower than omega_min)
             - `tau` (float):

            Returns: float
            )")
        ;

    py::class_<HDBParserForTests, HDBParser>(m_hdb_interpolators, "HDBParserForTests")
        .def(py::init<const std::vector<double>& /*omega*/, const std::vector<double>& /*Ma*/, const std::vector<double>& /*Br*/, const bool /*only_diagonal_terms_*/>(),
            py::arg("omega"),
            py::arg("Ma"),
            py::arg("Br"),
            py::arg("only_diagonal_terms")=false)
        .def("get_angular_frequencies", &HDBParserForTests::get_angular_frequencies)
        .def("get_radiation_damping_coeff", &HDBParserForTests::get_radiation_damping_coeff)
        .def("get_added_mass_coeff", &HDBParserForTests::get_added_mass_coeff)
        .def("get_added_mass", &HDBParserForTests::get_added_mass)
        ;
}

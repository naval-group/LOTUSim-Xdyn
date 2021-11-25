#include "py_xdyn_hdb.hpp"
#include "py_pybind_additions.hpp"
#include "hdb_interpolators/inc/History.hpp"
#include "hdb_interpolators/inc/HydroDBParser.hpp"
#include "hdb_interpolators/inc/HDBParser.hpp"
#include "hdb_interpolators/inc/PrecalParser.hpp"
#include "hdb_interpolators/inc/RaoInterpolator.hpp"
#include "hdb_interpolators/inc/TimestampedMatrix.hpp"

namespace py = pybind11;

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
        .def_static("from_file", &HDBParser::from_file)
        .def_static("from_string", &HDBParser::from_string)
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
        .def("get_modules_cartesian", &RaoInterpolator::get_modules_cartesian,
            py::arg("k"),
            R"(Interpolates the RAO's module for the values of omega & psi in the wave spectrum
             Input:
             - `k` (int): Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
             Return RAO_module[i][j], where i is frequency index & j direction index
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
}
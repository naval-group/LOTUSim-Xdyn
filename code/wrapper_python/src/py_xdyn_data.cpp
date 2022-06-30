#include "py_xdyn_data.hpp"
#include "py_pybind_additions.hpp"
#include "test_data_generator/inc/TriMeshTestData.hpp"
#include "test_data_generator/inc/hdb_data.hpp"
#include "test_data_generator/inc/yaml_data.hpp"
#include "test_data_generator/inc/precal_test_data.hpp"
#include "core/inc/Body.hpp"
#include "core/inc/BodyBuilder.hpp"
#include <string>

// Duplicated code from core/unit_tests and executables/src/test_hs.cpp
BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points);
BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points)
{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points, 0, 0.0, rot, 0.0, false);
}

void py_add_module_xdyn_data_body(py::module& m);
void py_add_module_xdyn_data_body(py::module& m)
{
    m.def("get_body", &get_body,
        py::arg("name"),
        py::arg("points") = two_triangles()
        );

}

void py_add_module_xdyn_data_mesh(py::module& m);
void py_add_module_xdyn_data_mesh(py::module& m)
{

    m.def("one_triangle",&one_triangle);
    m.def("one_triangle_clockwise",&one_triangle_clockwise);
    m.def("degenerated_triangle",&degenerated_triangle);
    m.def("unit_cube",&unit_cube);
    m.def("unit_cube_with_incorrect_orientation",&unit_cube_with_incorrect_orientation);
    m.def("unit_cube_clockwise",&unit_cube_clockwise);
    m.def("two_triangles",&two_triangles);
    m.def("two_triangles_immerged",&two_triangles_immerged);
    m.def("trapezium",&trapezium);
    m.def("n_gone",&n_gone);
    m.def("cube",&cube);
    m.def("tetrahedron",&tetrahedron);
    m.def("tetrahedron_clockwise",&tetrahedron_clockwise);
    m.def("generated_stl",&generated_stl);
    m.def("L",&L);
    m.def("U",&U);
}

void py_add_module_xdyn_data_yaml(py::module& m);
void py_add_module_xdyn_data_yaml(py::module& m)
{
    m.def("bug_2655", &test_data::bug_2655);
    m.def("hydrostatic_test", &test_data::hydrostatic_test);
    m.def("added_mass_from_hdb_file", &test_data::added_mass_from_hdb_file);
    m.def("full_example", &test_data::full_example);
    m.def("full_example_with_diagonal_inertia", &test_data::full_example_with_diagonal_inertia);
    m.def("full_example_with_propulsion", &test_data::full_example_with_propulsion);
    m.def("full_example_with_propulsion_and_old_key_name", &test_data::full_example_with_propulsion_and_old_key_name);
    m.def("falling_ball_example", &test_data::falling_ball_example);
    m.def("oscillating_cube_example", &test_data::oscillating_cube_example);
    m.def("new_oscillating_cube_example", &test_data::new_oscillating_cube_example);
    m.def("stable_cube_example", &test_data::stable_cube_example);
    m.def("stable_rolling_cube_test", &test_data::stable_rolling_cube_test);
    m.def("test_ship_hydrostatic_test", &test_data::test_ship_hydrostatic_test);
    m.def("test_ship_exact_hydrostatic_test", &test_data::test_ship_exact_hydrostatic_test);
    m.def("test_ship_fast_hydrostatic_test", &test_data::test_ship_fast_hydrostatic_test);
    m.def("test_ship_propulsion", &test_data::test_ship_propulsion);
    m.def("test_ship_waves_test", &test_data::test_ship_waves_test);
    m.def("test_ship_diffraction", &test_data::test_ship_diffraction);
    m.def("waves", &test_data::waves);
    m.def("simple_waves", &test_data::simple_waves);
    m.def("cube_in_waves", &test_data::cube_in_waves);
    m.def("waves_for_parser_validation_only", &test_data::waves_for_parser_validation_only);
    m.def("test_ship_froude_krylov", &test_data::test_ship_froude_krylov);
    m.def("falling_cube", &test_data::falling_cube);
    m.def("rolling_cube", &test_data::rolling_cube);
    m.def("test_ship_damping", &test_data::test_ship_damping);
    m.def("controlled_forces", &test_data::controlled_forces);
    m.def("bug_2961", &test_data::bug_2961);
    m.def("setpoints", &test_data::setpoints);
    m.def("unknown_controller", &test_data::unknown_controller);
    m.def("grpc_controller", &test_data::grpc_controller);
    m.def("controllers", &test_data::controllers);
    m.def("wageningen", &test_data::wageningen);
    m.def("resistance_curve", &test_data::resistance_curve);
    m.def("propulsion_and_resistance", &test_data::propulsion_and_resistance);
    m.def("dummy_controllers_and_commands_for_propulsion_and_resistance", &test_data::dummy_controllers_and_commands_for_propulsion_and_resistance);
    m.def("heading_keeping_base", &test_data::heading_keeping_base);
    m.def("heading_keeping_controllers", &test_data::heading_keeping_controllers);
    m.def("radiation_damping", &test_data::radiation_damping);
    m.def("diffraction", &test_data::diffraction);
    m.def("diffraction_precalr", &test_data::diffraction_precalr);
    m.def("test_ship_radiation_damping", &test_data::test_ship_radiation_damping);
    m.def("simple_track_keeping", &test_data::simple_track_keeping);
    m.def("maneuvering_commands", &test_data::maneuvering_commands);
    m.def("maneuvering_with_same_frame_of_reference", &test_data::maneuvering_with_same_frame_of_reference);
    m.def("maneuvering", &test_data::maneuvering);
    m.def("bug_2641", &test_data::bug_2641);
    m.def("simple_station_keeping", &test_data::simple_station_keeping);
    m.def("rudder", &test_data::rudder);
    m.def("bug_in_exact_hydrostatic", &test_data::bug_in_exact_hydrostatic);
    m.def("bug_2714_heading_keeping", &test_data::bug_2714_heading_keeping);
    m.def("bug_2714_station_keeping", &test_data::bug_2714_station_keeping);
    m.def("bug_2732", &test_data::bug_2732);
    m.def("L_config", &test_data::L_config);
    m.def("GM_cube", &test_data::GM_cube);
    m.def("bug_2838", &test_data::bug_2838);
    m.def("bug_2845", &test_data::bug_2845);
    m.def("maneuvering_with_commands", &test_data::maneuvering_with_commands);
    m.def("kt_kq", &test_data::kt_kq);
    m.def("linear_hydrostatics", &test_data::linear_hydrostatics);
    m.def("test_ship_linear_hydrostatics_without_waves", &test_data::test_ship_linear_hydrostatics_without_waves);
    m.def("test_ship_linear_hydrostatics_with_waves", &test_data::test_ship_linear_hydrostatics_with_waves);
    m.def("fmi", &test_data::fmi);
    m.def("dummy_history", &test_data::dummy_history);
    m.def("complete_yaml_message_from_gui", &test_data::complete_yaml_message_from_gui);
    m.def("JSON_message_with_requested_output", &test_data::JSON_message_with_requested_output);
    m.def("JSON_server_request_GM_cube_with_output", &test_data::JSON_server_request_GM_cube_with_output);
    m.def("complete_yaml_message_for_falling_ball", &test_data::complete_yaml_message_for_falling_ball);
    m.def("bug_2963_hs_fast", &test_data::bug_2963_hs_fast);
    m.def("bug_2963_hs_exact", &test_data::bug_2963_hs_exact);
    m.def("bug_2963_fk", &test_data::bug_2963_fk);
    m.def("bug_2963_diff", &test_data::bug_2963_diff);
    m.def("bug_2963_gm", &test_data::bug_2963_gm);
    m.def("bug_3004", &test_data::bug_3004);
    m.def("bug_3003", &test_data::bug_3003);
    m.def("bug_2984", &test_data::bug_2984);
    m.def("bug_3217", &test_data::bug_3217);
    m.def("bug_3227", &test_data::bug_3227);
    m.def("yml_bug_3230", &test_data::yml_bug_3230);
    m.def("bug_3235", &test_data::bug_3235);
    m.def("bug_3207_yml", &test_data::bug_3207_yml);
    m.def("bug_3241", &test_data::bug_3241);
    m.def("constant_force", &test_data::constant_force);
    m.def("issue_20", &test_data::issue_20);
    m.def("simserver_test_with_commands_and_delay", &test_data::simserver_test_with_commands_and_delay);
    m.def("simserver_message_without_Dt", &test_data::simserver_message_without_Dt);
    m.def("man_with_delay", &test_data::man_with_delay);
    m.def("invalid_json_for_cs", &test_data::invalid_json_for_cs);
    m.def("tutorial_09_gRPC_wave_model", &test_data::tutorial_09_gRPC_wave_model);
    m.def("tutorial_10_gRPC_force_model", &test_data::tutorial_10_gRPC_force_model);
    m.def("tutorial_10_gRPC_force_model_commands", &test_data::tutorial_10_gRPC_force_model_commands);
    m.def("grpc_setpoints", &test_data::grpc_setpoints);
    m.def("gRPC_force_model", &test_data::gRPC_force_model);
    m.def("gRPC_controller", &test_data::gRPC_controller);
    m.def("bug_3187", &test_data::bug_3187);
    m.def("bug_3185_with_invalid_frame", &test_data::bug_3185_with_invalid_frame);
    m.def("bug_3185", &test_data::bug_3185);
    m.def("tutorial_11_gRPC_controller", &test_data::tutorial_11_gRPC_controller);
    m.def("added_mass_from_precal_file", &test_data::added_mass_from_precal_file);
    m.def("tutorial_13_hdb_force_model", &test_data::tutorial_13_hdb_force_model);
    m.def("tutorial_13_precal_r_force_model", &test_data::tutorial_13_precal_r_force_model);
    m.def("tutorial_14_filtered_states", &test_data::tutorial_14_filtered_states);

}

void py_add_module_xdyn_data_test(py::module& m);
void py_add_module_xdyn_data_test(py::module& m)
{
    m.def("test_ship_hdb", &test_data::test_ship_hdb);
    m.def("analytical_Br", &test_data::analytical_Br, py::arg("omega"));
    m.def("analytical_K", &test_data::analytical_K, py::arg("tau"));
    m.def("test_ship_hdb", &test_data::test_ship_hdb);
    m.def("bug_3210", &test_data::bug_3210);
    m.def("bug_3230_hdb", &test_data::bug_3230_hdb);
    m.def("bug_3207_hdb", &test_data::bug_3207_hdb);
    m.def("bug_3238_hdb", &test_data::bug_3238_hdb);
    m.def("hdb_issue_184", &test_data::hdb_issue_184);
    m.def("big_hdb", &test_data::big_hdb);

    py::module m_precal = m.def_submodule("precal");
    m_precal.def("precal", &test_data::precal);
    m_precal.def("precal_with_si_unit", &test_data::precal_with_si_unit);
    m_precal.def("glossary_of_terms", &glossary_of_terms);
    m_precal.def("general", &general);
    m_precal.def("constants", &constants);
    m_precal.def("simulation", &simulation);
    m_precal.def("exports", &exports);
    m_precal.def("ship_particulars", &ship_particulars);
    m_precal.def("mass_properties", &mass_properties);
    m_precal.def("mass_matrix", &mass_matrix);
    m_precal.def("restoring_matrix", &restoring_matrix);
    m_precal.def("natural_periods_and_frequencies", &natural_periods_and_frequencies);
    m_precal.def("added_mass_damping_matrix_inf_freq", &added_mass_damping_matrix_inf_freq);
    m_precal.def("hull_properties", &hull_properties);
    m_precal.def("roll_damping", &roll_damping);
    m_precal.def("labels", &labels);
    m_precal.def("dimensions", &dimensions);
    m_precal.def("signals", &signals);
    m_precal.def("raos", &raos);
}

void py_add_module_xdyn_data(py::module& m)
{
    py::module m_data = m.def_submodule("data");
    py::module m_data_body = m_data.def_submodule("body");
    py::module m_data_mesh = m_data.def_submodule("mesh");
    py::module m_data_yaml = m_data.def_submodule("yaml");
    py::module m_data_test = m_data.def_submodule("test");

    py_add_module_xdyn_data_body(m_data_body);
    py_add_module_xdyn_data_mesh(m_data_mesh);
    py_add_module_xdyn_data_yaml(m_data_yaml);
    py_add_module_xdyn_data_test(m_data_test);
}

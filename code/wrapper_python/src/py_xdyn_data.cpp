#include "py_xdyn_data.hpp"
#include "py_pybind_additions.hpp"
#include "test_data_generator/inc/TriMeshTestData.hpp"
#include "test_data_generator/inc/yaml_data.hpp"
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

void py_add_module_xdyn_data(py::module& m)
{
    py::module m_data = m.def_submodule("data");
    py::module m_data_body = m_data.def_submodule("body");
    py::module m_data_mesh = m_data.def_submodule("mesh");
    py::module m_data_yaml = m_data.def_submodule("yaml");
    m_data_body.def("get_body", &get_body,
        py::arg("name"),
        py::arg("points") = two_triangles()
        );
    m_data_mesh.def("one_triangle",&one_triangle);
    m_data_mesh.def("one_triangle_clockwise",&one_triangle_clockwise);
    m_data_mesh.def("degenerated_triangle",&degenerated_triangle);
    m_data_mesh.def("unit_cube",&unit_cube);
    m_data_mesh.def("unit_cube_with_incorrect_orientation",&unit_cube_with_incorrect_orientation);
    m_data_mesh.def("unit_cube_clockwise",&unit_cube_clockwise);
    m_data_mesh.def("two_triangles",&two_triangles);
    m_data_mesh.def("two_triangles_immerged",&two_triangles_immerged);
    m_data_mesh.def("trapezium",&trapezium);
    m_data_mesh.def("n_gone",&n_gone);
    m_data_mesh.def("cube",&cube);
    m_data_mesh.def("tetrahedron",&tetrahedron);
    m_data_mesh.def("tetrahedron_clockwise",&tetrahedron_clockwise);
    m_data_mesh.def("generated_stl",&generated_stl);
    m_data_mesh.def("L",&L);
    m_data_mesh.def("U",&U);
    m_data_yaml.def("bug_2655", &test_data::bug_2655);
    m_data_yaml.def("hydrostatic_test", &test_data::hydrostatic_test);
    m_data_yaml.def("added_mass_from_hdb_file", &test_data::added_mass_from_hdb_file);
    m_data_yaml.def("full_example", &test_data::full_example);
    m_data_yaml.def("full_example_with_diagonal_inertia", &test_data::full_example_with_diagonal_inertia);
    m_data_yaml.def("full_example_with_propulsion", &test_data::full_example_with_propulsion);
    m_data_yaml.def("full_example_with_propulsion_and_old_key_name", &test_data::full_example_with_propulsion_and_old_key_name);
    m_data_yaml.def("falling_ball_example", &test_data::falling_ball_example);
    m_data_yaml.def("oscillating_cube_example", &test_data::oscillating_cube_example);
    m_data_yaml.def("new_oscillating_cube_example", &test_data::new_oscillating_cube_example);
    m_data_yaml.def("stable_cube_example", &test_data::stable_cube_example);
    m_data_yaml.def("stable_rolling_cube_test", &test_data::stable_rolling_cube_test);
    m_data_yaml.def("test_ship_hydrostatic_test", &test_data::test_ship_hydrostatic_test);
    m_data_yaml.def("test_ship_exact_hydrostatic_test", &test_data::test_ship_exact_hydrostatic_test);
    m_data_yaml.def("test_ship_fast_hydrostatic_test", &test_data::test_ship_fast_hydrostatic_test);
    m_data_yaml.def("test_ship_new_hydrostatic_test", &test_data::test_ship_new_hydrostatic_test);
    m_data_yaml.def("test_ship_propulsion", &test_data::test_ship_propulsion);
    m_data_yaml.def("test_ship_waves_test", &test_data::test_ship_waves_test);
    m_data_yaml.def("test_ship_diffraction", &test_data::test_ship_diffraction);
    m_data_yaml.def("waves", &test_data::waves);
    m_data_yaml.def("simple_waves", &test_data::simple_waves);
    m_data_yaml.def("cube_in_waves", &test_data::cube_in_waves);
    m_data_yaml.def("waves_for_parser_validation_only", &test_data::waves_for_parser_validation_only);
    m_data_yaml.def("test_ship_froude_krylov", &test_data::test_ship_froude_krylov);
    m_data_yaml.def("falling_cube", &test_data::falling_cube);
    m_data_yaml.def("rolling_cube", &test_data::rolling_cube);
    m_data_yaml.def("test_ship_damping", &test_data::test_ship_damping);
    m_data_yaml.def("controlled_forces", &test_data::controlled_forces);
    m_data_yaml.def("bug_2961", &test_data::bug_2961);
    m_data_yaml.def("setpoints", &test_data::setpoints);
    m_data_yaml.def("unknown_controller", &test_data::unknown_controller);
    m_data_yaml.def("grpc_controller", &test_data::grpc_controller);
    m_data_yaml.def("controllers", &test_data::controllers);
    m_data_yaml.def("wageningen", &test_data::wageningen);
    m_data_yaml.def("resistance_curve", &test_data::resistance_curve);
    m_data_yaml.def("propulsion_and_resistance", &test_data::propulsion_and_resistance);
    m_data_yaml.def("dummy_controllers_and_commands_for_propulsion_and_resistance", &test_data::dummy_controllers_and_commands_for_propulsion_and_resistance);
    m_data_yaml.def("heading_keeping_base", &test_data::heading_keeping_base);
    m_data_yaml.def("heading_keeping_controllers", &test_data::heading_keeping_controllers);
    m_data_yaml.def("radiation_damping", &test_data::radiation_damping);
    m_data_yaml.def("diffraction", &test_data::diffraction);
    m_data_yaml.def("diffraction_precalr", &test_data::diffraction_precalr);
    m_data_yaml.def("test_ship_radiation_damping", &test_data::test_ship_radiation_damping);
    m_data_yaml.def("simple_track_keeping", &test_data::simple_track_keeping);
    m_data_yaml.def("maneuvering_commands", &test_data::maneuvering_commands);
    m_data_yaml.def("maneuvering_with_same_frame_of_reference", &test_data::maneuvering_with_same_frame_of_reference);
    m_data_yaml.def("maneuvering", &test_data::maneuvering);
    m_data_yaml.def("bug_2641", &test_data::bug_2641);
    m_data_yaml.def("simple_station_keeping", &test_data::simple_station_keeping);
    m_data_yaml.def("rudder", &test_data::rudder);
    m_data_yaml.def("bug_in_exact_hydrostatic", &test_data::bug_in_exact_hydrostatic);
    m_data_yaml.def("bug_2714_heading_keeping", &test_data::bug_2714_heading_keeping);
    m_data_yaml.def("bug_2714_station_keeping", &test_data::bug_2714_station_keeping);
    m_data_yaml.def("bug_2732", &test_data::bug_2732);
    m_data_yaml.def("L_config", &test_data::L_config);
    m_data_yaml.def("GM_cube", &test_data::GM_cube);
    m_data_yaml.def("bug_2838", &test_data::bug_2838);
    m_data_yaml.def("bug_2845", &test_data::bug_2845);
    m_data_yaml.def("maneuvering_with_commands", &test_data::maneuvering_with_commands);
    m_data_yaml.def("kt_kq", &test_data::kt_kq);
    m_data_yaml.def("linear_hydrostatics", &test_data::linear_hydrostatics);
    m_data_yaml.def("test_ship_linear_hydrostatics_without_waves", &test_data::test_ship_linear_hydrostatics_without_waves);
    m_data_yaml.def("test_ship_linear_hydrostatics_with_waves", &test_data::test_ship_linear_hydrostatics_with_waves);
    m_data_yaml.def("fmi", &test_data::fmi);
    m_data_yaml.def("dummy_history", &test_data::dummy_history);
    m_data_yaml.def("complete_yaml_message_from_gui", &test_data::complete_yaml_message_from_gui);
    m_data_yaml.def("JSON_message_with_requested_output", &test_data::JSON_message_with_requested_output);
    m_data_yaml.def("JSON_server_request_GM_cube_with_output", &test_data::JSON_server_request_GM_cube_with_output);
    m_data_yaml.def("complete_yaml_message_for_falling_ball", &test_data::complete_yaml_message_for_falling_ball);
    m_data_yaml.def("bug_2963_hs_fast", &test_data::bug_2963_hs_fast);
    m_data_yaml.def("bug_2963_hs_exact", &test_data::bug_2963_hs_exact);
    m_data_yaml.def("bug_2963_fk", &test_data::bug_2963_fk);
    m_data_yaml.def("bug_2963_diff", &test_data::bug_2963_diff);
    m_data_yaml.def("bug_2963_gm", &test_data::bug_2963_gm);
    m_data_yaml.def("bug_3004", &test_data::bug_3004);
    m_data_yaml.def("bug_3003", &test_data::bug_3003);
    m_data_yaml.def("bug_2984", &test_data::bug_2984);
    m_data_yaml.def("bug_3217", &test_data::bug_3217);
    m_data_yaml.def("bug_3227", &test_data::bug_3227);
    m_data_yaml.def("yml_bug_3230", &test_data::yml_bug_3230);
    m_data_yaml.def("bug_3235", &test_data::bug_3235);
    m_data_yaml.def("bug_3207_yml", &test_data::bug_3207_yml);
    m_data_yaml.def("bug_3241", &test_data::bug_3241);
    m_data_yaml.def("constant_force", &test_data::constant_force);
    m_data_yaml.def("issue_20", &test_data::issue_20);
    m_data_yaml.def("simserver_test_with_commands_and_delay", &test_data::simserver_test_with_commands_and_delay);
    m_data_yaml.def("simserver_message_without_Dt", &test_data::simserver_message_without_Dt);
    m_data_yaml.def("man_with_delay", &test_data::man_with_delay);
    m_data_yaml.def("invalid_json_for_cs", &test_data::invalid_json_for_cs);
    m_data_yaml.def("tutorial_09_gRPC_wave_model", &test_data::tutorial_09_gRPC_wave_model);
    m_data_yaml.def("tutorial_10_gRPC_force_model", &test_data::tutorial_10_gRPC_force_model);
    m_data_yaml.def("tutorial_10_gRPC_force_model_commands", &test_data::tutorial_10_gRPC_force_model_commands);
    m_data_yaml.def("grpc_setpoints", &test_data::grpc_setpoints);
    m_data_yaml.def("gRPC_force_model", &test_data::gRPC_force_model);
    m_data_yaml.def("gRPC_controller", &test_data::gRPC_controller);
    m_data_yaml.def("bug_3187", &test_data::bug_3187);
    m_data_yaml.def("bug_3185_with_invalid_frame", &test_data::bug_3185_with_invalid_frame);
    m_data_yaml.def("bug_3185", &test_data::bug_3185);
    m_data_yaml.def("tutorial_11_gRPC_controller", &test_data::tutorial_11_gRPC_controller);
    m_data_yaml.def("added_mass_from_precal_file", &test_data::added_mass_from_precal_file);
    m_data_yaml.def("tutorial_13_hdb_force_model", &test_data::tutorial_13_hdb_force_model);
    m_data_yaml.def("tutorial_13_precal_r_force_model", &test_data::tutorial_13_precal_r_force_model);
    m_data_yaml.def("tutorial_14_filtered_states", &test_data::tutorial_14_filtered_states);
}
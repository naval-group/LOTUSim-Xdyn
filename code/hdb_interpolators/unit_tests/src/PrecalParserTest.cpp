/*
 * PrecalParserTest.cpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#include "PrecalParserTest.hpp"
#include "PrecalParser.hpp"
#include "PrecalParserHelper.hpp"
#include "precal_test_data.hpp"
#include <cstdio>
#include <fstream>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

PrecalParserTest::PrecalParserTest() {}

PrecalParserTest::~PrecalParserTest() {}

void PrecalParserTest::SetUp() {}

void PrecalParserTest::TearDown() {}

TEST_F(PrecalParserTest, can_parse_empty_section)
{
    const auto precal = parse_precal_from_string("[some section]");
    ASSERT_EQ(1, precal.sections.size());
    ASSERT_EQ("some section", precal.sections.at(0).title);
}

TEST_F(PrecalParserTest, can_parse_general_section)
{
    auto precal = parse_precal_from_string(general());
    ASSERT_EQ(1, precal.sections.size());
    ASSERT_EQ("General", precal.sections.at(0).title);
    ASSERT_EQ("NOT SPECIFIED", precal.sections.at(0).string_values["userName"]);
    ASSERT_EQ("NOT SPECIFIED", precal.sections.at(0).string_values["projectNumber"]);
    ASSERT_EQ("DTMB_5415_without_fins", precal.sections.at(0).string_values["projectName"]);
    ASSERT_EQ("NOT SPECIFIED", precal.sections.at(0).string_values["caseName"]);
    ASSERT_EQ("PRECAL - version 18.1.3", precal.sections.at(0).string_values["sourceName"]);
    ASSERT_EQ("2021-07-13", precal.sections.at(0).string_values["date"]);
    ASSERT_EQ("12:22:18", precal.sections.at(0).string_values["time"]);
    ASSERT_EQ("Windows", precal.sections.at(0).string_values["OS"]);
    ASSERT_DOUBLE_EQ(-2, precal.sections.at(0).scalar_values["numOmpThreads"]);
    ASSERT_DOUBLE_EQ(4, precal.sections.at(0).scalar_values["NrOmpThreads"]);
    ASSERT_EQ("18.1.3", precal.sections.at(0).string_values["VersionNumber"]);
    ASSERT_EQ("Tue Dec 10 10:07:44 2019", precal.sections.at(0).string_values["CreationDate"]);
    ASSERT_DOUBLE_EQ(1800, precal.sections.at(0).scalar_values["IfortVersion"]);
}

TEST_F(PrecalParserTest, can_parse_vectors)
{
    auto precal = parse_precal_from_string(
        "[Particulars-ship]\n"
        "COB              = {1.519,0.000,0.066} ; (ship center of buoyancy w.r.t. aft "
        "perpendicular - centerline - keel line, calculated from geometry)\n");
    ASSERT_EQ(3, precal.sections.at(0).vector_values["COB"].size());
    ASSERT_DOUBLE_EQ(1.519, precal.sections.at(0).vector_values["COB"].at(0));
    ASSERT_DOUBLE_EQ(0.000, precal.sections.at(0).vector_values["COB"].at(1));
    ASSERT_DOUBLE_EQ(0.066, precal.sections.at(0).vector_values["COB"].at(2));
}

TEST_F(PrecalParserTest, can_parse_ship_particulars)
{
    auto precal = parse_precal_from_string(ship_particulars());
    ASSERT_EQ(3, precal.sections.at(0).vector_values["COB"].size());
    ASSERT_DOUBLE_EQ(70.217, precal.sections.at(0).vector_values["COB"].at(0));
    ASSERT_DOUBLE_EQ(0.000, precal.sections.at(0).vector_values["COB"].at(1));
    ASSERT_DOUBLE_EQ(3.659, precal.sections.at(0).vector_values["COB"].at(2));
    ASSERT_DOUBLE_EQ(6.150, precal.sections.at(0).scalar_values["T_mean"]);
    ASSERT_DOUBLE_EQ(7.510, precal.sections.at(0).vector_values["COG"].at(2));
}

TEST_F(PrecalParserTest, can_parse_rao_titles)
{
    auto precal = parse_precal_from_string(raos());
    ASSERT_EQ("Signal 1: surge motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, "
              "U=0.000kn, mu=180.000deg (amplitude unit = m/m, phase unit = deg)",
              precal.raos.at(0).title_line);
    ASSERT_EQ("Signal 5: pitch motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, U=20.000kn, mu=90.000deg (amplitude unit = deg/m, phase unit = deg)",
              precal.raos.at(29).title_line);
}

TEST_F(PrecalParserTest, can_parse_rao_attributes)
{
    const auto rao_attributes = parse_rao_attributes(
        "Signal 1: surge motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, "
        "U=12.000kn, mu=180.000deg (amplitude unit = m/m, phase unit = deg)");
    ASSERT_EQ("surge motion", rao_attributes.name);
    ASSERT_EQ(Eigen::Vector3d(70.015, 0.000, 7.510), rao_attributes.position);
    ASSERT_DOUBLE_EQ(-1, rao_attributes.h);
    ASSERT_EQ("m", rao_attributes.h_unit);
    ASSERT_DOUBLE_EQ(0.5, rao_attributes.phi_a);
    ASSERT_EQ("deg", rao_attributes.phi_a_unit);
    ASSERT_DOUBLE_EQ(12, rao_attributes.U);
    ASSERT_EQ("kn", rao_attributes.U_unit);
    ASSERT_DOUBLE_EQ(180, rao_attributes.mu);
    ASSERT_EQ("deg", rao_attributes.mu_unit);
    ASSERT_EQ("m/m", rao_attributes.amplitude_unit);
    ASSERT_EQ("deg", rao_attributes.phase_unit);
}

TEST_F(PrecalParserTest, full_rao_parse_test)
{
    auto precal = parse_precal_from_string(raos());

    // Amplitudes and phases - Signal 4: roll motion, U=20.000kn, mu=90.000deg
    ASSERT_EQ("heave motion", precal.raos.at(15).attributes.name);
    ASSERT_EQ(Eigen::Vector3d(70.015,0.000,7.510), precal.raos.at(15).attributes.position);
    ASSERT_DOUBLE_EQ(-1, precal.raos.at(15).attributes.h);
    ASSERT_EQ("m", precal.raos.at(15).attributes.h_unit);
    ASSERT_DOUBLE_EQ(0.5, precal.raos.at(15).attributes.phi_a);
    ASSERT_EQ("deg", precal.raos.at(15).attributes.phi_a_unit);
    ASSERT_DOUBLE_EQ(12, precal.raos.at(15).attributes.U);
    ASSERT_EQ("kn", precal.raos.at(15).attributes.U_unit);
    ASSERT_DOUBLE_EQ(90, precal.raos.at(15).attributes.mu);
    ASSERT_EQ("deg", precal.raos.at(15).attributes.mu_unit);
    ASSERT_EQ("m/m", precal.raos.at(15).attributes.amplitude_unit);
    ASSERT_EQ("deg", precal.raos.at(15).attributes.phase_unit);
    ASSERT_DOUBLE_EQ(0.100835E+01, precal.raos.at(15).left_column[1]);
    ASSERT_DOUBLE_EQ(0.105814E+01, precal.raos.at(15).left_column[6]);
    ASSERT_DOUBLE_EQ(-0.081168, precal.raos.at(15).right_column[1]);
    ASSERT_DOUBLE_EQ(-15.504014, precal.raos.at(15).right_column[6]);

    // No phases - Signal 10: F_drift_m1_c4, U=20.000kn, mu=180.000deg
    ASSERT_EQ("kN/m2", precal.raos.at(38).attributes.amplitude_unit);
    ASSERT_DOUBLE_EQ(-0.655426E+01, precal.raos.at(38).left_column[0]);
    ASSERT_DOUBLE_EQ(-0.690853E+02, precal.raos.at(38).left_column[4]);
    ASSERT_EQ("N.A.", precal.raos.at(38).attributes.phase_unit);
    ASSERT_DOUBLE_EQ(0, precal.raos.at(38).right_column[0]);
    ASSERT_DOUBLE_EQ(0, precal.raos.at(38).right_column[4]);
}

TEST_F(PrecalParserTest, can_parse_from_file)
{
    const std::string filename = "precal.ini";
    std::ofstream of(filename);
    of << test_data::precal();
    of.close();
    auto precal = parse_precal_from_file(filename);
    ASSERT_EQ("heave motion", precal.raos.at(15).attributes.name);
    remove(filename.c_str());
}

TEST_F(PrecalParserTest, can_parse_added_mass_multi_line_vector)
{
    auto precal = parse_precal_from_string(added_mass_damping_matrix_inf_freq());
    ASSERT_EQ(1, precal.sections.size());
    ASSERT_NE(precal.sections.at(0).vector_values.end(),
              precal.sections.at(0).vector_values.find("total_added_mass_matrix_inf_freq_U1_mu1"));
    ASSERT_EQ(36,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu1"].size());
    ASSERT_DOUBLE_EQ(0.110E+06,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu1"].at(0));
    ASSERT_DOUBLE_EQ(-0.888E-01,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu1"].at(1));
    ASSERT_DOUBLE_EQ(0.612E+01,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu1"].at(5));

    ASSERT_NE(precal.sections.at(0).vector_values.end(),
              precal.sections.at(0).vector_values.find("total_added_mass_matrix_inf_freq_U1_mu2"));
    ASSERT_EQ(36,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu2"].size());
    ASSERT_DOUBLE_EQ(0.174E+02,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U1_mu2"].at(29));

    ASSERT_NE(precal.sections.at(0).vector_values.end(),
              precal.sections.at(0).vector_values.find("total_added_mass_matrix_inf_freq_U2_mu1"));
    ASSERT_EQ(36,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U2_mu1"].size());
    ASSERT_DOUBLE_EQ(0.570E+01,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U2_mu1"].at(5));

    ASSERT_NE(precal.sections.at(0).vector_values.end(),
              precal.sections.at(0).vector_values.find("total_added_mass_matrix_inf_freq_U2_mu2"));
    ASSERT_EQ(36,
            precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U2_mu2"].size());
    ASSERT_DOUBLE_EQ(-0.119E+03,
        precal.sections.at(0).vector_values["total_added_mass_matrix_inf_freq_U2_mu2"].at(29));

}

TEST_F(PrecalParserTest, can_parse_added_mass_matrix)
{
    /* FROM PRECAL_R
             0.110E+06  -0.888E-01   0.226E+06  -0.144E+00  0.270E+08   0.612E+01
            -0.122E-01   0.344E+07  -0.563E-02  -0.113E+07  0.169E+02   0.497E+08
             0.227E+06  -0.898E+00   0.129E+08   0.763E+01  0.844E+08   0.512E+01
             0.183E+00  -0.123E+07   0.251E+01   0.498E+08  0.854E+02   0.338E+09
             0.270E+08   0.106E+01   0.845E+08  -0.431E+02  0.119E+11   0.174E+02
             0.164E+01   0.497E+08   0.101E+02   0.345E+09  0.214E+03   0.522E+10


    Ma(0,1) = -Ma(0,1);
    Ma(0,2) = -Ma(0,2);
    Ma(0,4) = -Ma(0,4);
    Ma(0,5) = -Ma(0,5);
    Ma(1,0) = -Ma(1,0);
    Ma(1,3) = -Ma(1,3);
    Ma(2,0) = -Ma(2,0);
    Ma(2,3) = -Ma(2,3);
    Ma(3,1) = -Ma(3,1);
    Ma(3,2) = -Ma(3,2);
    Ma(3,4) = -Ma(3,4);
    Ma(3,5) = -Ma(3,5);
    Ma(4,0) = -Ma(4,0);
    Ma(4,3) = -Ma(4,3);
    Ma(5,0) = -Ma(5,0);
    Ma(5,3) = -Ma(5,3);
    */
    const auto precal = PrecalParser::from_string(test_data::precal());
    const auto Ma = precal.get_added_mass();
    ASSERT_DOUBLE_EQ( 0.110E+06  ,  Ma(0, 0));
    ASSERT_DOUBLE_EQ( -0.888E-01   , -Ma(0, 1));
    ASSERT_DOUBLE_EQ( 0.226E+06  , -Ma(0, 2));
    ASSERT_DOUBLE_EQ( -0.144E+00  ,  Ma(0, 3));
    ASSERT_DOUBLE_EQ( 0.270E+08   , -Ma(0, 4));
    ASSERT_DOUBLE_EQ( 0.612E+01, -Ma(0, 5));

    ASSERT_DOUBLE_EQ(-0.122E-01  , -Ma(1, 0));
    ASSERT_DOUBLE_EQ( 0.344E+07 ,  Ma(1, 1));
    ASSERT_DOUBLE_EQ( -0.563E-02 ,  Ma(1, 2));
    ASSERT_DOUBLE_EQ( -0.113E+07 , -Ma(1, 3));
    ASSERT_DOUBLE_EQ( 0.169E+02  ,  Ma(1, 4));
    ASSERT_DOUBLE_EQ( 0.497E+08,  Ma(1, 5));

    ASSERT_DOUBLE_EQ(0.227E+06  , -Ma(2, 0));
    ASSERT_DOUBLE_EQ(-0.898E+00   ,  Ma(2, 1));
    ASSERT_DOUBLE_EQ(0.129E+08  ,  Ma(2, 2));
    ASSERT_DOUBLE_EQ( 0.763E+01  , -Ma(2, 3));
    ASSERT_DOUBLE_EQ(0.844E+08   ,  Ma(2, 4));
    ASSERT_DOUBLE_EQ(0.512E+01,  Ma(2, 5));

    ASSERT_DOUBLE_EQ(0.183E+00 ,  Ma(3, 0));
    ASSERT_DOUBLE_EQ( -0.123E+07  , -Ma(3, 1));
    ASSERT_DOUBLE_EQ( 0.251E+01   , -Ma(3, 2));
    ASSERT_DOUBLE_EQ(0.498E+08 ,  Ma(3, 3));
    ASSERT_DOUBLE_EQ( 0.854E+02  , -Ma(3, 4));
    ASSERT_DOUBLE_EQ( 0.338E+09, -Ma(3, 5));

    ASSERT_DOUBLE_EQ(0.270E+08   , -Ma(4, 0));
    ASSERT_DOUBLE_EQ(0.106E+01   ,  Ma(4, 1));
    ASSERT_DOUBLE_EQ(0.845E+08  ,  Ma(4, 2));
    ASSERT_DOUBLE_EQ(-0.431E+02  , -Ma(4, 3));
    ASSERT_DOUBLE_EQ(0.119E+11  ,  Ma(4, 4));
    ASSERT_DOUBLE_EQ( 0.174E+02,  Ma(4, 5));

    ASSERT_DOUBLE_EQ(0.164E+01   , -Ma(5, 0));
    ASSERT_DOUBLE_EQ(0.497E+08  ,  Ma(5, 1));
    ASSERT_DOUBLE_EQ( 0.101E+02  ,  Ma(5, 2));
    ASSERT_DOUBLE_EQ( 0.345E+09  , -Ma(5, 3));
    ASSERT_DOUBLE_EQ(0.214E+03   ,  Ma(5, 4));
    ASSERT_DOUBLE_EQ(0.522E+10,  Ma(5, 5));
}




TEST_F(PrecalParserTest, can_parse_angular_frequencies)
{
    const auto precal = PrecalParser::from_string(test_data::precal());
    const std::vector<double> omegas = precal.get_angular_frequencies();
    ASSERT_EQ(7, omegas.size());
    ASSERT_DOUBLE_EQ(0.400, omegas.at(0));
    ASSERT_DOUBLE_EQ(0.500, omegas.at(1));
    ASSERT_DOUBLE_EQ(0.600, omegas.at(2));
    ASSERT_DOUBLE_EQ(0.700, omegas.at(3));
    ASSERT_DOUBLE_EQ(0.800, omegas.at(4));
    ASSERT_DOUBLE_EQ(0.900, omegas.at(5));
    ASSERT_DOUBLE_EQ(1.000, omegas.at(6));
}

TEST_F(PrecalParserTest, can_get_forward_speed)
{
    const auto precal = PrecalParser::from_string(test_data::precal());
    ASSERT_DOUBLE_EQ(0, precal.get_forward_speed());
}

TEST_F(PrecalParserTest, can_get_added_mass_coefficients_for_each_frequency)
{
    const auto precal = PrecalParser::from_string(test_data::precal());
    const std::vector<double> A_11 = precal.get_added_mass_coeff(0, 0);
    ASSERT_EQ(7, A_11.size());
    ASSERT_DOUBLE_EQ(0.275560E+03, A_11.at(0));
    ASSERT_DOUBLE_EQ(0.271498E+03, A_11.at(1));
    ASSERT_DOUBLE_EQ(0.233842E+03, A_11.at(2));
    ASSERT_DOUBLE_EQ(0.183239E+03, A_11.at(3));
    ASSERT_DOUBLE_EQ(0.147226E+03, A_11.at(4));
    ASSERT_DOUBLE_EQ(0.128148E+03, A_11.at(5));
    ASSERT_DOUBLE_EQ(0.114148E+03, A_11.at(6));
}

TEST_F(PrecalParserTest, can_get_radiation_damping_matrix_coefficients_for_each_frequency)
{
    const auto precal = PrecalParser::from_string(test_data::precal());
    const std::vector<double> Br_34 = precal.get_radiation_damping_coeff(2, 3);
    ASSERT_EQ(7, Br_34.size());
    ASSERT_DOUBLE_EQ(-0.223749E-03, Br_34.at(0));
    ASSERT_DOUBLE_EQ(-0.214409E-03, Br_34.at(1));
    ASSERT_DOUBLE_EQ( 0.123597E-02, Br_34.at(2));
    ASSERT_DOUBLE_EQ( 0.176562E-02, Br_34.at(3));
    ASSERT_DOUBLE_EQ( 0.260162E-03, Br_34.at(4));
    ASSERT_DOUBLE_EQ(-0.352648E-03, Br_34.at(5));
    ASSERT_DOUBLE_EQ(-0.102490E-02, Br_34.at(6));
}

TEST_F(PrecalParserTest, can_parse_diffraction_module_raos)
{
    auto precal = PrecalParser::from_string(test_data::precal());

    const std::vector<double> periods = precal.get_diffraction_module_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_diffraction_module_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_diffraction_module_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }

    // Multiplied by 1e3 to convert the table data to N/m
    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(0.138050E+03 * 1e3, table.at(0).at(6).at(1)); // X (F_dif_m1), first line, 180°
    ASSERT_DOUBLE_EQ(0.517219E+02 * 1e3, table.at(0).at(6).at(0)); // X (F_dif_m1), first line, 90°
    ASSERT_DOUBLE_EQ(0.117473E-02 * 1e3, table.at(1).at(6).at(1)); // Y (F_dif_m2), first line, 180°
    ASSERT_DOUBLE_EQ(0.847017E+04 * 1e3, table.at(2).at(4).at(0)); // Z (F_dif_m3), third line, 90°
    ASSERT_DOUBLE_EQ(0.360716E+04 * 1e3, table.at(3).at(1).at(0)); // K (F_dif_m4), sixth line, 90°
    ASSERT_DOUBLE_EQ(0.898604E+05 * 1e3, table.at(4).at(1).at(0)); // M (F_dif_m5), sixth line, 90°
    ASSERT_DOUBLE_EQ(0.291656E-01 * 1e3, table.at(5).at(0).at(1)); // N (F_dif_m6), seventh line, 180°

}

TEST_F(PrecalParserTest, can_parse_diffraction_module_raos_with_si_unit)
{
    auto precal = PrecalParser::from_string(test_data::precal_with_si_unit());

    const std::vector<double> periods = precal.get_diffraction_module_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_diffraction_module_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_diffraction_module_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }

    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(0.138050E+03, table.at(0).at(6).at(1)); // X (F_dif_m1), first line, 180°
    ASSERT_DOUBLE_EQ(0.517219E+02, table.at(0).at(6).at(0)); // X (F_dif_m1), first line, 90°
    ASSERT_DOUBLE_EQ(0.117473E-02, table.at(1).at(6).at(1)); // Y (F_dif_m2), first line, 180°
    ASSERT_DOUBLE_EQ(0.847017E+04, table.at(2).at(4).at(0)); // Z (F_dif_m3), third line, 90°
    ASSERT_DOUBLE_EQ(0.360716E+04, table.at(3).at(1).at(0)); // K (F_dif_m4), sixth line, 90°
    ASSERT_DOUBLE_EQ(0.898604E+05, table.at(4).at(1).at(0)); // M (F_dif_m5), sixth line, 90°
    ASSERT_DOUBLE_EQ(0.291656E-01, table.at(5).at(0).at(1)); // N (F_dif_m6), seventh line, 180°
}

TEST_F(PrecalParserTest, can_parse_diffraction_phase_raos)
{
    auto precal = PrecalParser::from_string(test_data::precal());

    const std::vector<double> periods = precal.get_diffraction_phase_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_diffraction_phase_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_diffraction_phase_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }

    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(  90.317017 * PI / 180, table.at(0).at(6).at(1)); // X (F_dif_m1), first line, 180°
    ASSERT_DOUBLE_EQ( -48.947906 * PI / 180, table.at(1).at(6).at(1)); // Y (F_dif_m2), first line, 180°
    ASSERT_DOUBLE_EQ( 131.552856 * PI / 180, table.at(2).at(4).at(0)); // Z (F_dif_m3), third line, 90°
    ASSERT_DOUBLE_EQ(-128.995148 * PI / 180, table.at(3).at(4).at(0)); // K (F_dif_m4), third line, 90°
    ASSERT_DOUBLE_EQ( 137.499237 * PI / 180, table.at(4).at(1).at(0)); // M (F_dif_m5), sixth line, 90°
    ASSERT_DOUBLE_EQ( -50.890854 * PI / 180, table.at(5).at(0).at(1)); // N (F_dif_m6), seventh line, 180°
}

TEST_F(PrecalParserTest, can_parse_froude_krylov_module_raos)
{
    // sim > parRES > expIncWaveFrc
    auto precal = PrecalParser::from_string(test_data::precal());

    const std::vector<double> periods = precal.get_froude_krylov_module_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_froude_krylov_module_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_froude_krylov_module_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }

    // Multiplied by 1e3 to convert the table data to N/m
    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(0.419735E+02 * 1E3, table.at(0).at(6).at(0)); // X (F_inc_m1), first line, 90°
    ASSERT_DOUBLE_EQ(0.988816E-03 * 1E3, table.at(1).at(5).at(1)); // Y (F_inc_m2), second line, 180°
    ASSERT_DOUBLE_EQ(0.179547E+05 * 1E3, table.at(2).at(4).at(0)); // Z (F_inc_m3), third line, 90°
    ASSERT_DOUBLE_EQ(0.658274E-02 * 1E3, table.at(3).at(3).at(1)); // K (F_inc_m4), fourth line, 180°
    ASSERT_DOUBLE_EQ(0.114842E+06 * 1E3, table.at(4).at(2).at(0)); // M (F_inc_m5), fifth line, 90°
    ASSERT_DOUBLE_EQ(0.208950E-01 * 1E3, table.at(5).at(1).at(1)); // N (F_inc_m6), sixth line, 180°

}


TEST_F(PrecalParserTest, can_parse_froude_krylov_module_raos_with_si_unit)
{
    // sim > parRES > expIncWaveFrc
    auto precal = PrecalParser::from_string(test_data::precal_with_si_unit());

    const std::vector<double> periods = precal.get_froude_krylov_module_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_froude_krylov_module_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_froude_krylov_module_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }

    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(0.419735E+02, table.at(0).at(6).at(0)); // X (F_inc_m1), first line, 90°
    ASSERT_DOUBLE_EQ(0.988816E-03, table.at(1).at(5).at(1)); // Y (F_inc_m2), second line, 180°
    ASSERT_DOUBLE_EQ(0.179547E+05, table.at(2).at(4).at(0)); // Z (F_inc_m3), third line, 90°
    ASSERT_DOUBLE_EQ(0.658274E-02, table.at(3).at(3).at(1)); // K (F_inc_m4), fourth line, 180°
    ASSERT_DOUBLE_EQ(0.114842E+06, table.at(4).at(2).at(0)); // M (F_inc_m5), fifth line, 90°
    ASSERT_DOUBLE_EQ(0.208950E-01, table.at(5).at(1).at(1)); // N (F_inc_m6), sixth line, 180°

}

TEST_F(PrecalParserTest, can_parse_froude_krylov_phase_raos)
{
    auto precal = PrecalParser::from_string(test_data::precal());

    const std::vector<double> periods = precal.get_froude_krylov_phase_periods();
    ASSERT_EQ(7, periods.size());
    ASSERT_DOUBLE_EQ(2 * PI / 0.4, periods.at(6));
    ASSERT_DOUBLE_EQ(2 * PI / 0.5, periods.at(5));
    ASSERT_DOUBLE_EQ(2 * PI / 0.6, periods.at(4));
    ASSERT_DOUBLE_EQ(2 * PI / 0.7, periods.at(3));
    ASSERT_DOUBLE_EQ(2 * PI / 0.8, periods.at(2));
    ASSERT_DOUBLE_EQ(2 * PI / 0.9, periods.at(1));
    ASSERT_DOUBLE_EQ(2 * PI,       periods.at(0));

    const std::vector<double> psis = precal.get_froude_krylov_phase_psis();
    ASSERT_EQ(2, psis.size());
    ASSERT_DOUBLE_EQ(PI, psis.at(1));
    ASSERT_DOUBLE_EQ(PI / 2, psis.at(0));

    const std::array<std::vector<std::vector<double>>, 6> table = precal.get_froude_krylov_phase_tables();
    for (size_t mod_idx = 0; mod_idx < 6; ++mod_idx)
    {
        ASSERT_EQ(7, table.at(mod_idx).size());
        for (size_t period_idx = 0; period_idx < 7; ++period_idx)
        {
            ASSERT_EQ(2, table.at(mod_idx).at(period_idx).size());
        }
    }
    // column (axis), pulsation, incidence
    ASSERT_DOUBLE_EQ(-179.999939 * PI/180, table.at(0).at(6).at(0)); // X (F_inc_m1), first line, 90°
    ASSERT_DOUBLE_EQ( 171.732681 * PI/180, table.at(1).at(5).at(1)); // Y (F_inc_m2), second line, 180°
    ASSERT_DOUBLE_EQ(  -0.000003 * PI/180, table.at(2).at(4).at(0)); // Z (F_inc_m3), third line, 90°
    ASSERT_DOUBLE_EQ( -26.438093 * PI/180, table.at(3).at(3).at(1)); // K (F_inc_m4), fourth line, 180°
    ASSERT_DOUBLE_EQ(  -0.000017 * PI/180, table.at(4).at(2).at(0)); // M (F_inc_m5), fifth line, 90°
    ASSERT_DOUBLE_EQ(  -0.018264 * PI/180, table.at(5).at(1).at(1)); // N (F_inc_m6), sixth line, 180°
}
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
    ASSERT_EQ("ONRT_SIMMAN", precal.sections.at(0).string_values["projectName"]);
    ASSERT_EQ("NOT SPECIFIED", precal.sections.at(0).string_values["caseName"]);
    ASSERT_EQ("PRECAL - version 18.1.3", precal.sections.at(0).string_values["sourceName"]);
    ASSERT_EQ("2021-02-18", precal.sections.at(0).string_values["date"]);
    ASSERT_EQ("17:43:27", precal.sections.at(0).string_values["time"]);
    ASSERT_EQ("Windows", precal.sections.at(0).string_values["OS"]);
    ASSERT_DOUBLE_EQ(-2, precal.sections.at(0).scalar_values["numOmpThreads"]);
    ASSERT_DOUBLE_EQ(2, precal.sections.at(0).scalar_values["NrOmpThreads"]);
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
    ASSERT_DOUBLE_EQ(1.519, precal.sections.at(0).vector_values["COB"].at(0));
    ASSERT_DOUBLE_EQ(0.000, precal.sections.at(0).vector_values["COB"].at(1));
    ASSERT_DOUBLE_EQ(0.066, precal.sections.at(0).vector_values["COB"].at(2));
    ASSERT_DOUBLE_EQ(0.112, precal.sections.at(0).scalar_values["T_mean"]);
    ASSERT_DOUBLE_EQ(0.156, precal.sections.at(0).vector_values["COG"].at(2));
}

TEST_F(PrecalParserTest, can_parse_rao_titles)
{
    auto precal = parse_precal_from_string(raos());
    ASSERT_EQ("Signal 1: surge motion at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
              "U=0.000kn, mu=0.000deg (amplitude unit = m/m, phase unit = deg)",
              precal.raos.at(0).title_line);
    ASSERT_EQ("Signal 30: F_drift_m5_c4 at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
              "U=0.000kn, mu=0.000deg (amplitude unit = N.m/m2, phase unit = N.A.)",
              precal.raos.at(29).title_line);
}

TEST_F(PrecalParserTest, can_parse_rao_attributes)
{
    const auto rao_attributes = parse_rao_attributes(
        "Signal 1: surge motion at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
        "U=0.000kn, mu=0.000deg (amplitude unit = m/m, phase unit = deg)");
    ASSERT_EQ("surge motion", rao_attributes.name);
    ASSERT_EQ(Eigen::Vector3d(1.521, 0.000, 0.156), rao_attributes.position);
    ASSERT_DOUBLE_EQ(-1, rao_attributes.h);
    ASSERT_EQ("m", rao_attributes.h_unit);
    ASSERT_DOUBLE_EQ(16, rao_attributes.phi_a);
    ASSERT_EQ("deg", rao_attributes.phi_a_unit);
    ASSERT_DOUBLE_EQ(0, rao_attributes.U);
    ASSERT_EQ("kn", rao_attributes.U_unit);
    ASSERT_DOUBLE_EQ(0, rao_attributes.mu);
    ASSERT_EQ("deg", rao_attributes.mu_unit);
    ASSERT_EQ("m/m", rao_attributes.amplitude_unit);
    ASSERT_EQ("deg", rao_attributes.phase_unit);
}

TEST_F(PrecalParserTest, full_rao_parse_test)
{
    auto precal = parse_precal_from_string(raos());
    ASSERT_EQ("F_drift_m2", precal.raos.at(15).attributes.name);
    ASSERT_EQ(Eigen::Vector3d(1.521, 0.000, 0.156), precal.raos.at(15).attributes.position);
    ASSERT_DOUBLE_EQ(-1, precal.raos.at(15).attributes.h);
    ASSERT_EQ("m", precal.raos.at(15).attributes.h_unit);
    ASSERT_DOUBLE_EQ(16, precal.raos.at(15).attributes.phi_a);
    ASSERT_EQ("deg", precal.raos.at(15).attributes.phi_a_unit);
    ASSERT_DOUBLE_EQ(0, precal.raos.at(15).attributes.U);
    ASSERT_EQ("kn", precal.raos.at(15).attributes.U_unit);
    ASSERT_DOUBLE_EQ(0, precal.raos.at(15).attributes.mu);
    ASSERT_EQ("deg", precal.raos.at(15).attributes.mu_unit);
    ASSERT_EQ("N/m2", precal.raos.at(15).attributes.amplitude_unit);
    ASSERT_EQ("N.A.", precal.raos.at(15).attributes.phase_unit);

    ASSERT_DOUBLE_EQ(0.282862E+02, precal.raos.at(15).left_column[3]);
    ASSERT_DOUBLE_EQ(0, precal.raos.at(15).right_column[3]);
    ASSERT_DOUBLE_EQ(0.831906E-04, precal.raos.at(39).left_column[3]);
    ASSERT_DOUBLE_EQ(-178.966675, precal.raos.at(5).right_column[29]);
}

TEST_F(PrecalParserTest, can_parse_from_file)
{
    const std::string filename = "precal.ini";
    std::ofstream of(filename);
    of << test_data::precal();
    of.close();
    auto precal = parse_precal_from_file(filename);
    ASSERT_EQ("F_drift_m2", precal.raos.at(15).attributes.name);
    remove(filename.c_str());
}

TEST_F(PrecalParserTest, can_parse_added_mass_matrix)
{
     const auto precal = PrecalParser::from_string(test_data::precal());
     const auto Ma = precal.get_added_mass();
     ASSERT_DOUBLE_EQ(0.582645, Ma(0,0)); // Signal 37
     ASSERT_DOUBLE_EQ(0.117227E-05, Ma(0,1)); // Signal 39
     ASSERT_DOUBLE_EQ(-0.250294E+01, Ma(0,2)); // Signal 41
     ASSERT_DOUBLE_EQ(-0.395587E-07, Ma(0,3)); // Signal 43
     ASSERT_DOUBLE_EQ(-0.431212E+01, Ma(0,4)); // Signal 45
     ASSERT_DOUBLE_EQ(0.123341E-05, Ma(0,5)); // Signal 47

     ASSERT_DOUBLE_EQ(0.170619E-04, Ma(1,0)); // Signal 49
     ASSERT_DOUBLE_EQ(0.163189E+02, Ma(1,1)); // Signal 51
     ASSERT_DOUBLE_EQ(-0.115813E-03, Ma(1,2)); // Signal 53
     ASSERT_DOUBLE_EQ(0.107510E+00, Ma(1,3)); // Signal 55
     ASSERT_DOUBLE_EQ(-0.169549E-03, Ma(1,4)); // Signal 57
     ASSERT_DOUBLE_EQ(0.399228E+01, Ma(1,5)); // Signal 59

     ASSERT_DOUBLE_EQ(-0.256342E+01, Ma(2,0)); // Signal 61
     ASSERT_DOUBLE_EQ(0.412774E-05, Ma(2,1)); // Signal 63
     ASSERT_DOUBLE_EQ(0.112625E+03, Ma(2,2)); // Signal 65
     ASSERT_DOUBLE_EQ(-0.717206E-06, Ma(2,3)); // Signal 67
     ASSERT_DOUBLE_EQ(0.230724E+02, Ma(2,4)); // Signal 69
     ASSERT_DOUBLE_EQ(0.177471E-05, Ma(2,5)); // Signal 71

     ASSERT_DOUBLE_EQ(-0.305183E-06, Ma(3,0)); // Signal 73
     ASSERT_DOUBLE_EQ(0.109620E+00, Ma(3,1)); // Signal 75
     ASSERT_DOUBLE_EQ(0.213158E-05, Ma(3,2)); // Signal 77
     ASSERT_DOUBLE_EQ(0.200769E+00, Ma(3,3)); // Signal 79
     ASSERT_DOUBLE_EQ(0.359481E-05, Ma(3,4)); // Signal 81
     ASSERT_DOUBLE_EQ(-0.743680E+00, Ma(3,5)); // Signal 83

     ASSERT_DOUBLE_EQ(-0.430115E+01, Ma(4,0)); // Signal 85
     ASSERT_DOUBLE_EQ(-0.648689E-05, Ma(4,1)); // Signal 87
     ASSERT_DOUBLE_EQ(0.223271E+02, Ma(4,2)); // Signal 89
     ASSERT_DOUBLE_EQ(-0.536761E-06, Ma(4,3)); // Signal 91
     ASSERT_DOUBLE_EQ(0.507329E+02, Ma(4,4)); // Signal 93
     ASSERT_DOUBLE_EQ(0.372722E-05, Ma(4,5)); // Signal 95

     ASSERT_DOUBLE_EQ(0.165006E-04, Ma(5,0)); // Signal 97
     ASSERT_DOUBLE_EQ(0.392391E+01, Ma(5,1)); // Signal 99
     ASSERT_DOUBLE_EQ(-0.129658E-03, Ma(5,2)); // Signal 101
     ASSERT_DOUBLE_EQ(-0.765712E+00, Ma(5,3)); // Signal 103
     ASSERT_DOUBLE_EQ(-0.168508E-03, Ma(5,4)); // Signal 105
     ASSERT_DOUBLE_EQ(0.129615E+02, Ma(5,5)); // Signal 107
}
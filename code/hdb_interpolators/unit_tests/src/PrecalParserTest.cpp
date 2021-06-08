/*
 * PrecalParserTest.cpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#include "PrecalParserTest.hpp"
#include "PrecalParser.hpp"
#include "precal_test_data.hpp"

PrecalParserTest::PrecalParserTest() {}

PrecalParserTest::~PrecalParserTest() {}

void PrecalParserTest::SetUp() {}

void PrecalParserTest::TearDown() {}

TEST_F(PrecalParserTest, can_parse_empty_section)
{
    const auto precal = parse_precal("[some section]");
    ASSERT_EQ(1, precal.sections.size());
    ASSERT_EQ("some section", precal.sections.at(0).title);
}

TEST_F(PrecalParserTest, can_parse_general_section)
{
    auto precal = parse_precal(general());
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
    auto precal = parse_precal("[Particulars-ship]\n"
    "COB              = {1.519,0.000,0.066} ; (ship center of buoyancy w.r.t. aft "
        "perpendicular - centerline - keel line, calculated from geometry)\n");
    ASSERT_EQ(3, precal.sections.at(0).vector_values["COB"].size());
    ASSERT_DOUBLE_EQ(1.519, precal.sections.at(0).vector_values["COB"].at(0));
    ASSERT_DOUBLE_EQ(0.000, precal.sections.at(0).vector_values["COB"].at(1));
    ASSERT_DOUBLE_EQ(0.066, precal.sections.at(0).vector_values["COB"].at(2));
}

TEST_F(PrecalParserTest, can_parse_ship_particulars)
{
    auto precal = parse_precal(ship_particulars());
    ASSERT_EQ(3, precal.sections.at(0).vector_values["COB"].size());
    ASSERT_DOUBLE_EQ(1.519, precal.sections.at(0).vector_values["COB"].at(0));
    ASSERT_DOUBLE_EQ(0.000, precal.sections.at(0).vector_values["COB"].at(1));
    ASSERT_DOUBLE_EQ(0.066, precal.sections.at(0).vector_values["COB"].at(2));
    ASSERT_DOUBLE_EQ(0.112, precal.sections.at(0).scalar_values["T_mean"]);
    ASSERT_DOUBLE_EQ(0.156, precal.sections.at(0).vector_values["COG"].at(2));
}

TEST_F(PrecalParserTest, can_parse_rao_titles)
{
    auto precal = parse_precal(raos());
    ASSERT_EQ("Signal 1: surge motion at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
           "U=0.000kn, mu=0.000deg (amplitude unit = m/m, phase unit = deg)", precal.raos.at(0).title_line);
    ASSERT_EQ("Signal 30: F_drift_m5_c4 at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
           "U=0.000kn, mu=0.000deg (amplitude unit = N.m/m2, phase unit = N.A.)", precal.raos.at(29).title_line);
}

TEST_F(PrecalParserTest, can_parse_rao_attributes)
{
    const auto rao_attributes = parse_rao_attributes("Signal 1: surge motion at (1.521,0.000,0.156), h=-1.000m, phi_a=16.000deg, "
           "U=0.000kn, mu=0.000deg (amplitude unit = m/m, phase unit = deg)");
    ASSERT_EQ("surge motion", rao_attributes.name);
    ASSERT_EQ(Eigen::Vector3d(1.521,0.000,0.156), rao_attributes.position);
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
    auto precal = parse_precal(raos());
    ASSERT_EQ("F_drift_m2", precal.raos.at(15).attributes.name);
    ASSERT_EQ(Eigen::Vector3d(1.521,0.000,0.156), precal.raos.at(15).attributes.position);
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
}
/*
 * ParseFMIXmlTest.cpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#include "ParseFMIXmlTest.hpp"
#include "ParseFMIXml.hpp"

ParseFMIXmlTest::ParseFMIXmlTest() : a(ssc::random_data_generator::DataGenerator(54222100))
{
}

ParseFMIXmlTest::~ParseFMIXmlTest()
{
}

void ParseFMIXmlTest::SetUp()
{
}

void ParseFMIXmlTest::TearDown()
{
}

std::string test_xml();
std::string test_xml()
{
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF8\"?>\n"
       << "<fmiModelDescription\n"
       << "    fmiVersion=\"1.0\"\n"
       << "    author=\"me\"\n"
       << "    modelName=\"Modelica.Mechanics.Rotational.Examples.Friction\"\n"
       << "    modelIdentifier=\"Modelica_Mechanics_Rotational_Examples_Friction\"\n"
       << "    guid=\"{8c4e810f-3df3-4a00-8276-176fa3c9f9e0}\"\n"
       << "    description=\"Drive train with clutch and brake\"\n"
       << "    version=\"3.1\"\n"
       << "    generationTool=\"Dymola Version 7.4, 2010-01-25\"\n"
       << "    generationDateAndTime=\"2009-12-22T16:57:33Z\"\n"
       << "    variableNamingConvention=\"structured\"\n"
       << "    numberOfContinuousStates=\"6\"\n"
       << "    numberOfEventIndicators=\"34\">\n"
       << "<UnitDefinitions>\n"
       << "<BaseUnit unit=\"rad\">\n"
       << "<DisplayUnitDefinition displayUnit=\"deg\" gain=\"57.2957795130823\"/>\n"
       << "</BaseUnit>\n"
       << "</UnitDefinitions>\n"
       << "<TypeDefinitions>\n"
       << "<Type name=\"Modelica.SIunits.Torque\">\n"
       << "<RealType quantity=\"MomentOfInertia\" unit=\"kg.m2\" min=\"0.0\"/>\n"
       << "</Type>\n"
       << "<Type name=\"Modelica.SIunits.AngularVelocity\">\n"
       << "<RealType quantity=\"AngularVelocity\" unit=\"rad/s\"/>\n"
       << "</Type>\n"
       << "</TypeDefinitions>\n"
       << "<DefaultExperiment startTime=\"0.0\" stopTime=\"3.0\" tolerance=\"0.0001\"/>\n"
       << "<ModelVariables>\n"
       << "<ScalarVariable\n"
       << "name=\"inertia1.J\"\n"
       << "valueReference=\"16777217\"\n"
       << "description=\"Moment of inertia\"\n"
       << "variability=\"parameter\">\n"
       << "<Real declaredType=\"Modelica.SIunits.Torque\" start=\"1\"/>\n"
       << "</ScalarVariable>\n"
       << "</ModelVariables>\n"
       << "</fmiModelDescription>\n";
    return ss.str();
}

const fmi::Xml xml = fmi::parse(test_xml());

TEST_F(ParseFMIXmlTest, can_parse_author)
{
    ASSERT_EQ("me", xml.attributes.author);
}
TEST_F(ParseFMIXmlTest, can_parse_description)
{
    ASSERT_EQ("Drive train with clutch and brake", xml.attributes.description);
}

TEST_F(ParseFMIXmlTest, can_parse_version)
{
    ASSERT_EQ("1.0", xml.attributes.fmiVersion);
}

TEST_F(ParseFMIXmlTest, can_parse_date_and_time)
{
    ASSERT_EQ(22, xml.attributes.generationDateAndTime.day);
    ASSERT_EQ(16, xml.attributes.generationDateAndTime.hours);
    ASSERT_EQ(57, xml.attributes.generationDateAndTime.minutes);
    ASSERT_EQ(12, xml.attributes.generationDateAndTime.month);
    ASSERT_EQ(33, xml.attributes.generationDateAndTime.seconds);
    ASSERT_EQ(2009, xml.attributes.generationDateAndTime.year);
}

TEST_F(ParseFMIXmlTest, can_parse_generation_tool)
{
    ASSERT_EQ("Dymola Version 7.4, 2010-01-25", xml.attributes.generationTool);
}

TEST_F(ParseFMIXmlTest, can_parse_guid)
{
    ASSERT_EQ("{8c4e810f-3df3-4a00-8276-176fa3c9f9e0}", xml.attributes.guid);
}

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

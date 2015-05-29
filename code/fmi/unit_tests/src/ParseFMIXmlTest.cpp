/*
 * ParseFMIXmlTest.cpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#include <limits>

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
       << "    modelIdentifier=\"IDModelica_Mechanics_Rotational_Examples_Friction\"\n"
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
       << "<DefaultExperiment startTime=\"1.0\" stopTime=\"3.0\" tolerance=\"0.0001\"/>\n"
       << "<ModelVariables>\n"
       << "<ScalarVariable\n"
       << "name=\"inertia1.J\"\n"
       << "valueReference=\"16777217\"\n"
       << "description=\"Moment of inertia\"\n"
       << "variability=\"parameter\">\n"
       << "<Real declaredType=\"Modelica.SIunits.Torque\" start=\"1\"/>\n"
       << "</ScalarVariable>\n"
       << "</ModelVariables>\n"
       << "<VendorAnnotations>\n"
       << "<Tool name=\"tool 1\">\n"
       << "<Annotation name=\"a1\" value=\"v5\"/>"
       << "<Annotation name=\"a2\" value=\"v4\"/>"
       << "<Annotation name=\"a3\" value=\"v3\"/>"
       << "</Tool>\n"
       << "<Tool name=\"tool 2\">\n"
       << "<Annotation name=\"a4\" value=\"v3\"/>"
       << "<Annotation name=\"a5\" value=\"v2\"/>"
       << "<Annotation name=\"a6\" value=\"v1\"/>"
       << "</Tool>\n"
       << "<Tool name=\"tool 3\">\n"
       << "<Annotation name=\"a7\" value=\"v0\"/>"
       << "<Annotation name=\"a8\" value=\"v1\"/>"
       << "</Tool>\n"
       << "</VendorAnnotations>\n"
       << "</fmiModelDescription>\n";
    return ss.str();
}

const fmi::Xml xml = fmi::parse(test_xml());

TEST_F(ParseFMIXmlTest, can_parse_attributes)
{
    ASSERT_EQ("me", xml.attributes.author);
    ASSERT_EQ("Drive train with clutch and brake", xml.attributes.description);
    ASSERT_EQ("1.0", xml.attributes.fmiVersion);
    ASSERT_EQ(22, xml.attributes.generationDateAndTime.day);
    ASSERT_EQ(16, xml.attributes.generationDateAndTime.hours);
    ASSERT_EQ(57, xml.attributes.generationDateAndTime.minutes);
    ASSERT_EQ(12, xml.attributes.generationDateAndTime.month);
    ASSERT_EQ(33, xml.attributes.generationDateAndTime.seconds);
    ASSERT_EQ(2009, xml.attributes.generationDateAndTime.year);
    ASSERT_EQ("Dymola Version 7.4, 2010-01-25", xml.attributes.generationTool);
    ASSERT_EQ("{8c4e810f-3df3-4a00-8276-176fa3c9f9e0}", xml.attributes.guid);
    ASSERT_EQ("Modelica.Mechanics.Rotational.Examples.Friction", xml.attributes.modelName);
    ASSERT_EQ("IDModelica_Mechanics_Rotational_Examples_Friction", xml.attributes.modelidentifier);
    ASSERT_EQ(34, xml.attributes.numberOfEventsIndicators);
    ASSERT_EQ(fmi::NamingConvention::STRUCTURED, xml.attributes.variableNamingConvention);
    ASSERT_EQ(6, xml.attributes.numberOfContinuousStates);
}

TEST_F(ParseFMIXmlTest, can_parse_unit_definitions)
{
    ASSERT_EQ(1, xml.UnitDefinitions.size());
    ASSERT_EQ("rad", xml.UnitDefinitions.at(0).unit);
    ASSERT_EQ(1, xml.UnitDefinitions.at(0).definitions.size());
    ASSERT_EQ("deg", xml.UnitDefinitions.at(0).definitions.at(0).displayUnit);
    ASSERT_DOUBLE_EQ(57.2957795130823, xml.UnitDefinitions.at(0).definitions.at(0).gain);
    ASSERT_DOUBLE_EQ(0, xml.UnitDefinitions.at(0).definitions.at(0).offset);
}

TEST_F(ParseFMIXmlTest, can_parse_default_experiment)
{
    ASSERT_DOUBLE_EQ(1, xml.default_experiment.startTime);
    ASSERT_DOUBLE_EQ(3, xml.default_experiment.stopTime);
    ASSERT_DOUBLE_EQ(1E-4, xml.default_experiment.tolerance);
}

TEST_F(ParseFMIXmlTest, can_parse_real_model_variables)
{
    ASSERT_EQ(1, xml.real_model_variables.size());
    ASSERT_EQ(fmi::Alias::NO_ALIAS, xml.real_model_variables.at(0).alias);
    ASSERT_EQ("Modelica.SIunits.Torque", xml.real_model_variables.at(0).attributes.declaredType);
    ASSERT_EQ("unit", xml.real_model_variables.at(0).attributes.displayUnit);
    ASSERT_EQ(false, xml.real_model_variables.at(0).attributes.fixed);
    ASSERT_EQ(std::numeric_limits<double>::max(), xml.real_model_variables.at(0).attributes.max);
    ASSERT_EQ(std::numeric_limits<double>::lowest(), xml.real_model_variables.at(0).attributes.min);
    ASSERT_EQ(1, xml.real_model_variables.at(0).attributes.nominal);
    ASSERT_EQ("", xml.real_model_variables.at(0).attributes.quantity);
    ASSERT_EQ(false, xml.real_model_variables.at(0).attributes.relativeQuantity);
    ASSERT_EQ(1, xml.real_model_variables.at(0).attributes.start);
    ASSERT_EQ("", xml.real_model_variables.at(0).attributes.unit);
    ASSERT_EQ(fmi::Causality::NONE, xml.real_model_variables.at(0).causality);
    ASSERT_EQ("Moment of inertia", xml.real_model_variables.at(0).description);
    ASSERT_TRUE(xml.real_model_variables.at(0).direct_dependency.empty());
    ASSERT_EQ("inertia1.J", xml.real_model_variables.at(0).name);
    ASSERT_EQ(16777217, xml.real_model_variables.at(0).valueReference);
    ASSERT_EQ(fmi::Variability::PARAMETER, xml.real_model_variables.at(0).variability);
}

TEST_F(ParseFMIXmlTest, can_parse_type_definitions)
{
    ASSERT_EQ(2, xml.type_definitions.size());
    ASSERT_EQ("", xml.type_definitions.at(0).description);
    ASSERT_EQ("Modelica.SIunits.Torque", xml.type_definitions.at(0).name);
    ASSERT_EQ("unit", xml.type_definitions.at(0).type_.displayUnit);
    ASSERT_DOUBLE_EQ(std::numeric_limits<double>::max(), xml.type_definitions.at(0).type_.max);
    ASSERT_DOUBLE_EQ(0, xml.type_definitions.at(0).type_.min);
    ASSERT_DOUBLE_EQ(1, xml.type_definitions.at(0).type_.nominal);
    ASSERT_EQ("MomentOfInertia", xml.type_definitions.at(0).type_.quantity);
    ASSERT_EQ(false, xml.type_definitions.at(0).type_.relativeQuantity);
    ASSERT_EQ("kg.m2", xml.type_definitions.at(0).type_.unit);

    ASSERT_EQ("", xml.type_definitions.at(1).description);
    ASSERT_EQ("Modelica.SIunits.AngularVelocity", xml.type_definitions.at(1).name);
    ASSERT_EQ("unit", xml.type_definitions.at(1).type_.displayUnit);
    ASSERT_DOUBLE_EQ(std::numeric_limits<double>::max(), xml.type_definitions.at(1).type_.max);
    ASSERT_DOUBLE_EQ(std::numeric_limits<double>::lowest(), xml.type_definitions.at(1).type_.min);
    ASSERT_DOUBLE_EQ(1, xml.type_definitions.at(1).type_.nominal);
    ASSERT_EQ("AngularVelocity", xml.type_definitions.at(1).type_.quantity);
    ASSERT_EQ(false, xml.type_definitions.at(1).type_.relativeQuantity);
    ASSERT_EQ("rad/s", xml.type_definitions.at(1).type_.unit);
}

TEST_F(ParseFMIXmlTest, can_parse_vendor_annotations)
{
    ASSERT_EQ(3, xml.vendor_annotations.size());

    ASSERT_EQ("tool 1", xml.vendor_annotations.at(0).name);
    ASSERT_EQ(3, xml.vendor_annotations.at(0).annotations.size());
    ASSERT_EQ("a1", xml.vendor_annotations.at(0).annotations.at(0).name);
    ASSERT_EQ("v5", xml.vendor_annotations.at(0).annotations.at(0).value);
    ASSERT_EQ("a2", xml.vendor_annotations.at(0).annotations.at(1).name);
    ASSERT_EQ("v4", xml.vendor_annotations.at(0).annotations.at(1).value);
    ASSERT_EQ("a3", xml.vendor_annotations.at(0).annotations.at(2).name);
    ASSERT_EQ("v3", xml.vendor_annotations.at(0).annotations.at(2).value);

    ASSERT_EQ("tool 2", xml.vendor_annotations.at(1).name);
    ASSERT_EQ(3, xml.vendor_annotations.at(1).annotations.size());
    ASSERT_EQ("a4", xml.vendor_annotations.at(1).annotations.at(0).name);
    ASSERT_EQ("v3", xml.vendor_annotations.at(1).annotations.at(0).value);
    ASSERT_EQ("a5", xml.vendor_annotations.at(1).annotations.at(1).name);
    ASSERT_EQ("v2", xml.vendor_annotations.at(1).annotations.at(1).value);
    ASSERT_EQ("a6", xml.vendor_annotations.at(1).annotations.at(2).name);
    ASSERT_EQ("v1", xml.vendor_annotations.at(1).annotations.at(2).value);

    ASSERT_EQ("tool 3", xml.vendor_annotations.at(2).name);
    ASSERT_EQ(2, xml.vendor_annotations.at(2).annotations.size());
    ASSERT_EQ("a7", xml.vendor_annotations.at(2).annotations.at(0).name);
    ASSERT_EQ("v0", xml.vendor_annotations.at(2).annotations.at(0).value);
    ASSERT_EQ("a8", xml.vendor_annotations.at(2).annotations.at(1).name);
    ASSERT_EQ("v1", xml.vendor_annotations.at(2).annotations.at(1).value);
}

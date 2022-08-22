/*
 * EmitFMIXmlTest.cpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#include "EmitFMIXmlTest.hpp"
#include "EmitFMIXml.hpp"
#include "get_sha.hpp"
#include "ParseFMIXml.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

EmitFMIXmlTest::EmitFMIXmlTest() : a(ssc::random_data_generator::DataGenerator(21212))
{
}

EmitFMIXmlTest::~EmitFMIXmlTest()
{
}

void EmitFMIXmlTest::SetUp()
{
}

void EmitFMIXmlTest::TearDown()
{
}

#define COMPARE(field) ASSERT_EQ(lhs.field, rhs.field);
#define COMPARE_DOUBLE(field) ASSERT_DOUBLE_EQ(lhs.field, rhs.field);

namespace fmi
{
    void compare(const Annotation& lhs, const Annotation& rhs);
    void compare(const Tool& lhs, const Tool& rhs);
    void compare(const RealType& lhs, const RealType& rhs);
    void compare(const Type<RealType>& lhs, const Type<RealType>& rhs);
    void compare(const RealAttributes& lhs, const RealAttributes& rhs);
    void compare(const ScalarVariable<RealAttributes>& lhs, const ScalarVariable<RealAttributes>& rhs);
    void compare(const DefaultExperiment lhs, const DefaultExperiment rhs);
    void compare(const DateTime lhs, const DateTime rhs);
    void compare(const Attributes lhs, const Attributes rhs);
    void compare(const DisplayUnitDefinition lhs, const DisplayUnitDefinition rhs);
    void compare(const BaseUnit lhs, const BaseUnit rhs);
    void compare(const Xml lhs, const Xml rhs);

    bool operator==(const Annotation& lhs, const Annotation& rhs);
    bool operator==(const Tool& lhs, const Tool& rhs);
    bool operator==(const RealType& lhs, const RealType& rhs);
    bool operator==(const Type<RealType>& lhs, const Type<RealType>& rhs);
    bool operator==(const RealAttributes& lhs, const RealAttributes& rhs);
    bool operator==(const ScalarVariable<RealAttributes>& lhs, const ScalarVariable<RealAttributes>& rhs);
    bool operator==(const DefaultExperiment lhs, const DefaultExperiment rhs);
    bool operator==(const DateTime lhs, const DateTime rhs);
    bool operator==(const Attributes lhs, const Attributes rhs);
    bool operator==(const DisplayUnitDefinition lhs, const DisplayUnitDefinition rhs);
    bool operator==(const BaseUnit lhs, const BaseUnit rhs);
    bool operator==(const Xml lhs, const Xml rhs);

    bool operator==(const Annotation& lhs, const Annotation& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const Tool& lhs, const Tool& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const RealType& lhs, const RealType& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const Type<RealType>& lhs, const Type<RealType>& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const RealAttributes& lhs, const RealAttributes& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const ScalarVariable<RealAttributes>& lhs, const ScalarVariable<RealAttributes>& rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const DefaultExperiment lhs, const DefaultExperiment rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const DateTime lhs, const DateTime rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const Attributes lhs, const Attributes rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const DisplayUnitDefinition lhs, const DisplayUnitDefinition rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const BaseUnit lhs, const BaseUnit rhs)
    {
        compare(lhs,rhs);
        return true;
    }

    bool operator==(const Xml lhs, const Xml rhs)
    {
        compare(lhs,rhs);
        return true;
    }


    void compare(const Annotation& lhs, const Annotation& rhs)
    {
        ASSERT_EQ(lhs.name, rhs.name);
        COMPARE(name);
        COMPARE(value);
    }

    void compare(const Tool& lhs, const Tool& rhs)
    {
        COMPARE(name);
        COMPARE(annotations.size());
        for(size_t i = 0 ; i < lhs.annotations.size() ; ++i)
        {
            COMPARE(annotations.at(0));
        }
    }

    void compare(const RealType& lhs, const RealType& rhs)
    {
        COMPARE(quantity);
        COMPARE(unit);
        COMPARE(displayUnit);
        COMPARE(relativeQuantity);
        COMPARE_DOUBLE(min);
        COMPARE_DOUBLE(max);
        COMPARE_DOUBLE(nominal);
    }

    void compare(const Type<RealType>& lhs, const Type<RealType>& rhs)
    {
        COMPARE(name);
        COMPARE(description);
        COMPARE(type_);
    }

    void compare(const RealAttributes& lhs, const RealAttributes& rhs)
    {
        COMPARE(declaredType);
        COMPARE(quantity);
        COMPARE(unit);
        COMPARE(displayUnit);
        COMPARE(relativeQuantity);
        COMPARE_DOUBLE(min);
        COMPARE_DOUBLE(max);
        COMPARE_DOUBLE(nominal);
        COMPARE_DOUBLE(start);
    }

    void compare(const ScalarVariable<RealAttributes>& lhs, const ScalarVariable<RealAttributes>& rhs)
    {
        COMPARE(alias);
        COMPARE(attributes);
        COMPARE(causality);
        COMPARE(description);
        COMPARE(direct_dependency.size());
        for (size_t i = 0 ; i < lhs.direct_dependency.size() ; ++i)
        {
            COMPARE(direct_dependency.at(i));
        }
        COMPARE(name);
        COMPARE(valueReference);
        COMPARE(variability);
    }

    void compare(const DefaultExperiment lhs, const DefaultExperiment rhs)
    {
        COMPARE_DOUBLE(startTime);
        COMPARE_DOUBLE(stopTime);
        COMPARE_DOUBLE(tolerance);
    }

    void compare(const DateTime lhs, const DateTime rhs)
    {
        COMPARE(day);
        COMPARE(hours);
        COMPARE(minutes);
        COMPARE(month);
        COMPARE(seconds);
        COMPARE(year);
    }

    void compare(const Attributes lhs, const Attributes rhs)
    {
        COMPARE(author);
        COMPARE(description);
        COMPARE(fmiVersion);
        COMPARE(generationDateAndTime);
        COMPARE(generationTool);
        COMPARE(guid);
        COMPARE(modelName);
        COMPARE(modelidentifier);
        COMPARE(numberOfEventsIndicators);
        COMPARE(variableNamingConvention);
        COMPARE(version);
    }

    void compare(const DisplayUnitDefinition lhs, const DisplayUnitDefinition rhs)
    {
        COMPARE(displayUnit);
        COMPARE_DOUBLE(gain);
        COMPARE_DOUBLE(offset);
    }

    void compare(const BaseUnit lhs, const BaseUnit rhs)
    {
        COMPARE(definitions.size());
        for (size_t i = 0 ; i < lhs.definitions.size() ; ++i)
        {
            COMPARE(definitions.at(i));
        }
        COMPARE(unit);
    }

    void compare(const Xml lhs, const Xml rhs)
    {
        COMPARE(UnitDefinitions.size());
        for (size_t i = 0 ; i < lhs.UnitDefinitions.size() ; ++i)
        {
            COMPARE(UnitDefinitions.at(i));
        }
        COMPARE(attributes);
        COMPARE(default_experiment);
        COMPARE(real_model_variables.size());
        for (size_t i = 0 ; i < lhs.real_model_variables.size() ; ++i)
        {
            COMPARE(real_model_variables.at(i));
        }
        COMPARE(type_definitions.size());
        for (size_t i = 0 ; i < lhs.type_definitions.size() ; ++i)
        {
            COMPARE(type_definitions.at(i));
        }
        COMPARE(vendor_annotations.size());
        for (size_t i = 0 ; i < lhs.vendor_annotations.size() ; ++i)
        {
            COMPARE(vendor_annotations.at(i));
        }
    }
}

TEST_F(EmitFMIXmlTest, example)
{
//! [EmitFMIXmlTest example]
    const fmi::Xml original_xml = a.random<fmi::Xml>();
//! [EmitFMIXmlTest example]
//! [EmitFMIXmlTest expected output]
    const std::string str = fmi::emit(original_xml);
    const fmi::Xml parsed_xml = fmi::parse(str);
    ASSERT_EQ(original_xml, parsed_xml);
//! [EmitFMIXmlTest expected output]
}

TEST_F(EmitFMIXmlTest, can_generate_xml_corresponding_to_simulation)
{
    const auto yaml = test_data::bug_2845();
    const fmi::Xml xml = fmi::build(yaml);
    ASSERT_EQ(fmi::get_sha(SimulatorYamlParser(yaml).parse()), xml.attributes.guid);
    ASSERT_EQ("SES", xml.attributes.author);
    ASSERT_EQ("Ship & Environment Simulator", xml.attributes.description);
    ASSERT_EQ("1.0", xml.attributes.fmiVersion);
    ASSERT_EQ("SES", xml.attributes.generationTool);
    ASSERT_EQ("fmi_simulator", xml.attributes.modelName);
    ASSERT_EQ("fmi_simulator", xml.attributes.modelidentifier);
    ASSERT_EQ(0, xml.attributes.numberOfEventsIndicators);
    ASSERT_EQ(13, xml.attributes.numberOfContinuousStates);
    ASSERT_EQ(fmi::NamingConvention::FLAT, xml.attributes.variableNamingConvention);
    ASSERT_TRUE(xml.UnitDefinitions.empty());
    ASSERT_DOUBLE_EQ(0, xml.default_experiment.startTime);
    ASSERT_DOUBLE_EQ(10, xml.default_experiment.stopTime);
    ASSERT_DOUBLE_EQ(1E-3, xml.default_experiment.tolerance);
    ASSERT_EQ(13 /*states*/ + 3 /*commands*/, xml.real_model_variables.size());
    ASSERT_EQ("x(TestShip)", xml.real_model_variables.at(0).name);
    ASSERT_EQ("y(TestShip)", xml.real_model_variables.at(1).name);
    ASSERT_EQ("z(TestShip)", xml.real_model_variables.at(2).name);
    ASSERT_EQ("u(TestShip)", xml.real_model_variables.at(3).name);
    ASSERT_EQ("v(TestShip)", xml.real_model_variables.at(4).name);
    ASSERT_EQ("w(TestShip)", xml.real_model_variables.at(5).name);
    ASSERT_EQ("p(TestShip)", xml.real_model_variables.at(6).name);
    ASSERT_EQ("q(TestShip)", xml.real_model_variables.at(7).name);
    ASSERT_EQ("r(TestShip)", xml.real_model_variables.at(8).name);
    ASSERT_EQ("qr(TestShip)", xml.real_model_variables.at(9).name);
    ASSERT_EQ("qi(TestShip)", xml.real_model_variables.at(10).name);
    ASSERT_EQ("qj(TestShip)", xml.real_model_variables.at(11).name);
    ASSERT_EQ("qk(TestShip)", xml.real_model_variables.at(12).name);
    ASSERT_EQ("PropRudd(rpm)", xml.real_model_variables.at(13).name);
    ASSERT_EQ("PropRudd(P/D)", xml.real_model_variables.at(14).name);
    ASSERT_EQ("PropRudd(beta)", xml.real_model_variables.at(15).name);
    for (size_t i = 0 ; i < 16 ; ++i)
    {
        ASSERT_EQ(fmi::Alias::NO_ALIAS, xml.real_model_variables.at(i).alias);
        ASSERT_EQ(fmi::Causality::OUTPUT, xml.real_model_variables.at(i).causality); // Cf. FMI 2.0 p. 72
        ASSERT_EQ("", xml.real_model_variables.at(i).attributes.declaredType);
        ASSERT_EQ("", xml.real_model_variables.at(i).attributes.displayUnit);
        ASSERT_FALSE(xml.real_model_variables.at(i).attributes.fixed);
        ASSERT_EQ(std::numeric_limits<double>::lowest(), xml.real_model_variables.at(i).attributes.min);
        ASSERT_EQ(std::numeric_limits<double>::max(), xml.real_model_variables.at(i).attributes.max);
        ASSERT_EQ(1, xml.real_model_variables.at(i).attributes.nominal);
        ASSERT_FALSE(xml.real_model_variables.at(i).attributes.relativeQuantity);
        ASSERT_EQ(i, xml.real_model_variables.at(i).valueReference);
        ASSERT_EQ("", xml.real_model_variables.at(i).description);
        ASSERT_TRUE( xml.real_model_variables.at(i).direct_dependency.empty());
        ASSERT_EQ(fmi::Variability::CONTINUOUS, xml.real_model_variables.at(i).variability);
    }
    for (size_t i = 0 ; i < 3 ; ++i)
    {
        ASSERT_EQ("Distance", xml.real_model_variables.at(i).attributes.quantity);
        ASSERT_EQ("m", xml.real_model_variables.at(i).attributes.unit);
    }
    ASSERT_EQ("m/s", xml.real_model_variables.at(3).attributes.unit);
    ASSERT_EQ("m/s", xml.real_model_variables.at(4).attributes.unit);
    ASSERT_EQ("m/s", xml.real_model_variables.at(5).attributes.unit);
    ASSERT_EQ("rad/s", xml.real_model_variables.at(6).attributes.unit);
    ASSERT_EQ("rad/s", xml.real_model_variables.at(7).attributes.unit);
    ASSERT_EQ("rad/s", xml.real_model_variables.at(8).attributes.unit);
    for (size_t i = 0 ; i < 7 ; ++i)
    {
        ASSERT_EQ("", xml.real_model_variables.at(i+9).attributes.unit);
    }
    ASSERT_EQ("Speed", xml.real_model_variables.at(3).attributes.quantity);
    ASSERT_EQ("Speed", xml.real_model_variables.at(4).attributes.quantity);
    ASSERT_EQ("Speed", xml.real_model_variables.at(5).attributes.quantity);
    ASSERT_EQ("Angular velocity", xml.real_model_variables.at(6).attributes.quantity);
    ASSERT_EQ("Angular velocity", xml.real_model_variables.at(7).attributes.quantity);
    ASSERT_EQ("Angular velocity", xml.real_model_variables.at(8).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(9).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(10).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(11).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(12).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(13).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(14).attributes.quantity);
    ASSERT_EQ("", xml.real_model_variables.at(15).attributes.quantity);
    ASSERT_EQ(0, xml.real_model_variables.at(0).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(1).attributes.start);
    ASSERT_DOUBLE_EQ(-0.099, xml.real_model_variables.at(2).attributes.start);
    ASSERT_DOUBLE_EQ(10*0.51444444444444444444, xml.real_model_variables.at(3).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(4).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(5).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(6).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(7).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(8).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(13).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(14).attributes.start);
    ASSERT_EQ(0, xml.real_model_variables.at(15).attributes.start);
    ASSERT_TRUE(xml.type_definitions.empty());
    ASSERT_TRUE(xml.vendor_annotations.empty());
}

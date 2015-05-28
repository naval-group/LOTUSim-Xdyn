/*
 * EmitFMIXmlTest.cpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#include "EmitFMIXmlTest.hpp"
#include "EmitFMIXml.hpp"
#include "ParseFMIXml.hpp"

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

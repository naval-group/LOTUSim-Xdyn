/*
 * parse_fmi_xml.hpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#ifndef PARSE_FMI_XML_HPP_
#define PARSE_FMI_XML_HPP_

#include <cstddef>
#include <string>
#include <vector>


namespace fmi
{
    struct DateTime
    {
        DateTime();
        size_t year;
        size_t month;
        size_t day;
        size_t hours;
        size_t minutes;
        size_t seconds;
    };

    enum class NamingConvention {FLAT, STRUCTURED};

    struct Attributes
    {
        Attributes();
        std::string fmiVersion;
        std::string modelName;
        std::string modelidentifier;
        std::string guid;
        std::string description;
        std::string author;
        std::string version;
        std::string generationTool;
        DateTime generationDateAndTime;
        NamingConvention variableNamingConvention;
        size_t numberOfEventsIndicators;
    };

    struct DisplayUnitDefinition
    {
        DisplayUnitDefinition();
        std::string displayUnit;
        double gain;
        double offset;
    };

    struct BaseUnit
    {
        BaseUnit();
        std::string unit;
        std::vector<DisplayUnitDefinition> definitions;
    };

    struct DefaultExperiment
    {
        DefaultExperiment();
        double startTime;
        double stopTime;
        double tolerance;
    };

    struct Annotation
    {
        Annotation();
        std::string name;
        std::string value;
    };

    struct Tool
    {
        Tool();
        std::string name;
        std::vector<Annotation> annotations;
    };

    struct RealType
    {
        RealType();
        std::string quantity;
        std::string unit;
        std::string displayUnit;
        bool relativeQuantity;
        double min;
        double max;
        double nominal;
    };

    template <typename T> struct Type
    {
        Type() : name(), description() {}
        std::string name;
        std::string description;
        T type_;
    };

    enum class Variability {CONSTANT, PARAMETER, DISCRETE, CONTINUOUS};
    enum class Causality {INPUT, OUTPUT, INTERNAL, NONE};
    enum class Alias {NO_ALIAS, ALIAS, NEGATED_ALIAS};

    struct RealAttributes
    {
        RealAttributes();
        double declaredType;
        std::string quantity;
        std::string unit;
        std::string displayUnit;
        bool relativeQuantity;
        double min;
        double max;
        double nominal;
        double start;
        bool fixed;
    };

    template <typename T> struct ScalarVariable
    {
        ScalarVariable() : name(), valueReference(), description(), variability(), causality(), alias(Alias::NO_ALIAS), attributes(), direct_dependency() {}
        std::string name;
        size_t valueReference;
        std::string description;
        Variability variability;
        Causality causality;
        Alias alias;
        T attributes;
        std::vector<std::string> direct_dependency;
    };

    struct Xml
    {
        Xml();
        Attributes attributes;
        std::vector<BaseUnit> UnitDefinitions;
        DefaultExperiment default_experiment;
        std::vector<Tool> vendor_annotations;
        std::vector<Type<RealType> > type_definitions;
        std::vector<ScalarVariable<RealAttributes> > real_model_variables;
    };
}

#endif  /* PARSE_FMI_XML_HPP_ */

/*
 * parse_fmi_xml.cpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#include "FMIXml.hpp"

fmi::DateTime::DateTime() : year(), month(), day(), hours(), minutes(), seconds()
{
}

fmi::Attributes::Attributes() :
    fmiVersion(),
    modelName(),
    modelidentifier(),
    guid(),
    description(),
    author(),
    version(),
    generationTool(),
    generationDateAndTime(),
    variableNamingConvention(),
    numberOfEventsIndicators(),
    numberOfContinuousStates()
{
}

fmi::DisplayUnitDefinition::DisplayUnitDefinition() :
        displayUnit(),
        gain(),
        offset()
{
}

fmi::BaseUnit::BaseUnit():
        unit(),
        definitions()
{
}

fmi::DefaultExperiment::DefaultExperiment():
        startTime(),
        stopTime(),
        tolerance()
{
}

fmi::Annotation::Annotation() :
        name(),
        value()
{
}

fmi::Tool::Tool() :
    name(),
    annotations()
{}

fmi::RealType::RealType() :
    quantity(),
    unit(),
    displayUnit(),
    relativeQuantity(),
    min(),
    max(),
    nominal()
{
}

fmi::RealAttributes::RealAttributes() :
        declaredType(),
        quantity(),
        unit(),
        displayUnit(),
        relativeQuantity(),
        min(),
        max(),
        nominal(),
        start(),
        fixed()
{}

fmi::Xml::Xml() :
        attributes(),
        UnitDefinitions(),
        default_experiment(),
        vendor_annotations(),
        type_definitions(),
        real_model_variables()
{}

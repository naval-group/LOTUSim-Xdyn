/*
 * random_FMI_XML.cpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */
#include "FMIXml.hpp"
#include <ssc/random_data_generator.hpp>
namespace ssc
{
    namespace random_data_generator
    {
        template <> fmi::Annotation TypedScalarDataGenerator<fmi::Annotation>::get() const
        {
            fmi::Annotation ret;
            ret.name = random<std::string>();
            ret.value = random<std::string>();
            return ret;
        }

        template <> fmi::Tool TypedScalarDataGenerator<fmi::Tool>::get() const
        {
            fmi::Tool ret;
            ret.annotations = random_vector_of<fmi::Annotation>();
            ret.name = random<std::string>();
            return ret;
        }

        template <> fmi::RealType TypedScalarDataGenerator<fmi::RealType>::get() const
        {
            fmi::RealType ret;
            ret.displayUnit = random<std::string>();
            ret.quantity = random<std::string>();
            ret.unit = random<std::string>();
            ret.max = random<double>();
            ret.min = random<double>();
            ret.nominal = random<double>();
            ret.relativeQuantity = random<bool>();
            return ret;
        }

        template <> fmi::Type<fmi::RealType> TypedScalarDataGenerator<fmi::Type<fmi::RealType> >::get() const
        {
            fmi::Type<fmi::RealType> ret;
            ret.description = random<std::string>();
            ret.name = random<std::string>();
            ret.type_ = random<fmi::RealType>();
            return ret;
        }

        template <> fmi::Variability TypedScalarDataGenerator<fmi::Variability>::get() const
        {
            const size_t i = random<size_t>().between(0,3);
            if (i==0)
            {
                return fmi::Variability::CONSTANT;
            }
            if (i==1)
            {
                return fmi::Variability::CONTINUOUS;
            }
            if (i==2)
            {
                return fmi::Variability::DISCRETE;
            }
            return fmi::Variability::PARAMETER;
        }

        template <> fmi::Causality TypedScalarDataGenerator<fmi::Causality>::get() const
        {
            const size_t i = random<size_t>().between(0,3);
            if (i==0)
            {
                return fmi::Causality::INPUT;
            }
            if (i==1)
            {
                return fmi::Causality::INTERNAL;
            }
            if (i==2)
            {
                return fmi::Causality::NONE;
            }
            return fmi::Causality::OUTPUT;
        }

        template <> fmi::Alias TypedScalarDataGenerator<fmi::Alias>::get() const
        {
            const size_t i = random<size_t>().between(0,2);
            if (i==0)
            {
                return fmi::Alias::ALIAS;
            }
            if (i==1)
            {
                return fmi::Alias::NEGATED_ALIAS;
            }
            return fmi::Alias::NO_ALIAS;
        }

        template <> fmi::RealAttributes TypedScalarDataGenerator<fmi::RealAttributes>::get() const
        {
            fmi::RealAttributes ret;
            ret.declaredType = random<std::string>();
            ret.displayUnit = random<std::string>();
            ret.quantity = random<std::string>();
            ret.unit = random<std::string>();
            ret.fixed = random<bool>();
            ret.max = random<double>();
            ret.min = random<double>();
            ret.nominal = random<double>();
            ret.start = random<double>();
            ret.relativeQuantity = random<bool>();
            return ret;
        }

        template <> fmi::ScalarVariable<fmi::RealAttributes> TypedScalarDataGenerator<fmi::ScalarVariable<fmi::RealAttributes>>::get() const
        {
            fmi::ScalarVariable<fmi::RealAttributes> ret;
            ret.alias = random<fmi::Alias>();
            ret.attributes = random<fmi::RealAttributes>();
            ret.causality = random<fmi::Causality>();
            ret.description = random<std::string>();
            ret.direct_dependency = random_vector_of<std::string>();
            ret.name = random<std::string>();
            ret.valueReference = random<size_t>();
            ret.variability = random<fmi::Variability>();
            return ret;
        }

        template <> fmi::DefaultExperiment TypedScalarDataGenerator<fmi::DefaultExperiment>::get() const
        {
            fmi::DefaultExperiment ret;
            ret.startTime = random<double>();
            ret.stopTime = random<double>();
            ret.tolerance = random<double>();
            return ret;
        }

        template <> fmi::DateTime TypedScalarDataGenerator<fmi::DateTime>::get() const
        {
            fmi::DateTime ret;
            ret.day = random<size_t>().between(10,31);
            ret.hours = random<size_t>().between(12,23);
            ret.minutes = random<size_t>().between(10,59);
            ret.month = random<size_t>().between(10,12);
            ret.seconds = random<size_t>().between(10,59);
            ret.year = random<size_t>().between(1000,9999);
            return ret;
        }

        template <> fmi::NamingConvention TypedScalarDataGenerator<fmi::NamingConvention>::get() const
        {
            size_t i = random<size_t>().between(0,1);
            if (i == 0)
            {
                return fmi::NamingConvention::FLAT;
            }
            return fmi::NamingConvention::STRUCTURED;
        }

        template <> fmi::Attributes TypedScalarDataGenerator<fmi::Attributes>::get() const
        {
            fmi::Attributes ret;
            ret.author = random<std::string>();
            ret.description = random<std::string>();
            ret.fmiVersion = random<std::string>();
            ret.generationDateAndTime = random<fmi::DateTime>();
            ret.generationTool = random<std::string>();
            ret.guid = random<std::string>();
            ret.modelName = random<std::string>();
            ret.modelidentifier = random<std::string>();
            ret.numberOfEventsIndicators = random<size_t>();
            ret.variableNamingConvention = random<fmi::NamingConvention>();
            ret.version = random<std::string>();
            ret.numberOfContinuousStates = random<size_t>();
            return ret;
        }

        template <> fmi::DisplayUnitDefinition TypedScalarDataGenerator<fmi::DisplayUnitDefinition>::get() const
        {
            fmi::DisplayUnitDefinition ret;
            ret.displayUnit = random<std::string>();
            ret.gain = random<double>().between(-10,10);
            ret.offset = random<double>().between(-10,10);
            return ret;
        }

        template <> fmi::BaseUnit TypedScalarDataGenerator<fmi::BaseUnit>::get() const
        {
            fmi::BaseUnit ret;
            ret.definitions = random_vector_of<fmi::DisplayUnitDefinition>();
            ret.unit = random<std::string>();
            return ret;
        }

        template <> fmi::Xml TypedScalarDataGenerator<fmi::Xml>::get() const
        {
            fmi::Xml ret;
            ret.UnitDefinitions = random_vector_of<fmi::BaseUnit>();
            ret.attributes = random<fmi::Attributes>();
            ret.default_experiment = random<fmi::DefaultExperiment>();
            ret.real_model_variables = random_vector_of<fmi::ScalarVariable<fmi::RealAttributes> >();
            ret.type_definitions = random_vector_of<fmi::Type<fmi::RealType> >();
            ret.vendor_annotations = random_vector_of<fmi::Tool>();
            return ret;
        }
    }
}

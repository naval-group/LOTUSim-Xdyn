/*
 * UWCurrentModel.hpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

#ifndef ENVIRONMENT_MODELS_INC_UWCurrentMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_UWCurrentMODEL_HPP_

#include <Eigen/Dense>
#include <memory>

#include <ssc/macros.hpp>
#include <boost/optional/optional.hpp>
#include "yaml-cpp/exceptions.h"

#include "xdyn/external_data_structures/YamlModel.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"

class UWCurrentModel;
typedef std::shared_ptr<UWCurrentModel> UWCurrentModelPtr;
typedef std::function<boost::optional<UWCurrentModelPtr>(const YamlModel&)> UWCurrentParser;

class UWCurrentModel
{
public:
    UWCurrentModel();
    virtual ~UWCurrentModel();
    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, const double t, const double wave_height) const=0;

    template <typename UWCurrentType>
    static UWCurrentParser build_parser()
    {
        auto parser = [](const YamlModel& yaml) -> boost::optional<UWCurrentModelPtr>
        {
            boost::optional<UWCurrentModelPtr> ret;
            if (yaml.model == UWCurrentType::model_name())
            {
                std::string context = "Invalid input data for model '" + UWCurrentType::model_name() + "'.";
                try
                {
                    ret.reset(UWCurrentModelPtr(new UWCurrentType(UWCurrentType::parse(yaml.yaml))));
                }
                catch (const InvalidInputException& exception)
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "The error was: " << exception.get_message() << std::endl << "Model containing error is defined line "
                          << yaml.index_of_first_line_in_global_yaml << " of the YAML file." << std::endl);
                }
                catch (const YAML::Exception& exception)
                {
                    const size_t line_number = yaml.index_of_first_line_in_global_yaml;
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "Model containing error is defined line "
                          << line_number << " of the YAML file." << std::endl << "The error was: " << exception.msg);
                }
            }
            return ret;
        };
        return parser;
    }
};

#endif /* ENVIRONMENT_MODELS_INC_UWCurrentMODEL_HPP_ */

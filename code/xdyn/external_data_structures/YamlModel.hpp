/*
 * YamlModel.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLMODEL_HPP_
#define YAMLMODEL_HPP_

#include <string>

struct YamlModel
{
    YamlModel(): model(), yaml(), index_of_first_line_in_global_yaml(0) {};
    YamlModel(
        const std::string& model_,
        const std::string& yaml_,
        size_t index_of_first_line_in_global_yaml_ = 0):
            model(model_),yaml(yaml_),index_of_first_line_in_global_yaml(index_of_first_line_in_global_yaml_)
        {};
    std::string model;
    std::string yaml;
    size_t      index_of_first_line_in_global_yaml; //!< Because the force parsers will treat the yaml as a new document so we provide an offset to help diagnosis
};

#endif /* YAMLMODEL_HPP_ */

#ifndef OBSERVERS_AND_API_INC_JSONSERIALIZER_HPP_
#define OBSERVERS_AND_API_INC_JSONSERIALIZER_HPP_

#include <string>

#include "YamlSimServerInputs.hpp"
#include "SimServerInputs.hpp"
#include "YamlState.hpp"

YamlSimServerInputs deserialize(const std::string& input);
std::string serialize(const std::vector<YamlState>& states);
std::string serialize(const YamlState& dx_dt);

#endif /* OBSERVERS_AND_API_INC_JSONSERIALIZER_HPP_ */

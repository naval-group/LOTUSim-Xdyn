#ifndef GRPC_INC_TOGRPCCOMMON_HPP_
#define GRPC_INC_TOGRPCCOMMON_HPP_

#include "environment_models/DiscreteDirectionalWaveSpectrum.hpp" // xdyn/code/environment_models/DiscreteDirectionalWaveSpectrum.hpp
#include "wave_types.pb.h"
#include <grpcpp/grpcpp.h>

#include <string>
#include <vector>

void flat_spectrum_response_from_vector_of_flat_discrete_directional_wave_spectra(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& spectra, SpectrumResponse* spectrum_response);
void copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination);
void copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination);

#endif /* GRPC_INC_TOGRPCCOMMON_HPP_ */

/*
 * environment_parsers.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENT_PARSERS_HPP_
#define ENVIRONMENT_PARSERS_HPP_

#include "xdyn/external_data_structures/YamlWaveModelInput.hpp"
#include "xdyn/environment_models/YamlSpectraInput.hpp"
#include "xdyn/external_data_structures/YamlGRPC.hpp"

#include <string>
#include <boost/optional/optional_io.hpp>

YamlDefaultWaveModel  parse_default_wave_model(const std::string& yaml);
YamlWaveModel         parse_waves(const std::string& yaml);
YamlDiscretization    parse_discretization(const std::string& yaml);
YamlDiracDirection    parse_wave_dirac_direction(const std::string& yaml);
YamlDiracSpectrum     parse_wave_dirac_spectrum(const std::string& yaml);
YamlJonswap           parse_jonswap(const std::string& yaml);
YamlPiersonMoskowitz  parse_pierson_moskowitz(const std::string& yaml);
YamlBretschneider     parse_bretschneider(const std::string& yaml);
YamlCos2s             parse_cos2s(const std::string& yaml);
boost::optional<int>  parse_seed_of_random_number_generator(const std::string& yaml);
YamlGRPC              parse_grpc(const std::string& yaml);

#endif  /* ENVIRONMENT_PARSERS_HPP_ */

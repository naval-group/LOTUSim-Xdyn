/*
 * YamlRAO.hpp
 *
 */

#ifndef YAMLRAO_HPP_
#define YAMLRAO_HPP_

#include <string>
#include <boost/optional.hpp>

#include "YamlCoordinates.hpp"
/*
 *     ss << "model: diffraction\n"
       << "hdb: test_ship.hdb\n"
       << "calculation point in body frame:\n"
       << "    x: {value: 0.696, unit: m}\n"
       << "    y: {value: 0, unit: m}\n"
       << "    z: {value: 1.418, unit: m}\n"
       << "mirror for 180 to 360: true\n";
 */
struct YamlRAO
{
    enum class TypeOfRao {DIFFRACTION_RAO, FROUDE_KRYLOV_RAO};
    YamlRAO();
    std::string           hdb_filename;
    std::string           precal_filename;
    YamlCoordinates       calculation_point;
    bool                  mirror;
    boost::optional<bool> use_encounter_period;
    TypeOfRao             type_of_rao;
};

#endif /* YAMLRAO_HPP_ */

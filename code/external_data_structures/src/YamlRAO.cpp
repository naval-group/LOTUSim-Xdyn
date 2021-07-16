/*
 * YamlRadiationDamping.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#include "YamlRAO.hpp"

YamlRAO::YamlRAO() : hdb_filename(),
                     precal_filename(),
                     calculation_point(),
                     mirror(false),
                     use_encounter_period(),
                     type_of_rao(TypeOfRao::DIFFRACTION_RAO)
{
}

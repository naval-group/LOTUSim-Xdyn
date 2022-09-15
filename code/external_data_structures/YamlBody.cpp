/*
 * YamlBody.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "external_data_structures/YamlBody.hpp"

YamlBody::YamlBody() :
    name(),
    mesh(),
    position_of_body_frame_relative_to_mesh(),
    initial_position_of_body_frame_relative_to_NED_projected_in_NED(),
    initial_velocity_of_body_frame_relative_to_NED_projected_in_body(),
    dynamics(),
    external_forces(),
    blocked_dof(),
    filtered_states()
{
}

YamlCSVDOF::YamlCSVDOF() :
    YamlDOF<std::string>(),
    filename()
{
}

YamlBlockedDOF::YamlBlockedDOF() : from_yaml(), from_csv()
{
}


YamlFilteredStates::YamlFilteredStates()
    : x("")
    , y("")
    , z("")
    , u("")
    , v("")
    , w("")
    , p("")
    , q("")
    , r("")
    , phi("")
    , theta("")
    , psi("")
{}
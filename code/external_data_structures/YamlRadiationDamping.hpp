/*
 * YamlRadiationDamping.hpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#ifndef YAMLRADIATIONDAMPING_HPP_
#define YAMLRADIATIONDAMPING_HPP_

#include <string>

enum class TypeOfQuadrature {RECTANGLE, TRAPEZOIDAL, SIMPSON, GAUSS_KRONROD, BURCHER, CLENSHAW_CURTIS, FILON};

#include "external_data_structures/YamlCoordinates.hpp"

struct YamlRadiationDamping
{
    YamlRadiationDamping();
    std::string         hdb_filename;                                         //!< Path to a HDB file storing the radiation damping matrices
    std::string         precal_r_filename;                                    //!< Path to a PRECAL_R file storing the radiation damping matrices
    TypeOfQuadrature    type_of_quadrature_for_cos_transform;                 //!< What integration algorithm to use to compute retardation functions?
    TypeOfQuadrature    type_of_quadrature_for_convolution;                   //!< What integration algorithm to use to compute the radiation forces (convolution)?
    size_t              nb_of_points_for_retardation_function_discretization; //!< How many points to use to compute the spline interpolating the retardation functions?
    double              omega_min;                                            //!< Lower bound of the cosine integral used to calculate the retardation functions from the radiation dampings
    double              omega_max;                                            //!< Upper bound of the cosine integral used to calculate the retardation functions from the radiation dampings
    double              tau_min;                                              //!< Lower bound of the convolution integral, to calculate Fr
    double              tau_max;                                              //!< Upper bound of the convolution integral, to calculate Fr
    bool                output_Br_and_K;                                      //!< Should the program output
    YamlCoordinates     calculation_point_in_body_frame;                      //!< Where were the damping matrices (read from the HDB file) computed?
    bool                remove_constant_speed;                                //!< Should the constant (mean) velocity be subtracted from the total velocity before in this model
    bool                forward_speed_correction;                             //!< Should a forward speed correction be applied (if the HDB results are at zero forward speed)
};

#endif /* YAMLRADIATIONDAMPING_HPP_ */

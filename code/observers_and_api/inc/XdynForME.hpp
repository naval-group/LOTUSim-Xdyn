/*
 * XdynForME.hpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_INC_XDYNFORME_HPP_
#define OBSERVERS_AND_API_INC_XDYNFORME_HPP_

#include <string>

#include "SimServerInputs.hpp"
#include "ConfBuilder.hpp"
#include "YamlState.hpp"

class XdynForME
{
    public :
        XdynForME(const std::string& yaml_model);
        YamlState handle(const YamlSimServerInputs& request);
        YamlState handle(const SimServerInputs& request);
        double get_Tmax() const;

    private :
        XdynForME();
        ConfBuilder builder;
};



#endif /* OBSERVERS_AND_API_INC_XDYNFORME_HPP_ */

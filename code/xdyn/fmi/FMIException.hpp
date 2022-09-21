/*
 * FMIException.hpp
 *
 *  Created on: May 18, 2015
 *      Author: cady
 */

#ifndef FMIEXCEPTION_HPP_
#define FMIEXCEPTION_HPP_

#include <ssc/exception_handling.hpp>

namespace fmi
{
    class Exception: public ssc::exception_handling::Exception
    {
        public:
            Exception(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
                ::ssc::exception_handling::Exception(message, file, function, line)
            {
            }
    };
}

#endif  /* FMIEXCEPTION_HPP_ */

/*
 * PrecalParser.hpp
 *
 *  Created on: Jun 8, 2021
 *      Author: cady
 */

#ifndef __PRECALPARSERHPP__
#define __PRECALPARSERHPP__

#include "PrecalParserHelper.hpp"

class PrecalParser
{
    public:
        PrecalParser() = delete;
        static PrecalParser from_string(const std::string& precal_file_contents);
        static PrecalParser from_file(const std::string& path_to_precal_file);

        std::vector<double> get_vector_value(const std::string& section_title,
                                             const std::string& vector_key,
                                             const std::string& object_name,
                                             const std::string& not_found_message) const;

        Eigen::Matrix<double,6,6> get_added_mass() const;

    private:
        PrecalParser(const PrecalFile& precal_file);
        PrecalFile precal_file;


};


#endif
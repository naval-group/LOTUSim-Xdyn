/*
 * PrecalParser.hpp
 *
 *  Created on: Jun 8, 2021
 *      Author: cady
 */

#ifndef __PRECALPARSERHPP__
#define __PRECALPARSERHPP__

#include "HydroDBParser.hpp"
#include "PrecalParserHelper.hpp"
#include "TimestampedMatrix.hpp"

class PrecalParser : public HydroDBParser
{
    public:
        PrecalParser() = delete;
        static PrecalParser from_string(const std::string& precal_file_contents);
        static PrecalParser from_file(const std::string& path_to_precal_file);

        std::vector<double> get_vector_value(const std::string& section_title,
                                             const std::string& vector_key,
                                             const std::string& object_name,
                                             const std::string& not_found_message) const;
        std::string get_string_value(const std::string& section_title,
                                     const std::string& string_key,
                                     const std::string& object_name,
                                     const std::string& not_found_message) const;

        Eigen::Matrix<double,6,6> get_added_mass() const;
        void init_diffraction_tables();
        std::array<std::vector<std::vector<double> >,6 > get_diffraction_module_tables() const;
        std::array<std::vector<std::vector<double> >,6 > get_diffraction_phase_tables() const;
        std::vector<double> get_diffraction_module_periods() const;
        std::vector<double> get_diffraction_phase_periods() const;
        std::vector<double> get_diffraction_module_psis() const;
        std::vector<double> get_diffraction_phase_psis() const;


    private:
        PrecalParser(const PrecalFile& precal_file);
        PrecalFile precal_file;
        RAOData diffraction_module;
        RAOData diffraction_phase;


};


#endif
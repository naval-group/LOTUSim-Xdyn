/*
 * PrecalParser.hpp
 *
 *  Created on: Jun 8, 2021
 *      Author: cady
 */

#ifndef __PRECALPARSERHPP__
#define __PRECALPARSERHPP__

#include <boost/variant.hpp>
#include "HydroDBParser.hpp"
#include "PrecalParserHelper.hpp"

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
        /**
         * @brief Get the angular frequencies for the radiation damping and added mass matrices
         */
        std::vector<double> get_angular_frequencies() const;
        /**
         * @brief Speed at which the radiation damping matrices were calculated.
         * Used to determine if we can apply a forward-speed correction to the radiation damping
         * force model.
         */
        double get_forward_speed() const;
        
        /**
         * @brief Added mass matrix coefficient Ma(i,j) = f(omega) for all omegas
         */
        std::vector<double> get_added_mass_coeff(const size_t i, const size_t j) const;


    private:
        PrecalParser(const PrecalFile& precal_file);
        PrecalFile precal_file;
        boost::variant<RAOData,std::string> diffraction_module;
        boost::variant<RAOData,std::string> diffraction_phase;

};


#endif
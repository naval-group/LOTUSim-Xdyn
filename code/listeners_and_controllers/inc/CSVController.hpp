/*
 * CSVController.hpp
 *
 *  Created on: Mar, 16 2022
 *      Author: cady
 */

#ifndef CSVCONTROLLER_HPP_
#define CSVCONTROLLER_HPP_

#include "Controller.hpp"
#include "State.hpp"
#include <ssc/data_source.hpp>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>

/*
 * Tests for CSVController are found in `observers_and_api/unit_tests/src/CSVControllerTest.cpp`
 * because these tests need a Sim instance, which requires the observers_and_api include directory.
 * To avoid cross-dependencies, the corresponding unit tests are moved to
 * observers_and_api/unit_tests.
 */

class CSVController : public Controller
{
  public:
    struct Yaml
    {
        Yaml(const std::string& yaml);
        std::string path; //!< Path to CSV file containing the commands
        std::string time_column; //!< Title of the time column
        char separator; //!< Column separator (either ',' or ';')
        bool shift_time_column; //!< If set to true, the time values are shifted so the first time
                                //!< value in the CSV matches the beginning of the simulation.
                                //!< Otherwise, the time values read from the CSV file are left
                                //!< unchanged.
        std::map<std::string,std::string> commands; //!< Command name to column name mapping
    };

    CSVController(const double tstart, const std::string& yaml);

    std::vector<std::string> get_command_names() const;

    const Yaml yaml; //!< Controller-specific yaml

  private:
    /**
     * @brief Updates the controller output value in the datasource
     *
     * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
     * @param time Current simulation time (in seconds).
     * @param system The continuous system. Used to retrieve the continuous states.
     */
    void update_discrete_states(const double time, ssc::solver::ContinuousSystem* system);

    /**
     * @brief Get the date of next discrete state update (next line in CSV file)
     */
    double get_date_of_next_update(const double current_time) const override;

    struct InputFile
    {
        InputFile(const Yaml& y);
        std::unordered_map<std::string, double> get_commands() const;
        void read_next_line();
        /**
         * @brief True if the CSV file has been read entirely (has_more_value is false)
         * 
         * @return true we reached the end of file, false otherwise.
         */
        bool eof() const;

        /**
         * @brief Get the date of the next line in the CSV (current buffer value).
         */
        double get_current_date() const;

        private:
            InputFile() = delete;
            InputFile(const std::string&) = delete;
            std::vector<std::string> get_headers();

            const Yaml yaml;
            std::ifstream file;
            std::vector<std::string> headers;
            std::unordered_map<std::string, std::string> column_name_to_command_name;
            std::unordered_map<std::string, double> current_values;
            double current_date;
            bool has_more_values;
    };

    InputFile csv;
};

#endif /* CSVCONTROLLER_HPP_ */

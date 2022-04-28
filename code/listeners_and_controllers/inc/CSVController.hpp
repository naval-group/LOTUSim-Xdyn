/*
 * CSVController.hpp
 *
 *  Created on: Mar, 16 2022
 *      Author: cady
 */

#ifndef CSVCONTROLLER_HPP_
#define CSVCONTROLLER_HPP_

#include "Controller.hpp"
#include "CSVLineByLineReader.hpp"
#include <string>
#include <map>


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

    CSVController(const double tstart //!< Date of beginning of simulation (usually 0): this is needed by the parent class, but the date of the first call can only be known after reading the first line of the CSV file (or if shift_time_column is true)
                , const std::string& name, const std::string& yaml);

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
    void update_discrete_states(const double time, ssc::solver::ContinuousSystem& system);

    /**
     * @brief Get the date of next discrete state update (next line in CSV file)
     */
    double get_date_of_next_update(const double current_time) const override;


    /**
     * @brief Initializes member tstart on the first controller call: for the CSV controller, this
     * is the first simulation step. This value does not necessarily correspond to the date of the
     * first line in the CSV file: it is used to shift the time values of the CSV file
     * (if shift_time_column is set to true). When it is used, all dates in the CSV file are shifted
     * so the first date (on the CSV's first line) matches the simulation tstart.
     * @param t Current simulation time
     */
    void initialize_tstart_on_first_call(const double t);

    /**
     * @brief Calculates a shifted time value if shift_time_column is set to true. If shift_time_column
     * is false, simply return the 'time' input unchanged. If the time is shifted, it is done so the
     * date of the first line in the CSV file matches the simulation start date. This means only the
     * time span between two consecutive lines of the CSV file is preserved: all original dates will
     * be changed.
     * @param time Current simulation time
     * @return Shifted time (or just 'time' if no shift was requested)
     */
    double shift_time_if_necessary(const double time) const;

    CSVLineByLineReader csv;
    double tstart; //!< Date of the first simulation step (used to shift the time column if necessary)
    bool got_tstart;
};

#endif /* CSVCONTROLLER_HPP_ */

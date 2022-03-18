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
    };

    CSVController(const double tstart, const std::string& yaml);

    std::vector<std::string> get_command_names() const;
    std::string get_name() const;

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

    /** \brief Gets the value of the controller measured input used by `compute_command` from the
     * system states
     */
    double get_measured_value(const ssc::solver::ContinuousSystem* sys) const;

    /** \brief Computes the command value from the input data
     */
    double compute_command(const double setpoint, const double measured_value, const double t);

    /**
     * @brief Get the date of next discrete state update (next line in CSV file)
     */
    double get_date_of_next_update(const double current_time) const override;
};

#endif /* CSVCONTROLLER_HPP_ */

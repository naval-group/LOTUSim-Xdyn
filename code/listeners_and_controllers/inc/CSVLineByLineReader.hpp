#ifndef CSVLINEBYLINEREADERHPP
#define CSVLINEBYLINEREADERHPP

#include "CSVYaml.hpp"
#include <fstream>
#include <unordered_map>
#include <vector>


class CSVLineByLineReader
{
    public:
        CSVLineByLineReader(const CSVYaml& y);
        /**
         * @brief Get values in the CSV for a given date. Updates the position in the CSV.
         * @details If the CSV contains t0 and t1, querying t for t0 <= t < t1 will return
         * values in the t0 line.
         * @param t The date (will be )
         * @return Hash map containing all values
         */
        std::unordered_map<std::string, double> get_values(const double t);

        /**
         * @brief Get the date of the next line in the CSV
         * 
         * @return Date of the next line in the CSV. 
         */
        double get_next_date() const;

        /**
         * @brief True if the CSV file has been read entirely (has_more_value is false)
         * 
         * @return true we reached the end of file, false otherwise.
         */
        bool eof() const;

    private:
        struct DateValues
        {
            DateValues();
            DateValues(const std::map<std::string,std::string>& commands2columns);
            double date;
            std::unordered_map<std::string, double> values;
        };
        CSVLineByLineReader() = delete;
        CSVLineByLineReader(const std::string&) = delete;
        /**
         * @brief Reads the next line from the CSV. Returns true if we haven't reached end-of-file.
         * 
         * @return true if there are lines left in the CSV, false if we reached EOF.
         */
        DateValues read_next_line();

        std::vector<std::string> get_headers();
        void set_read_position(const double date); //!< Sets the position in the stream buffer so 'date' is between current_date and next_date

        const CSVYaml yaml;
        std::ifstream file;
        std::vector<std::string> headers;
        std::unordered_map<std::string, std::string> column_name_to_command_name;
        DateValues current;
        DateValues next;
        bool has_more_values;
        std::map<double, std::streampos> date2position; //!< For each date, stores the position in the CSV file
        std::streampos position_of_first_line;
        const DateValues zero; //!< Default values (everything at zero, date at infinity)
        double max_date_so_far;
};

#endif // CSVLINEBYLINEREADERHPP

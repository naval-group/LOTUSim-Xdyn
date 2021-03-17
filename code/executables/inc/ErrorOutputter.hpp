#ifndef ERROROUTPUTTER_HPP
#define ERROROUTPUTTER_HPP

#include <sstream>
#include <string>

class ErrorOutputter
{
    public:
        enum class Status
        {
            OK,
            INVALID_INPUT,
            SIMULATION_ERROR
        };
        ErrorOutputter();
        void invalid_request(const std::string &function, const int line);
        void empty_history();
        void invalid_state_size(const std::string& state_name, const size_t state_size, const size_t t_size);
        void simulation_error(const std::string& error_message);

        bool contains_errors() const;
        std::string get_message() const;
        Status get_status() const;

      private:
          std::stringstream ss;
          
          Status status;

};

#endif
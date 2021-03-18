#include "ErrorOutputter.hpp"
#include "InternalErrorException.hpp"
#include "ConnexionError.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "yaml-cpp/exceptions.h"
#include "listeners.hpp"
#include "simulator_api.hpp"

#include <ssc/exception_handling.hpp>
#include <ssc/json/JSONException.hpp>
#include <ssc/solver.hpp>
#include <ssc/websocket.hpp>

#include <functional>
#include <boost/program_options.hpp>

ErrorOutputter::ErrorOutputter() : ss(), status(Status::OK)
{}

void ErrorOutputter::run_and_report_errors(const std::function<void(void)>& f)
{
    try
    {
        f();
    }
    catch(const boost::program_options::error& e)
    {
        invalid_command_line(e.what());
    }
    catch(const InvalidInputException& e)
    {
        invalid_input(e.get_message());
    }
    catch(const InternalErrorException& e)
    {
        internal_error(e.what());
    }
    catch(const MeshException& e)
    {
        invalid_input(std::string("A problem was detected with the STL file (mesh): ") + e.get_message() + "\n");
    }
    catch(const NumericalErrorException& e)
    {
        simulation_error(e.get_message());
    }
    catch(const ssc::websocket::WebSocketException& e)
    {
        std::stringstream s;
        s << "Unable to launch the websocket server: maybe the port you specified not available?" << std::endl
           << " You can run sudo lsof -i :9002 to check which process is using the port (eg. for port 9002 in this case)." << std::endl
           << "Try specifying a different port number using the -p flag." << std::endl
           << "The full error message was: " << e.what() << std::endl;
        network_error(s.str());
    }
    catch(const ConnexionError& e)
    {
        std::stringstream s;
        s << "This simulation requires xdyn to connect to a server but there was a problem with that connection: " << e.get_message() << std::endl;
        network_error(s.str());
    }
    catch(const ssc::json::Exception& e)
    {
        invalid_input(std::string("There is a syntax problem with the supplied JSON: ")+ e.get_message() + "\n");
    }
    catch(ssc::exception_handling::Exception& e)
    {
        internal_error(std::string("The following problem was detected:\n")+ e.get_message() + "\n");
    }
    catch(const YAML::Exception& e)
    {
        std::stringstream s;
        s << "There is a syntax problem with the YAML file line " << e.mark.line+1 << ", column " << e.mark.column+1 << ": " << e.msg << "." << std::endl
           << "Please note that as all YAML files supplied on the command-line are concatenated, the line number given here corresponds to the line number in the concatenated YAML." << std::endl;
        invalid_input(s.str());
    }
    catch(std::exception& e)
    {
        internal_error(e.what());
    }
}

void ErrorOutputter::invalid_request(const std::string &function, const int line)
{
    ss << "'request' is a NULL pointer in "
       << function
       << ", line "
       << line
       << ": this is an implementation error in xdyn. You should contact xdyn's support team."
       << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::empty_history()
{
    ss << "The state history provided to xdyn is empty ('t' has size 0): we need at least one value for each state to set the initial conditions.";
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::invalid_state_size(const std::string& state_name, const size_t state_size, const size_t t_size)
{
    ss << "State '"
       << state_name
       << "' has size "
       << state_size
       << ", whereas 't' has size "
       << t_size
       << ": this is a problem in the client code (caller of xdyn), not a problem with xdyn. Please ensure that '"
       << state_name
       << "' and 't' have the same size in the request's 'States' type."
       << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::invalid_command_line(const std::string& error)
{
    ss << "The command-line you supplied is not valid:" << std::endl << '\t' << error << std::endl << "Try running the program again with the -h flag to get a list of supported options." << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::invalid_input(const std::string& error_message)
{
    ss << "The input you provided is invalid & we cannot simulate. " << error_message << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorOutputter::simulation_error(const std::string& error_message)
{
    ss << "The simulation has diverged and cannot continue: " << error_message << std::endl;
    ss << "Maybe you can use another solver? For example, if you used a Euler integration scheme, maybe the simulation can be run with" << std::endl
        << "a Runge-Kutta 4 solver (--solver rk4) or a Runge-Kutta-Cash-Karp solver (--solver rkck)"<< std::endl;
    status = Status::SIMULATION_ERROR;
}

bool ErrorOutputter::contains_errors() const
{
    return status != Status::OK;
}

std::string ErrorOutputter::get_message() const
{
    return ss.str();
}

ErrorOutputter::Status ErrorOutputter::get_status() const
{
    return status;
}

void ErrorOutputter::internal_error(const std::string& error_message)
{
    ss << "The following error should never arise & is clearly a sign of a bug inxdyn: please send an email to the support team containing the following:" << std::endl
          << "- Input YAML file(s) + STL (if needed)" << std::endl
          << "- Command-line arguments" << std::endl
          << "- The following error message: " << std::endl
          << error_message << std::endl;
    status = Status::INTERNAL_ERROR;
}

void ErrorOutputter::network_error(const std::string& error_message)
{
    ss << "Network error: " << error_message;
    status = Status::NETWORK_ERROR;
}
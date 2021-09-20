#include <boost/algorithm/string.hpp> // For boost::replace_all
#include "ErrorReporter.hpp"
#include "InternalErrorException.hpp"
#include "ConnexionError.hpp"
#include "MeshException.hpp"
#include "GRPCError.hpp"
#include "NumericalErrorException.hpp"
#include "yaml-cpp/exceptions.h"
#include "listeners.hpp"
#include "simulator_api.hpp"

#include <ssc/exception_handling.hpp>
#include <ssc/json/JSONException.hpp>
#include <ssc/solver.hpp>
#include <ssc/websocket.hpp>
#include <ssc/text_file_reader.hpp>

#include <functional>
#include <boost/program_options.hpp>
#include <sstream>
#include <iomanip>

ErrorReporter::ErrorReporter() : ss(), status(Status::OK)
{}

std::string replace_whitespace(std::string str);
std::string replace_whitespace(std::string str)
{
    boost::replace_all(str, " ", "⸱");
    boost::replace_all(str, "\t", "→");
    return str;
}

std::string dump(const std::string& yaml, const int problematic_line);
std::string dump(const std::string& yaml, const int problematic_line)
{
    std::istringstream iss(yaml);
    int line_number = 1;
    std::stringstream ss;
    for (std::string line; std::getline(iss, line); line_number++) {}
    iss.clear();//Reset the flags only
    iss.seekg(0, iss.beg);//Move the get pointer to the beginning of the stringstream
    const int total_nb_of_lines = line_number;
    ss << total_nb_of_lines;
    const size_t width = ss.str().length();
    ss.str("");
    line_number = 1;
    for (std::string line; std::getline(iss, line); line_number++)
    {
        if (line_number == problematic_line)
        {
            ss << "> ";
        }
        else
        {
            ss << "  ";
        }
        ss << std::setw(width) << line_number << " | " << replace_whitespace(line) << std::endl;
    }
    return ss.str();
}

void ErrorReporter::run_and_report_errors(const std::function<void(void)>& f, const bool dump_yaml, const std::string& yaml_dump)
{
    int problematic_line = 0;
    status = Status::OK;
    // May be more efficient than ss.str("") (cf. https://stackoverflow.com/a/20792/1042071)
    ss.str(std::string());
    // See https://stackoverflow.com/questions/2848087/how-to-clear-stringstream for why we need ss.clear()
    ss.clear();
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
    catch(const TextFileReaderException& e)
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
    catch(const GRPCError& e)
    {
        invalid_input(std::string("A problem was detected when using a remote model (gRPC): ") + e.get_message() + "\n");
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
        problematic_line = e.mark.line+1;
    }
    catch(std::exception& e)
    {
        internal_error(e.what());
    }
    if (status != Status::OK)
    {
        if (dump_yaml && problematic_line > 0)
        {
            if (yaml_dump.empty())
            {
                ss << "No YAML content was found or there was an error before xdyn even got to the parsing stage." << std::endl;
            }
            else
            {
                ss << "The concatenatd YAML seen by xdyn was:" << std::endl << dump(yaml_dump, problematic_line) << std::endl;
            }
        }
        std::cerr << ss.str();
    }
}

void ErrorReporter::run_and_report_errors_with_yaml_dump(const std::function<void(void)>& f, const std::string& yaml_dump)
{
    run_and_report_errors(f, true, yaml_dump);
}

void ErrorReporter::run_and_report_errors_without_yaml_dump(const std::function<void(void)>& f)
{
    run_and_report_errors(f, false, "");
}

void ErrorReporter::invalid_request(const std::string &function, const int line)
{
    ss << "'request' is a NULL pointer in "
       << function
       << ", line "
       << line
       << ": this is an implementation error in xdyn. You should contact xdyn's support team."
       << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorReporter::empty_history()
{
    ss << "The state history provided to xdyn is empty ('t' has size 0): we need at least one value for each state to set the initial conditions.";
    status = Status::INVALID_INPUT;
}

void ErrorReporter::invalid_state_size(const std::string& state_name, const size_t state_size, const size_t t_size)
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

void ErrorReporter::invalid_command_line(const std::string& error)
{
    ss << "The command-line you supplied is not valid:" << std::endl << '\t' << error << std::endl << "Try running the program again with the -h flag to get a list of supported options." << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorReporter::invalid_input(const std::string& error_message)
{
    ss << "The input you provided is invalid & we cannot simulate. " << error_message << std::endl;
    status = Status::INVALID_INPUT;
}

void ErrorReporter::simulation_error(const std::string& error_message)
{
    ss << "The simulation has diverged and cannot continue: " << error_message << std::endl;
    ss << "Maybe you can use another solver? For example, if you used a Euler integration scheme, maybe the simulation can be run with" << std::endl
        << "a Runge-Kutta 4 solver (--solver rk4) or a Runge-Kutta-Cash-Karp solver (--solver rkck)"<< std::endl;
    status = Status::SIMULATION_ERROR;
}

bool ErrorReporter::contains_errors() const
{
    return status != Status::OK;
}

std::string ErrorReporter::get_message() const
{
    return ss.str();
}

ErrorReporter::Status ErrorReporter::get_status() const
{
    return status;
}

void ErrorReporter::internal_error(const std::string& error_message)
{
    ss << "The following error should never arise & is clearly a sign of a bug in xdyn: please send an email to the support team containing the following:" << std::endl
          << "- Input YAML file(s) + STL (if needed)" << std::endl
          << "- Command-line arguments" << std::endl
          << "- The following error message: " << std::endl
          << error_message << std::endl;
    status = Status::INTERNAL_ERROR;
}

void ErrorReporter::network_error(const std::string& error_message)
{
    ss << "Network error: " << error_message;
    status = Status::NETWORK_ERROR;
}

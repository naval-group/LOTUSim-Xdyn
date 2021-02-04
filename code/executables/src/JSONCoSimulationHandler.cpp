#include <boost/algorithm/string/replace.hpp>

#include "report_xdyn_exceptions_to_user.hpp"
#include "parse_history.hpp"

#include "JSONCoSimulationHandler.hpp"

std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

JSONCoSimulationHandler::JSONCoSimulationHandler(const TR1(shared_ptr)<XdynForCS>& simserver, const bool verbose_):
        sim_server(simserver),
        verbose(verbose_)
{
}

JSONCoSimulationHandler::~JSONCoSimulationHandler()
{
}

void JSONCoSimulationHandler::operator()(const ssc::websocket::Message& msg)
{
    const std::string input_json = msg.get_payload();
    if (verbose)
    {
        std::cout << current_date_time() << " Received: " << input_json << std::endl;
    }
    const std::function<void(const std::string&)> quiet_error_outputter = [&msg](const std::string& what) {msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const std::function<void(const std::string&)> verbose_error_outputter = [&msg](const std::string& what) {std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const auto error_outputter = verbose ? verbose_error_outputter : quiet_error_outputter;
    const std::function<void(void)> quiet_f = [&msg, this, &input_json]() {msg.send_text(encode_YamlStates(this->sim_server->play_one_step(input_json)));};
    const std::function<void(void)> verbose_f = [&msg, this, &input_json]() {const std::string json = encode_YamlStates(this->sim_server->play_one_step(input_json)); std::cout << current_date_time() << " Sending: " << json << std::endl; msg.send_text(json);};
    const std::function<void(void)> f = verbose ? verbose_f : quiet_f;
    report_xdyn_exceptions_to_user(f, error_outputter);
}

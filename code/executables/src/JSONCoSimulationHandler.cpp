#include <boost/algorithm/string/replace.hpp>

#include "report_xdyn_exceptions_to_user.hpp"
#include "JSONSerializer.hpp"

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
    const auto f = [&input_json, this, &msg]()
        {
            SimServerInputs server_inputs(deserialize(input_json), sim_server->get_Tmax());
            const std::vector<YamlState> dx_dt = sim_server->handle(server_inputs);
            const std::string output_json = serialize(dx_dt);
            if (verbose)
            {
                std::cout << current_date_time() << " Sending: " << output_json << std::endl;
            }
            msg.send_text(output_json);
        };
    report_xdyn_exceptions_to_user(f, error_outputter);
}

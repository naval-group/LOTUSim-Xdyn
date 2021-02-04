#include <boost/algorithm/string/replace.hpp>
#include <iomanip> // std::setprecision

#include "report_xdyn_exceptions_to_user.hpp"
#include "HistoryParser.hpp"

#include "JSONModelExchangeHandler.hpp"

std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

JSONModelExchangeHandler::JSONModelExchangeHandler(const TR1(shared_ptr)<XdynForME>& simserver, const bool verbose_):
        sim_server(simserver),
        verbose(verbose_)
{
}

JSONModelExchangeHandler::~JSONModelExchangeHandler()
{
}

void JSONModelExchangeHandler::operator()(const ssc::websocket::Message& msg)
{
    const std::string input_json = msg.get_payload();
    if (verbose)
    {
        std::cout << current_date_time() << " Received: " << input_json << std::endl;
    }
    const std::function<void(const std::string&)> quiet_error_outputter = [&msg](const std::string& what) {msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const std::function<void(const std::string&)> verbose_error_outputter = [&msg](const std::string& what) {std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const auto error_outputter = verbose ? verbose_error_outputter : quiet_error_outputter;
    const std::string input_yaml = msg.get_payload();
    const auto f =
            [&input_yaml, this, &msg]()
            {
        SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, sim_server->get_Tmax());
        const std::vector<double> dx_dt = sim_server->calculate_dx_dt(server_inputs);
        std::stringstream ss;
        // Set precision to shortest possible representation, without losing precision
        // Cf. https://stackoverflow.com/a/23437425, and, more specifically answer https://stackoverflow.com/a/4462034
        ss << std::defaultfloat << std::setprecision(17);
        ss << "{"
                << "\"dx_dt\": "  << dx_dt[0] << ","
                << "\"dy_dt\": "  << dx_dt[1] << ","
                << "\"dz_dt\": "  << dx_dt[2] << ","
                << "\"du_dt\": "  << dx_dt[3] << ","
                << "\"dv_dt\": "  << dx_dt[4] << ","
                << "\"dw_dt\": "  << dx_dt[5] << ","
                << "\"dp_dt\": "  << dx_dt[6] << ","
                << "\"dq_dt\": "  << dx_dt[7] << ","
                << "\"dr_dt\": "  << dx_dt[8] << ","
                << "\"dqr_dt\": " << dx_dt[9] << ","
                << "\"dqi_dt\": " << dx_dt[10] << ","
                << "\"dqj_dt\": " << dx_dt[11] << ","
                << "\"dqk_dt\": " << dx_dt[12]
                                           << "}";
        const std::string output_json = ss.str();
        if (verbose)
        {
            std::cout << current_date_time() << " Sending: " << output_json << std::endl;
        }
        msg.send_text(output_json);
            };
    report_xdyn_exceptions_to_user(f, error_outputter);
}

#include "observers_and_api/XdynForCS.hpp"
#include "parse_XdynForCSCommandLineArguments.hpp"
#include "XdynForCSCommandLineArguments.hpp"

#include "observers_and_api/gRPCProtoBufServer.hpp"
#include "CosimulationServiceImpl.hpp"
#include "JSONWebSocketServer.hpp"
#include "ErrorReporter.hpp"

#include <ssc/text_file_reader.hpp>

XdynForCS get_SimServer(const XdynForCSCommandLineArguments& input_data, const std::string& yaml);
XdynForCS get_SimServer(const XdynForCSCommandLineArguments& input_data, const std::string& yaml)
{
    return XdynForCS(yaml, input_data.solver, input_data.initial_timestep);
}

void start_ws_server(const XdynForCSCommandLineArguments& input_data, const std::string& yaml);
void start_ws_server(const XdynForCSCommandLineArguments& input_data, const std::string& yaml)
{
    JSONWebSocketServer<XdynForCS> server(get_SimServer(input_data, yaml), input_data.verbose);
    server.start(input_data.port, input_data.show_websocket_debug_information);
}

void start_grpc_server(const XdynForCSCommandLineArguments& input_data, const std::string& yaml);
void start_grpc_server(const XdynForCSCommandLineArguments& input_data, const std::string& yaml)
{
    XdynForCS simserver = get_SimServer(input_data, yaml);
    ErrorReporter error_outputter;
    std::shared_ptr<grpc::Service> handler(new CosimulationServiceImpl(simserver, error_outputter));
    gRPCProtoBufServer server(handler);
    server.start(input_data.port);
}

int main(int argc, char** argv)
{
    XdynForCSCommandLineArguments input_data;
    if (argc==1) return display_help(argv[0], input_data);
    int error = 0;
    ErrorReporter error_outputter;
    error_outputter.run_and_report_errors_without_yaml_dump([&error,&argc,&argv,&input_data]{error = get_input_data(argc, argv, input_data);});
    if (error)
    {
        if (error_outputter.contains_errors())
        {
            std::cerr << error_outputter.get_message() << std::endl;
        }
        return error;
    }
    if (input_data.empty() || input_data.show_help) return EXIT_SUCCESS;
    std::string yaml;
    error_outputter.run_and_report_errors_without_yaml_dump([&yaml, input_data]{const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);yaml = yaml_reader.get_contents();});
    const auto run_ws = [input_data, yaml](){
    {
        start_ws_server(input_data, yaml);
    }};
    const std::function< void(void) > run_grpc = [input_data, yaml](){
    {
        start_grpc_server(input_data, yaml);
    }};
    const std::function< void(void) > run = input_data.grpc ? run_grpc : run_ws;
    if (input_data.catch_exceptions)
    {
        error_outputter.run_and_report_errors_with_yaml_dump(run, yaml);
        if (error_outputter.contains_errors())
        {
            std::cerr << error_outputter.get_message() << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        run();
    }
    google::protobuf::ShutdownProtobufLibrary();
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

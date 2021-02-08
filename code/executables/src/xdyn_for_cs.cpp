#include "XdynForCS.hpp"
#include "report_xdyn_exceptions_to_user.hpp"
#include "parse_XdynForCSCommandLineArguments.hpp"
#include "XdynForCSCommandLineArguments.hpp"

#include "gRPCProtoBufServer.hpp"
#include "CosimulationServiceImpl.hpp"
#include "JSONWebSocketServer.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/websocket.hpp>

#include <ssc/check_ssc_version.hpp>

CHECK_SSC_VERSION(8,0)

XdynForCS get_SimServer(const XdynForCSCommandLineArguments& input_data);
XdynForCS get_SimServer(const XdynForCSCommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    return XdynForCS(yaml, input_data.solver, input_data.initial_timestep);
}

void start_ws_server(const XdynForCSCommandLineArguments& input_data);
void start_ws_server(const XdynForCSCommandLineArguments& input_data)
{
    JSONWebSocketServer<XdynForCS> server(get_SimServer(input_data), input_data.verbose);
    server.start(input_data.port, input_data.show_websocket_debug_information);
}

void start_grpc_server(const XdynForCSCommandLineArguments& input_data);
void start_grpc_server(const XdynForCSCommandLineArguments& input_data)
{
    XdynForCS simserver = get_SimServer(input_data);
    std::shared_ptr<grpc::Service> handler(new CosimulationServiceImpl(simserver));
    gRPCProtoBufServer server(handler);
    server.start(input_data.port);
}

int main(int argc, char** argv)
{
    XdynForCSCommandLineArguments input_data;
    if (argc==1) return display_help(argv[0], input_data);
    int error = 0;
    report_xdyn_exceptions_to_user([&error,&argc,&argv,&input_data]{error = get_input_data(argc, argv, input_data);}, [](const std::string& s){std::cerr << s;});
    if (error)
    {
        return error;
    }
    if (input_data.empty() || input_data.show_help) return EXIT_SUCCESS;
    const auto run_ws = [input_data](){
    {
        start_ws_server(input_data);
    }};
    const std::function< void(void) > run_grpc = [input_data](){
    {
        start_grpc_server(input_data);
    }};
    const std::function< void(void) > run = input_data.grpc ? run_grpc : run_ws;
    if (input_data.catch_exceptions)
    {
        report_xdyn_exceptions_to_user(run, [](const std::string& s){std::cerr << s;});
    }
    else
    {
        run();
    }
    google::protobuf::ShutdownProtobufLibrary();
    return error;
}

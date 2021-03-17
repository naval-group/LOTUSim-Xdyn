#include "XdynForME.hpp"
#include "parse_XdynForMECommandLineArguments.hpp"
#include "report_xdyn_exceptions_to_user.hpp"
#include "XdynForMECommandLineArguments.hpp"

#include "gRPCProtoBufServer.hpp"
#include "ModelExchangeServiceImpl.hpp"
#include "JSONWebSocketServer.hpp"
#include "gRPCErrorOutputter.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/check_ssc_version.hpp>

CHECK_SSC_VERSION(8,0)

void start_ws_server(const XdynForMECommandLineArguments& input_data);
void start_ws_server(const XdynForMECommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    JSONWebSocketServer<XdynForME> server(XdynForME(yaml), input_data.verbose);
    server.start(input_data.port, input_data.show_websocket_debug_information);
}

void start_grpc_server(const XdynForMECommandLineArguments& input_data);
void start_grpc_server(const XdynForMECommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    XdynForME xdyn(yaml);
    std::shared_ptr<ErrorOutputter> error_outputter(new gRPCErrorOutputter());
    std::shared_ptr<grpc::Service> handler(new ModelExchangeServiceImpl(xdyn, error_outputter));
    gRPCProtoBufServer server(handler);
    server.start(input_data.port);
}

int main(int argc, char** argv)
{
    XdynForMECommandLineArguments input_data;
    if (argc==1) return fill_input_or_display_help(argv[0], input_data);
    int error = 0;
    report_xdyn_exceptions_to_user([&error,&argc,&argv,&input_data]{error = get_input_data(argc, argv, input_data);}, [](const std::string& s){std::cerr << s;});
    if (error)
    {
        return error;
    }
    if (input_data.show_help)
    {
        return 0;
    }
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
    return 0;
}


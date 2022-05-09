/*
 * xdyn_grpc_airy.cpp
 *
 */
#include <exception>
#include <iostream>
#include <google/protobuf/stubs/common.h>
#include <grpcpp/grpcpp.h>
#include "gRPCChecks.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "display_command_line_arguments.hpp"
#include "ErrorReporter.hpp"
#include "AiryGRPC.hpp"
#include "simulator_api.hpp" // xdyn/code/observers_and_api/inc/simulator_api.hpp
#include "yaml_data.hpp" // To be removed late // xdyn/code/test_data_generator/inc/yaml_data.hpp
#include <ssc/text_file_reader.hpp>

const std::string xdyn_grpc_airy_description = "This is the Airy wave gRPC server, based on xdyn";

struct XdynGrpcAiryCommandLineArguments
{
    short unsigned int port;
    std::vector<std::string> yaml_filenames;
    bool catch_exceptions;
    XdynGrpcAiryCommandLineArguments(): port(50051), yaml_filenames(), catch_exceptions(false){};
};

po::options_description attach_command_line_arguments_to_options_description(XdynGrpcAiryCommandLineArguments& input_data);
po::options_description attach_command_line_arguments_to_options_description(XdynGrpcAiryCommandLineArguments& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                       "Show this help message")
        ("port,p",     po::value<short unsigned int>(&input_data.port),                  "Port for the gRPC server. Default is 50051. Available values are 1024-65535 (2^16, but port 0 is reserved and unavailable and ports in range 1-1023 are privileged (application needs to be run as root to have access to those ports)")
        ("yml,y",      po::value<std::vector<std::string> >(&input_data.yaml_filenames), "Name(s) of the YAML file(s)")
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
    ;
    return desc;
}

bool invalid(const XdynGrpcAiryCommandLineArguments& input);
bool invalid(const XdynGrpcAiryCommandLineArguments& input)
{
    if (input.port==0)
    {
        std::cerr << "Error: no port number was defined (or port 0 was defined, which is equally invalid)." << std::endl
                 << "You can define one using the -p flag. Check the available port range using the -h flag." << std::endl;
        return true;
    }
    if (input.port < 1024) // input.port is always less than 65536 because it is a short unsigned int.
    {
        std::cerr << "Error: you cannot start this gRPC server on port " << input.port << ": only range 1024-65535 is available." << std::endl;
        return true;
    }
    return false;
}

int parse_command_line_for_xdyn_grpc_airy(int argc, char **argv, XdynGrpcAiryCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    input_data.catch_exceptions = not(has.debug);
    if (has.help)
    {
        print_usage(std::cout, desc, argv[0], xdyn_grpc_airy_description);
        return EXIT_SUCCESS;
    }
    else if (invalid(input_data))
    {
        print_usage(std::cout, desc, argv[0], xdyn_grpc_airy_description);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int run(const XdynGrpcAiryCommandLineArguments& input_data, ErrorReporter& error_outputter);
int run(const XdynGrpcAiryCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    std::string yaml_input;
    if (input_data.yaml_filenames.empty())
    {
        yaml_input = test_data::simple_waves();
        std::cout << "Using default input YAML:" << std::endl;
        std::cout << yaml_input << std::endl;
    }
    else
    {
        error_outputter.run_and_report_errors_without_yaml_dump([&yaml_input,&input_data](){yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();});
    }
    EnvironmentAndFrames env;
    error_outputter.run_and_report_errors_with_yaml_dump([&env, &yaml_input](){env = get_environment_for_wave_queries(yaml_input);}, yaml_input);
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    error_outputter.run_and_report_errors_with_yaml_dump([&env, &input_data](){run_xdyn_airy_server(env, input_data.port);}, yaml_input);
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_help(char *argv, XdynGrpcAiryCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
    print_usage(std::cout, desc, argv, xdyn_grpc_airy_description);
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    int error = 0;
    XdynGrpcAiryCommandLineArguments input_data;
    ErrorReporter error_outputter;
    try
    {
        if (argc==1) return display_help(argv[0], input_data);
        error = parse_command_line_for_xdyn_grpc_airy(argc, argv, input_data);
    }
    catch(boost::program_options::error& e)
    {
        error_outputter.invalid_command_line(e.what());
        return EXIT_FAILURE;
    }
    if (error)
    {
        return error;
    }
    if (input_data.catch_exceptions)
    {
        try
        {
            return run(input_data, error_outputter);
        }
        catch (const std::exception& e)
        {
            error_outputter.internal_error(e.what());
            if (error_outputter.contains_errors())
            {
                std::cerr << error_outputter.get_message() << std::endl;
            }
            return EXIT_FAILURE;
        }
    }
    const auto ret = run(input_data, error_outputter);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}

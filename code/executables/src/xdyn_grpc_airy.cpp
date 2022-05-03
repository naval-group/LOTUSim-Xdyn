/*
 * xdyn.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <exception>
#include <iostream>
#include <google/protobuf/stubs/common.h>
#include "parse_XdynCommandLineArguments.hpp"
#include "display_command_line_arguments.hpp"
#include "ErrorReporter.hpp"
#include "AiryGRPC.hpp"

struct XdynGrpcAiryCommandLineArguments
{
    std::vector<std::string> yaml_filenames;
    bool catch_exceptions;
    bool empty() const;
    XdynGrpcAiryCommandLineArguments();
};

XdynGrpcAiryCommandLineArguments::XdynGrpcAiryCommandLineArguments() : yaml_filenames(), catch_exceptions(false)
{
}

bool XdynGrpcAiryCommandLineArguments::empty() const
{
    return yaml_filenames.empty();
}


bool invalid(const XdynGrpcAiryCommandLineArguments& input);
bool invalid(const XdynGrpcAiryCommandLineArguments& input)
{
    if (input.empty()) return true;
    if (input.yaml_filenames.empty())
    {
        std::cerr << "Error: no input YAML files defined: need at least one." << std::endl;
        return true;
    }
    return false;
}

po::options_description attach_command_line_arguments_to_options_description(XdynGrpcAiryCommandLineArguments& input_data);
po::options_description attach_command_line_arguments_to_options_description(XdynGrpcAiryCommandLineArguments& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                       "Show this help message")
        ("yml,y",      po::value<std::vector<std::string> >(&input_data.yaml_filenames), "Name(s) of the YAML file(s)")
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
    ;
    return desc;
}

int parse_command_line_for_xdyn_grpc_airy(int argc, char **argv, XdynGrpcAiryCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    input_data.catch_exceptions = not(has.debug);
    if (has.help)
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator");
        return EXIT_SUCCESS;
    }
    else if (invalid(input_data))
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int run(const XdynGrpcAiryCommandLineArguments& input_data, ErrorReporter& error_outputter);
int run(const XdynGrpcAiryCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    if (not(input_data.empty()))
    {
        run_xdyn_airy_server(create_default_spectrum());
    }
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int display_help(char *argv, XdynGrpcAiryCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
    print_usage(std::cout, desc, argv, "This is a ship simulator");
    return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    XdynGrpcAiryCommandLineArguments input_data;
    int error = 0;
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
    //const auto ret = run(input_data, error_outputter);
    const Airy airy = create_default_spectrum();
    run_xdyn_airy_server(airy);
    int ret = EXIT_SUCCESS;
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}

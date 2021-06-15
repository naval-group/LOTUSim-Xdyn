/*
 * gz.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: cady
 */



#include <google/protobuf/stubs/common.h>
#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>

#include "display_command_line_arguments.hpp"
#include "GZCurve.hpp"
#include "make_sim_for_GZ.hpp"
#include "OptionPrinter.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "ErrorReporter.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

CHECK_SSC_VERSION(10,0)
struct GZOptions
{
    GZOptions() : dphi(0), phi_max(0), stl_filename(), yaml_files(), output_csv_file()
    {}
    double dphi;
    double phi_max;
    std::string stl_filename;
    std::vector<std::string> yaml_files;
    std::string output_csv_file;
    bool empty() const
    {
        return (dphi==0) and (phi_max == 0) and stl_filename.empty() and yaml_files.empty() and output_csv_file.empty();
    }
};

bool invalid(const GZOptions& input, ErrorReporter& outputter);
bool invalid(const GZOptions& input, ErrorReporter& outputter)
{
    if (input.empty()) return true;
    if (input.stl_filename.empty())
    {
        outputter.invalid_input("No STL file defined.");
        return true;
    }
    if (input.yaml_files.empty())
    {
        outputter.invalid_input("No input YAML files defined: need at least one.");
        return true;
    }
    if (input.dphi<=0)
    {
        std::stringstream ss;
        ss << "Error: the roll step should be strictly positive. Received " << input.dphi;
        outputter.invalid_input(ss.str());
        return true;
    }
    if (input.phi_max<input.dphi)
    {
        std::stringstream ss;
        ss << "Error: the maximum roll angle should be greater than the roll step. Received " << input.phi_max;
        outputter.invalid_input(ss.str());
        return true;
    }
    return false;
}

po::options_description gz_options(GZOptions& input_data);
po::options_description gz_options(GZOptions& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                          "Show this help message")
        ("yml,y",   po::value<std::vector<std::string> >(&input_data.yaml_files),           "Path(s) to the YAML file(s)")
        ("stl,s",   po::value<std::string>(&input_data.stl_filename),                       "Path to the STL file")
        ("dphi",    po::value<double>(&input_data.dphi),                                    "Roll angle step (in degrees)")
        ("phi_max", po::value<double>(&input_data.phi_max),                                 "Maximum roll angle (in degrees)")
        ("csv,c",   po::value<std::string>(&input_data.output_csv_file)->default_value(""), "Name of the output CSV file (optional)")
    ;
    return desc;
}

int get_gz_data(int argc, char **argv, GZOptions& input_data, ErrorReporter& error_outputter);
int get_gz_data(int argc, char **argv, GZOptions& input_data, ErrorReporter& error_outputter)
{
    const po::options_description desc = gz_options(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    if (invalid(input_data, error_outputter) or has.help)
    {
        std::cerr << error_outputter.get_message() << std::endl;
        print_usage(std::cout, desc, argv[0], "Righting lever curve computer");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

template <typename T> void write(std::ostream& os, const T& v1, const T& v2, const char sep)
{
    os << v1 << sep << v2 << std::endl;
}

int main(int argc, char** argv)
{
    GZOptions input_data;
    ErrorReporter error_outputter;
    const int error = get_gz_data(argc, argv, input_data, error_outputter);
    if (not(error))
    {
        std::string yaml;
        error_outputter.run_and_report_errors_without_yaml_dump([&yaml, input_data]{const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_files);yaml = yaml_reader.get_contents();});
        const auto f = [input_data,yaml]()
            {
                const ssc::text_file_reader::TextFileReader stl_reader(input_data.stl_filename);
                const Sim sim = GZ::make_sim(yaml, stl_reader.get_contents());
                const GZ::Curve calculate(sim);
                const auto phis = calculate.get_phi(input_data.dphi*PI/180., input_data.phi_max*PI/180.);
                std::ofstream of;

                if (not(input_data.output_csv_file.empty()))
                {
                    of.open(input_data.output_csv_file.c_str(), std::ios::out);
                }

                std::ostream& os = input_data.output_csv_file.empty() ? std::cout : of;
                const char sep = input_data.output_csv_file.empty() ? '\t' : ';';
                write<std::string>(os,"Phi [deg]", "GZ(phi) [m]", sep);
                for (auto phi:phis)
                {
                    write(os, phi*180./PI, calculate.gz(phi), sep);
                }
            };
        error_outputter.run_and_report_errors_with_yaml_dump(f, yaml);
    }
    google::protobuf::ShutdownProtobufLibrary();
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

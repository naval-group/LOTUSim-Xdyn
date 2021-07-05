/*
 * PrecalParserHelper.hpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#ifndef PRECALPARSERHELPER_HPP_
#define PRECALPARSERHELPER_HPP_
#include <map>
#include <string>
#include <vector>
#include <Eigen/Dense>
struct Section
{
    Section();
    std::string title;
    std::map<std::string, double> scalar_values;
    std::map<std::string, std::string> string_values;
    std::map<std::string, std::vector<double> > vector_values;
};

struct RAOAttributes
{
    RAOAttributes();
    std::string name;
    Eigen::Vector3d position;
    double h; // Depth
    std::string  h_unit;
    double phi_a; // Roll amplitude
    std::string  phi_a_unit;
    double U; // Speed
    std::string  U_unit;
    double mu; // Waves direction
    std::string mu_unit;
    std::string  amplitude_unit;
    std::string phase_unit;
};

struct RAO
{
    RAO();
    std::string title_line;
    RAOAttributes attributes;
    std::vector<double> left_column;
    std::vector<double> right_column;
};

struct PrecalFile
{
    PrecalFile();
    std::vector<Section> sections;
    std::vector<RAO> raos;
};

PrecalFile parse_precal_from_string(const std::string& file_contents);
PrecalFile parse_precal_from_file(const std::string& filename);
RAOAttributes parse_rao_attributes(const std::string rao_attributes);

#endif // PRECALPARSERHELPER_HPP_
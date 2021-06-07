/*
 * PrecalParser.hpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#ifndef PRECALPARSER_HPP_
#define PRECALPARSER_HPP_
#include <map>
#include <string>
#include <vector>

struct Section
{
    Section();
    std::string title;
    std::map<std::string, double> scalar_values;
    std::map<std::string, std::string> string_values;
    std::map<std::string, std::vector<double> > vector_values;
};

struct RAO
{
    RAO();
    std::string title;
    std::vector<double> left_column;
    std::vector<double> right_column;
};

struct PrecalFile
{
    PrecalFile();
    std::vector<Section> sections;
    std::vector<RAO> raos;
};

PrecalFile parse_precal(const std::string& file_contents);

#endif // PRECALPARSER_HPP_
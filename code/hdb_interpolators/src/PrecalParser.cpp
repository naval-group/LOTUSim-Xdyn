#include "PrecalParser.hpp"

PrecalParser PrecalParser::from_string(const std::string& precal_file_contents)
{
    return PrecalParser(parse_precal_from_string(precal_file_contents));
}

PrecalParser PrecalParser::from_file(const std::string& path_to_precal_file)
{
    return PrecalParser(parse_precal_from_file(path_to_precal_file));
}

PrecalParser::PrecalParser(const PrecalFile& precal_file_) : precal_file(precal_file_) {}

Eigen::Matrix<double,6,6> PrecalParser::get_added_mass() const
{
    Eigen::Matrix<double,6,6> Ma;
    size_t signal_idx = 0;
    for (auto section : precal_file.sections)
    {
        if (section.title.substr(0, 6) == "signal")
        {
            const std::string section_name = section.string_values["name"];
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    std::stringstream ss;
                    ss << "A_m" << i+1 << "m" << j+1;
                    if (section_name == ss.str())
                    {
                        Ma(i,j) = precal_file.raos.at(signal_idx).left_column.front();
                    }
                }
            }
            signal_idx++;
        }
    }
    return Ma;
}
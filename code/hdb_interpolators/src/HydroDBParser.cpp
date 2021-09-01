#include "HDBParser.hpp"
#include "PrecalParser.hpp"

std::shared_ptr<HydroDBParser> parser_factory(const std::string& hdb_filename, const std::string& precal_filename)
{
    if (hdb_filename.empty())
    {
        if (precal_filename.empty())
        {
            return std::shared_ptr<HydroDBParser>();
        }
        return std::shared_ptr<HydroDBParser>(new PrecalParser(PrecalParser::from_file(precal_filename)));
    }
    return std::shared_ptr<HydroDBParser>(new HDBParser(HDBParser::from_file(hdb_filename)));
}
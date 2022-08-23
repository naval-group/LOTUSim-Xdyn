#ifndef HDBDATAHPP
#define HDBDATAHPP

#include <string>

namespace test_data
{
    std::string test_ship_hdb();
    std::string test_ship_hdb_deprecated_aquaplus_format();
    double analytical_Br(const double omega);
    double analytical_K(const double tau);
    std::string bug_3210();
    std::string bug_3210_deprecated_aquaplus_format();
    std::string bug_3230_hdb();
    std::string bug_3207_hdb();
    std::string bug_3238_hdb();
    std::string hdb_issue_184();
    std::string big_hdb();
}

#endif

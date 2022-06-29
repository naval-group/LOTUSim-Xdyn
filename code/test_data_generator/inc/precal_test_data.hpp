#ifndef PRECALTESTDATAHPP
#define PRECALTESTDATAHPP

#include <string>

namespace test_data
{
    /**\brief Return an example of a Precal file with kilo units*/
    std::string precal();

    /**\brief Return an example of a Precal file with SI units*/
    std::string precal_with_si_unit();
}

std::string glossary_of_terms();
std::string general();
std::string constants();
std::string simulation();
std::string exports();
std::string ship_particulars();
std::string mass_properties();
std::string mass_matrix();
std::string restoring_matrix();
std::string natural_periods_and_frequencies();
std::string added_mass_damping_matrix_inf_freq();
std::string hull_properties();
std::string roll_damping();
std::string labels();
std::string dimensions();
std::string signals();
std::string raos();


#endif
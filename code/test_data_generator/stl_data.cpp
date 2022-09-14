#include "stl_data.hpp"
#include <sstream>
#include <vector>

std::string test_data::single_facet()
{
    std::stringstream data;
    data << "solid MYSOLID\n"
         << "facet normal 0.4 0.4 0.2\n"
         << "    outerloop\n"
         << "    vertex  1.0 2.1 3.2\n"
         << "    vertex  2.1 3.7 4.5\n"
         << "    vertex  3.1 4.5 6.7\n"
         << "    endloop\n"
         << "endfacet\n"
         << "endsolid MYSOLID\n";
    return data.str();
}

std::string test_data::three_facets()
{
    std::stringstream data;
    data << "solid MYSOLID\n"
         << "  facet normal   0.000000E+00  0.000000E+00   1.00000\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00  0.000000E+00   1.00000\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "\n"
         << "  facet normal   0.000000E+00  0.000000E+00  -1.00000\n"
         << "    outer loop\n"
         << "      vertex   0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "endsolid MYSOLID\n";
    return data.str();
}

std::string test_data::cube()
{
    std::stringstream data;
    data << "solid MYSOLID\n"
         << "  facet normal   0.000000E+00  0.000000E+00   1.00000\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00  0.000000E+00   1.00000\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "\n"
         << "  facet normal   0.000000E+00  0.000000E+00  -1.00000\n"
         << "    outer loop\n"
         << "      vertex   0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00  0.000000E+00  -1.00000\n"
         << "    outer loop\n"
         << "      vertex   0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000      0.500000      0.000000E+00\n"
         << "      vertex   0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   -1.00000      0.000000E+00  0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   -1.00000      0.000000E+00  0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "      vertex  -0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal    1.00000      0.000000E+00  0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000     -0.500000      0.000000E+00\n"
         << "      vertex   0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal    1.00000      0.000000E+00  0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000      0.000000E+00\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00  -1.00000      0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex   0.500000     -0.500000      0.000000E+00\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00  -1.00000      0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000     -0.500000      0.000000E+00\n"
         << "      vertex   0.500000     -0.500000       1.00000\n"
         << "      vertex  -0.500000     -0.500000       1.00000\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00   1.00000      0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal   0.000000E+00   1.00000      0.000000E+00\n"
         << "    outer loop\n"
         << "      vertex  -0.500000      0.500000       1.00000\n"
         << "      vertex   0.500000      0.500000      0.000000E+00\n"
         << "      vertex  -0.500000      0.500000      0.000000E+00\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "endsolid MYSOLID\n";
    return data.str();
}

// Cf. https://gitlab.sirehna.com/sirehna/xdyn/-/merge_requests/166#note_115836
std::string test_data::cube_for_gm_test()
{
    const std::string part1 = R"(
solid OBJECT
  facet normal 0 0 1
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.3571428656578064 -0.5 1
      vertex -0.35714280605316162 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.35714280605316162 -0.3571428656578064 1
      vertex -0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0.99999999999997813 0 -2.0861626914836125e-07
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.5 -0.3571428656578064 1
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -0.99999999999997813 2.0861626914836125e-07 0
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
      vertex -0.5 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 2.0861626914836125e-07 -0.99999999999997813 0
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.5 -0.5 0.8571428656578064
      vertex -0.3571428656578064 -0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -0.99999999999997813 -2.0861626914836125e-07
    outer loop
      vertex -0.5 -0.5 1
      vertex -0.3571428656578064 -0.49999997019767761 0.8571428656578064
      vertex -0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.3571428656578064 -0.5 1
      vertex 0.3571428656578064 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.3571428656578064 -0.5 0.8571428656578064
      vertex 0.5 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 0 0.99999999999997813
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.5 -0.3571428656578064 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 0 2.0861626914836125e-07 0.99999999999997813
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
      vertex 0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.5 -0.5 0.8571428656578064
      vertex 0.5 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.5 1
      vertex 0.5 -0.3571428656578064 0.8571428656578064
      vertex 0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0.99999999999997813 0 -2.0861626914836125e-07
    outer loop
      vertex 0.5 0.5 1
      vertex 0.5 0.3571428656578064 1
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 0.99999999999997813 -2.0861626914836125e-07 0
    outer loop
      vertex 0.5 0.5 1
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
      vertex 0.5 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.5 0.5 1
      vertex 0.3571428656578064 0.5 1
      vertex 0.3571428656578064 0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.5 0.5 1
      vertex 0.3571428656578064 0.3571428656578064 1
      vertex 0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 0.99999999999997813 0
    outer loop
      vertex 0.5 0.5 1
      vertex 0.5 0.5 0.8571428656578064
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal 0 0.99999999999997813 -2.0861626914836125e-07
    outer loop
      vertex 0.5 0.5 1
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
      vertex 0.3571428656578064 0.5 1
    endloop
  endfacet
  facet normal 2.0861618210690206e-07 0 0.99999999999997813
    outer loop
      vertex -0.5 0.5 1
      vertex -0.5 0.3571428656578064 1
      vertex -0.35714280605316162 0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 0 -2.0861626914836125e-07 0.99999999999997813
    outer loop
      vertex -0.5 0.5 1
      vertex -0.35714280605316162 0.3571428656578064 0.99999997019767761
      vertex -0.3571428656578064 0.5 1
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.5 1
      vertex -0.5 0.5 0.8571428656578064
      vertex -0.5 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.5 1
      vertex -0.5 0.3571428656578064 0.8571428656578064
      vertex -0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.3571428656578064 -0.5 0
      vertex 0.3571428656578064 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.3571428656578064 -0.3571428656578064 0
      vertex 0.5 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0.99999999999997813 0 2.0861626914836125e-07
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.5 -0.3571428656578064 0
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal 0.99999999999997813 2.0861626914836125e-07 0
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
      vertex 0.5 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 -0.99999999999997813 0
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.5 -0.5 0.1428571343421936
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -0.99999999999997813 2.0861626914836125e-07
    outer loop
      vertex 0.5 -0.5 0
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
      vertex 0.3571428656578064 -0.5 0
    endloop
  endfacet
  facet normal 2.0861626914836125e-07 0 -0.99999999999997813
    outer loop
      vertex -0.5 -0.5 0
      vertex -0.5 -0.3571428656578064 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal 0 2.0861626914836125e-07 -0.99999999999997813
    outer loop
      vertex -0.5 -0.5 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
      vertex -0.3571428656578064 -0.5 0
    endloop
  endfacet
  facet normal -0.99999999999997813 0 2.0861626914836125e-07
    outer loop
      vertex -0.5 0.5 0
      vertex -0.5 0.3571428656578064 0
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal -0.99999999999997813 -2.0861626914836125e-07 0
    outer loop
      vertex -0.5 0.5 0
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
      vertex -0.5 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 0.5 0
      vertex -0.3571428656578064 0.5 0
      vertex -0.35714280605316162 0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 0.5 0
      vertex -0.35714280605316162 0.3571428656578064 0
      vertex -0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal 2.0861626914836125e-07 0.99999999999997813 0
    outer loop
      vertex -0.5 0.5 0
      vertex -0.5 0.5 0.1428571343421936
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal 0 0.99999999999997813 2.0861626914836125e-07
    outer loop
      vertex -0.5 0.5 0
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
      vertex -0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.5 0.5 0
      vertex 0.3571428656578064 0.5 0
      vertex 0.3571428656578064 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.5 0.5 0
      vertex 0.3571428656578064 0.5 0.1428571343421936
      vertex 0.5 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 0 -0.99999999999997813
    outer loop
      vertex 0.5 0.5 0
      vertex 0.5 0.3571428656578064 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal 0 -2.0861626914836125e-07 -0.99999999999997813
    outer loop
      vertex 0.5 0.5 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
      vertex 0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.5 0
      vertex 0.5 0.5 0.1428571343421936
      vertex 0.5 0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.5 0
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.5 0.1428571343421936
      vertex 0.5 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.5 0.28571429848670959
      vertex 0.5 0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.3571428656578064 0.28571429848670959
      vertex 0.5 0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.21428570151329041 0.28571429848670959
      vertex 0.5 0.21428570151329041 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.21428570151329041 0.1428571343421936
      vertex 0.5 0.21428570151329041 0
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.1428571343421936
      vertex 0.5 0.21428570151329041 0
      vertex 0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 2.086162256276226e-07 -0.99999999999997824
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.3571428656578064 0.21428570151329041 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 0 -0.99999999999997824
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
      vertex 0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.5 0.21428570151329041 0.1428571343421936
      vertex 0.5 0.071428567171096802 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.5 0.071428567171096802 0.1428571343421936
      vertex 0.5 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.5 0.071428567171096802 0
      vertex 0.3571428656578064 0.071428552269935608 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.5 0.21428570151329041 0
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.3571428656578064 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.21428570151329041 0.071428567171096802 0
      vertex 0.21428570151329041 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.21428570151329041 0.21428570151329041 0
      vertex 0.3571428656578064 0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.5 0.071428567171096802 0
      vertex 0.5 -0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.5 -0.071428567171096802 0
      vertex 0.3571428656578064 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.3571428656578064 -0.071428567171096802 0
      vertex 0.21428570151329041 -0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.3571428656578064 0.071428552269935608 0
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.071428567171096802 -0.071428574621677399 0
      vertex 0.071428567171096802 0.071428574621677399 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.071428567171096802 0.071428574621677399 0
      vertex 0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.3571428656578064 -0.071428567171096802 0
      vertex 0.3571428656578064 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.3571428656578064 -0.21428570151329041 0
      vertex 0.21428570151329041 -0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.21428570151329041 -0.21428570151329041 0
      vertex 0.071428567171096802 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.21428570151329041 -0.071428567171096802 0
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex 0.071428567171096802 -0.071428574621677399 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex -0.071428537368774414 -0.21428570151329041 0
      vertex -0.071428537368774414 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex -0.071428537368774414 -0.071428567171096802 0
      vertex 0.071428567171096802 -0.071428574621677399 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex 0.21428570151329041 -0.21428570151329041 0
      vertex 0.21428570151329041 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex 0.21428570151329041 -0.3571428656578064 0
      vertex 0.071428567171096802 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex 0.071428567171096802 -0.3571428656578064 0
      vertex -0.071428567171096802 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.071428567171096802 -0.21428570151329041 0
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex -0.071428537368774414 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex -0.21428570151329041 -0.3571428656578064 0
      vertex -0.21428568661212921 -0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex -0.21428568661212921 -0.21428570151329041 0
      vertex -0.071428537368774414 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex 0.071428567171096802 -0.3571428656578064 0
      vertex 0.071428567171096802 -0.5 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex 0.071428567171096802 -0.5 0
      vertex -0.071428596973419189 -0.5 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex -0.071428596973419189 -0.5 0
      vertex -0.21428570151329041 -0.5 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.071428567171096802 -0.3571428656578064 0
      vertex -0.21428570151329041 -0.5 0
      vertex -0.21428570151329041 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 2.0861626914836125e-07 -0.99999999999997824
    outer loop
      vertex -0.21428570151329041 -0.5 0
      vertex -0.3571428656578064 -0.5 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal -2.086162256276226e-07 -0 -0.99999999999997824
    outer loop
      vertex -0.21428570151329041 -0.5 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
      vertex -0.21428570151329041 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.21428570151329041 -0.5 0
      vertex -0.071428596973419189 -0.5 0
      vertex -0.071428596973419189 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.21428570151329041 -0.5 0
      vertex -0.071428596973419189 -0.5 0.1428571343421936
      vertex -0.21428573131561279 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.21428573131561279 -0.5 0.1428571343421936
      vertex -0.071428596973419189 -0.5 0.1428571343421936
      vertex -0.071428589522838593 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.21428573131561279 -0.5 0.1428571343421936
      vertex -0.071428589522838593 -0.5 0.28571429848670959
      vertex -0.21428573131561279 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.21428573131561279 -0.5 0.1428571343421936
      vertex -0.21428573131561279 -0.5 0.28571429848670959
      vertex -0.3571428656578064 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.21428573131561279 -0.5 0.1428571343421936
      vertex -0.3571428656578064 -0.5 0.28571429848670959
      vertex -0.3571428656578064 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.1428571343421936
      vertex -0.3571428656578064 -0.5 0
      vertex -0.21428570151329041 -0.5 0
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex -0.3571428656578064 -0.5 0.1428571343421936
      vertex -0.21428570151329041 -0.5 0
      vertex -0.21428573131561279 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.1428571343421936
      vertex -0.3571428656578064 -0.5 0.28571429848670959
      vertex -0.5 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.1428571343421936
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.5 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.3571428656578064 -0.5 0.28571429848670959
      vertex -0.3571428656578064 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.3571428656578064 -0.5 0.4285714328289032
      vertex -0.5 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.5 -0.5 0.4285714328289032
      vertex -0.5 -0.3571428656578064 0.4285714253783226
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.3571428656578064 0.5714285746216774
      vertex -0.5 -0.21428570151329041 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.21428570151329041 0.5714285671710968
      vertex -0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.21428570151329041 0.4285714328289032
      vertex -0.5 -0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.21428570151329041 0.28571429848670959
      vertex -0.5 -0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.5 0.4285714328289032
      vertex -0.5 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.3571428656578064 0.4285714253783226
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.5 -0.3571428656578064 0.5714285746216774
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.5 -0.5 0.71428570151329041
      vertex -0.5 -0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.5 -0.3571428656578064 0.71428570151329041
      vertex -0.5 -0.3571428656578064 0.5714285746216774
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.5 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.3571428656578064 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.571428582072258
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.3571428656578064 -0.5 0.571428582072258
      vertex -0.3571428656578064 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.5 -0.5 0.5714285671710968
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.5 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.3571428656578064 -0.5 0.571428582072258
      vertex -0.21428573131561279 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.21428573131561279 -0.5 0.5714285671710968
      vertex -0.21428573131561279 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.21428573131561279 -0.5 0.71428570151329041
      vertex -0.21428570151329041 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -2.0861622562761805e-07 -0.99999999999995648 2.0861622562761805e-07
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.3571428656578064 -0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal 2.0861626914835673e-07 -0.99999999999995648 2.0861622562761805e-07
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.3571428656578064 -0.49999997019767761 0.8571428656578064
      vertex -0.5 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0.71428570151329041
      vertex -0.5 -0.5 0.8571428656578064
      vertex -0.5 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.21428573131561279 -0.5 0.71428570151329041
      vertex -0.071428574621677399 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.071428574621677399 -0.5 0.71428570151329041
      vertex -0.071428582072257996 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.071428582072257996 -0.5 0.8571428656578064
      vertex -0.071428567171096802 -0.5 1
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.071428567171096802 -0.5 1
      vertex -0.21428567171096802 -0.5 1
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.21428567171096802 -0.5 1
      vertex -0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal -2.0861622562761805e-07 -0.99999999999995648 -2.0861626914835673e-07
    outer loop
      vertex -0.21428570151329041 -0.5 0.8571428656578064
      vertex -0.3571428656578064 -0.5 1
      vertex -0.3571428656578064 -0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.21428567171096802 -0.5 1
      vertex -0.071428567171096802 -0.5 1
      vertex -0.071428559720516205 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.21428567171096802 -0.5 1
      vertex -0.071428559720516205 -0.3571428656578064 1
      vertex -0.21428570151329041 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428567171096802 -0.5 1
      vertex -0.21428570151329041 -0.3571428656578064 1
      vertex -0.35714280605316162 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428567171096802 -0.5 1
      vertex -0.35714280605316162 -0.3571428656578064 1
      vertex -0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.21428570151329041 -0.3571428656578064 1
      vertex -0.071428559720516205 -0.3571428656578064 1
      vertex -0.071428537368774414 -0.21428573131561279 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 -0.3571428656578064 1
      vertex -0.071428537368774414 -0.21428573131561279 1
      vertex -0.21428570151329041 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428570151329041 -0.3571428656578064 1
      vertex -0.21428570151329041 -0.21428570151329041 1
      vertex -0.35714280605316162 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428570151329041 -0.3571428656578064 1
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.35714280605316162 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.5 -0.21428570151329041 1
      vertex -0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.5 -0.3571428656578064 1
      vertex -0.35714280605316162 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.21428570151329041 -0.21428570151329041 1
      vertex -0.21428570151329041 -0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.21428570151329041 -0.071428574621677399 1
      vertex -0.35714280605316162 -0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.35714280605316162 -0.071428567171096802 1
      vertex -0.5 -0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 -0.21428570151329041 1
      vertex -0.5 -0.071428567171096802 1
      vertex -0.5 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.5 -0.071428582072257996 0.8571428656578064
      vertex -0.5 -0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.5 -0.21428570151329041 0.8571428656578064
      vertex -0.5 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.35714280605316162 -0.071428567171096802 1
      vertex -0.35714280605316162 0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.35714280605316162 0.071428567171096802 1
      vertex -0.5 0.071428567171096802 1
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.5 0.071428567171096802 1
      vertex -0.5 0.071428574621677399 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 -0.071428567171096802 1
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 -0.071428582072257996 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 0.071428567171096802 1
      vertex -0.5 0.21428570151329041 1
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 0.21428570151329041 1
      vertex -0.5 0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 0.21428570151329041 0.8571428656578064
      vertex -0.5 0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 0.21428570151329041 0.71428570151329041
      vertex -0.5 0.071428567171096802 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 0.071428567171096802 0.71428570151329041
      vertex -0.5 -0.071428574621677399 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.071428574621677399 0.8571428656578064
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 -0.071428582072257996 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 0.071428567171096802 0.71428570151329041
      vertex -0.5 0.071428574621677399 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 0.071428574621677399 0.5714285671710968
      vertex -0.5 -0.071428574621677399 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 -0.071428574621677399 0.5714285671710968
      vertex -0.5 -0.21428570151329041 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 -0.21428570151329041 0.5714285671710968
      vertex -0.5 -0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 -0.21428570151329041 0.71428570151329041
      vertex -0.5 -0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.71428570151329041
      vertex -0.5 -0.21428570151329041 0.8571428656578064
      vertex -0.5 -0.071428582072257996 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.21428570151329041 0.71428570151329041
      vertex -0.5 -0.21428570151329041 0.5714285671710968
      vertex -0.5 -0.3571428656578064 0.5714285746216774
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.71428570151329041
      vertex -0.5 -0.3571428656578064 0.5714285746216774
      vertex -0.5 -0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal -0.99999999999997824 -0 2.086162256276226e-07
    outer loop
      vertex -0.5 -0.21428570151329041 0.71428570151329041
      vertex -0.5 -0.3571428656578064 0.71428570151329041
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -0.99999999999997824 -2.086162256276226e-07 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.71428570151329041
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
      vertex -0.5 -0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.5714285671710968
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428574621677399 0.5714285671710968
      vertex -0.5 -0.21428570151329041 0.4285714328289032
      vertex -0.5 -0.21428570151329041 0.5714285671710968
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 0.071428567171096802 0.42857146263122559
      vertex -0.5 0.071428574621677399 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 0.071428574621677399 0.28571429848670959
      vertex -0.5 -0.071428574621677399 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.071428574621677399 0.28571429848670959
      vertex -0.5 -0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.21428570151329041 0.28571429848670959
      vertex -0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.28571429848670959
      vertex -0.5 0.071428574621677399 0.28571429848670959
      vertex -0.5 0.071428552269935608 0.14285716414451599
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.28571429848670959
      vertex -0.5 0.071428552269935608 0.14285716414451599
      vertex -0.5 -0.071428567171096802 0.14285716414451599
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.071428574621677399 0.28571429848670959
      vertex -0.5 -0.071428567171096802 0.14285716414451599
      vertex -0.5 -0.21428570151329041 0.1428571343421936
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.071428574621677399 0.28571429848670959
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.071428567171096802 0.14285716414451599
      vertex -0.5 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.071428567171096802 0
      vertex -0.5 -0.21428570151329041 0
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.21428570151329041 0
      vertex -0.5 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0
      vertex -0.5 -0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0.28571429848670959
      vertex -0.5 -0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.1428571343421936
      vertex -0.5 -0.5 0.1428571343421936
      vertex -0.5 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.1428571343421936
      vertex -0.5 -0.5 0.28571429848670959
      vertex -0.5 -0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.5 -0.21428570151329041 0
      vertex -0.5 -0.071428567171096802 0
      vertex -0.35714280605316162 -0.071428582072257996 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 -0.21428570151329041 0
      vertex -0.35714280605316162 -0.071428582072257996 0
      vertex -0.3571428656578064 -0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 -2.086162256276226e-07 -0.99999999999997824
    outer loop
      vertex -0.5 -0.21428570151329041 0
      vertex -0.3571428656578064 -0.21428570151329041 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal 2.0861626914836125e-07 0 -0.99999999999997824
    outer loop
      vertex -0.5 -0.21428570151329041 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
      vertex -0.5 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.3571428656578064 -0.21428570151329041 0
      vertex -0.35714280605316162 -0.071428582072257996 0
      vertex -0.21428570151329041 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.3571428656578064 -0.21428570151329041 0
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.21428568661212921 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.3571428656578064 -0.21428570151329041 0
      vertex -0.21428568661212921 -0.21428570151329041 0
      vertex -0.21428570151329041 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -2.0861622562761805e-07 -2.0861622562761805e-07 -0.99999999999995648
    outer loop
      vertex -0.3571428656578064 -0.21428570151329041 0
      vertex -0.21428570151329041 -0.3571428656578064 0
      vertex -0.3571428656578064 -0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.35714280605316162 -0.071428582072257996 0
      vertex -0.35714280605316162 0.071428574621677399 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.35714280605316162 0.071428574621677399 0
      vertex -0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.21428570151329041 0.071428567171096802 0
      vertex -0.071428537368774414 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex -0.071428537368774414 -0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.071428537368774414 -0.071428567171096802 0
      vertex -0.071428537368774414 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 -0.071428567171096802 0
      vertex -0.071428537368774414 -0.21428570151329041 0
      vertex -0.21428568661212921 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex -0.071428552269935608 0.21428570151329041 0
      vertex 0.071428582072257996 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex 0.071428567171096802 0.071428574621677399 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex 0.071428567171096802 0.071428574621677399 0
      vertex 0.071428567171096802 -0.071428574621677399 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex 0.071428567171096802 -0.071428574621677399 0
      vertex -0.071428537368774414 -0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex -0.071428552269935608 0.21428570151329041 0
      vertex -0.071428567171096802 0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex -0.071428567171096802 0.3571428656578064 0
      vertex 0.071428596973419189 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex 0.071428596973419189 0.3571428656578064 0
      vertex 0.21428570151329041 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.21428570151329041 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex 0.21428570151329041 0.21428570151329041 0
      vertex 0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.071428582072257996 0.21428570151329041 0
      vertex 0.21428570151329041 0.071428567171096802 0
      vertex 0.071428567171096802 0.071428574621677399 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.071428596973419189 0.3571428656578064 0
      vertex 0.071428596973419189 0.5 0
    endloop
  endfacet
)";
    const std::string part2 = R"(
  facet normal -0 0 -1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.071428596973419189 0.5 0
      vertex 0.21428570151329041 0.5 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.21428570151329041 0.5 0
      vertex 0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 -2.0861626914835673e-07 -0.99999999999995648
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.3571428656578064 0.5 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 2.0861622562761805e-07 -0.99999999999995648
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.3571428656578064 0.3571428656578064 2.9802322387695312e-08
      vertex 0.3571428656578064 0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 0
      vertex 0.3571428656578064 0.21428570151329041 0
      vertex 0.21428570151329041 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0
      vertex 0.071428596973419189 0.5 0
      vertex 0.071428582072257996 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0
      vertex 0.071428582072257996 0.5 0.1428571343421936
      vertex 0.21428570151329041 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0
      vertex 0.21428570151329041 0.5 0.1428571343421936
      vertex 0.3571428656578064 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0
      vertex 0.3571428656578064 0.5 0.1428571343421936
      vertex 0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.1428571343421936
      vertex 0.071428582072257996 0.5 0.1428571343421936
      vertex 0.071428567171096802 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0.1428571343421936
      vertex 0.071428567171096802 0.5 0.28571429848670959
      vertex 0.21428570151329041 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.1428571343421936
      vertex 0.21428570151329041 0.5 0.28571429848670959
      vertex 0.3571428656578064 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.1428571343421936
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.3571428656578064 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.21428570151329041 0.5 0.28571429848670959
      vertex 0.21428570151329041 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.21428570151329041 0.5 0.4285714328289032
      vertex 0.3571428656578064 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.3571428656578064 0.5 0.4285714328289032
      vertex 0.5 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.5 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.5 0.5 0.28571429848670959
      vertex 0.5 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.28571429848670959
      vertex 0.5 0.5 0.1428571343421936
      vertex 0.3571428656578064 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.3571428656578064 0.5 0.4285714328289032
      vertex 0.3571428656578064 0.5 0.571428582072258
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.3571428656578064 0.5 0.571428582072258
      vertex 0.5 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.5 0.5 0.5714285671710968
      vertex 0.5 0.3571428656578064 0.571428582072258
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.5 0.3571428656578064 0.571428582072258
      vertex 0.5 0.3571428656578064 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.5 0.3571428656578064 0.4285714253783226
      vertex 0.5 0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.5 0.4285714328289032
      vertex 0.5 0.3571428656578064 0.28571429848670959
      vertex 0.5 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.3571428656578064 0.4285714253783226
      vertex 0.5 0.3571428656578064 0.571428582072258
      vertex 0.5 0.21428570151329041 0.5714285746216774
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.4285714253783226
      vertex 0.5 0.21428570151329041 0.5714285746216774
      vertex 0.5 0.21428570151329041 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.4285714253783226
      vertex 0.5 0.21428570151329041 0.4285714253783226
      vertex 0.5 0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.4285714253783226
      vertex 0.5 0.21428570151329041 0.28571429848670959
      vertex 0.5 0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.4285714253783226
      vertex 0.5 0.21428570151329041 0.5714285746216774
      vertex 0.5 0.071428567171096802 0.5714285746216774
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.4285714253783226
      vertex 0.5 0.071428567171096802 0.5714285746216774
      vertex 0.5 0.071428567171096802 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.4285714253783226
      vertex 0.5 0.071428567171096802 0.4285714253783226
      vertex 0.5 0.071428567171096802 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.4285714253783226
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 0.071428567171096802 0.1428571343421936
      vertex 0.5 0.21428570151329041 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 0.21428570151329041 0.1428571343421936
      vertex 0.5 0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 0.071428567171096802 0.4285714253783226
      vertex 0.5 -0.071428567171096802 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.071428567171096802 0.4285714328289032
      vertex 0.5 -0.071428567171096802 0.28571429848670959
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.071428582072257996 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 0.071428567171096802 0.1428571343421936
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 -0.071428567171096802 0
      vertex 0.5 0.071428567171096802 0
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 0.071428567171096802 0
      vertex 0.5 0.071428567171096802 0.1428571343421936
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 -0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.21428570151329041 0.28571432828903198
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 -0.21428570151329041 0.28571432828903198
      vertex 0.5 -0.21428570151329041 0.1428571343421936
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 -0.21428570151329041 0.1428571343421936
      vertex 0.5 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 -0.071428582072257996 0.1428571343421936
      vertex 0.5 -0.21428570151329041 0
      vertex 0.5 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.071428567171096802 0
      vertex 0.5 -0.071428567171096802 0
    endloop
  endfacet
  facet normal 0.99999999999997824 -2.086162256276226e-07 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.5 -0.21428570151329041 0.1428571343421936
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal 0.99999999999997824 0 2.0861626914836125e-07
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
      vertex 0.5 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.5 -0.3571428656578064 0
      vertex 0.3571428656578064 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.5 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.3571428656578064 0
      vertex 0.3571428656578064 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.1428571343421936
      vertex 0.5 -0.21428570151329041 0.28571432828903198
      vertex 0.5 -0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 0.99999999999995648 -2.0861622562761805e-07 -2.0861622562761805e-07
    outer loop
      vertex 0.5 -0.21428570151329041 0.1428571343421936
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.21428570151329041 0.28571432828903198
      vertex 0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.21428570151329041 0.4285714328289032
      vertex 0.5 -0.3571428656578064 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.3571428656578064 0.4285714253783226
      vertex 0.5 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.5 0.4285714328289032
      vertex 0.5 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.5 0.28571429848670959
      vertex 0.5 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0.99999999999995648 2.0861626914835673e-07 -2.0861622562761805e-07
    outer loop
      vertex 0.5 -0.3571428656578064 0.28571429848670959
      vertex 0.5 -0.5 0.1428571343421936
      vertex 0.49999997019767761 -0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.28571429848670959
      vertex 0.5 -0.5 0.4285714328289032
      vertex 0.3571428656578064 -0.5 0.428571417927742
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.28571429848670959
      vertex 0.3571428656578064 -0.5 0.428571417927742
      vertex 0.3571428656578064 -0.5 0.28571432828903198
    endloop
  endfacet
  facet normal -4.3520729595117975e-14 -0.99999999999997813 -2.0861618210690206e-07
    outer loop
      vertex 0.5 -0.5 0.28571429848670959
      vertex 0.3571428656578064 -0.5 0.28571432828903198
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 -0.99999999999997824 0
    outer loop
      vertex 0.5 -0.5 0.28571429848670959
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
      vertex 0.5 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.3571428656578064 -0.5 0.28571432828903198
      vertex 0.21428570151329041 -0.5 0.28571431338787079
      vertex 0.21428570151329041 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 -0.99999999999995648 -2.0861618210689754e-07
    outer loop
      vertex 0.3571428656578064 -0.5 0.28571432828903198
      vertex 0.21428570151329041 -0.5 0.1428571343421936
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.3571428656578064 -0.5 0.28571432828903198
      vertex 0.3571428656578064 -0.5 0.428571417927742
      vertex 0.21428570151329041 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.3571428656578064 -0.5 0.28571432828903198
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.21428570151329041 -0.5 0.28571431338787079
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.071428537368774414 -0.5 0.4285714328289032
      vertex 0.071428552269935608 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex 0.21428570151329041 -0.5 0.28571431338787079
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.3571428656578064 -0.5 0.428571417927742
      vertex 0.3571428656578064 -0.5 0.5714285746216774
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.3571428656578064 -0.5 0.5714285746216774
      vertex 0.21428570151329041 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.5714285671710968
      vertex 0.3571428656578064 -0.5 0.5714285746216774
      vertex 0.3571428656578064 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.5714285671710968
      vertex 0.3571428656578064 -0.5 0.71428570151329041
      vertex 0.21428570151329041 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.21428570151329041 -0.5 0.5714285671710968
      vertex 0.21428570151329041 -0.5 0.71428570151329041
      vertex 0.071428567171096802 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.21428570151329041 -0.5 0.5714285671710968
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex 0.071428537368774414 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428537368774414 -0.5 0.5714285671710968
      vertex 0.071428537368774414 -0.5 0.4285714328289032
      vertex 0.21428570151329041 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.071428537368774414 -0.5 0.5714285671710968
      vertex 0.21428570151329041 -0.5 0.4285714328289032
      vertex 0.21428570151329041 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.071428537368774414 -0.5 0.5714285671710968
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex -0.071428574621677399 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428537368774414 -0.5 0.5714285671710968
      vertex -0.071428574621677399 -0.5 0.71428570151329041
      vertex -0.071428582072257996 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex -0.2142857164144516 -0.5 0.4285714328289032
      vertex -0.21428573131561279 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex -0.21428573131561279 -0.5 0.28571429848670959
      vertex -0.071428589522838593 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex -0.071428589522838593 -0.5 0.28571429848670959
      vertex 0.071428552269935608 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex 0.071428537368774414 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex 0.071428537368774414 -0.5 0.4285714328289032
      vertex 0.071428537368774414 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex 0.071428537368774414 -0.5 0.5714285671710968
      vertex -0.071428582072257996 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex -0.071428582072257996 -0.5 0.5714285671710968
      vertex -0.21428573131561279 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428574621677399 -0.5 0.4285714328289032
      vertex -0.21428573131561279 -0.5 0.5714285671710968
      vertex -0.2142857164144516 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.2142857164144516 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex -0.2142857164144516 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.28571429848670959
      vertex -0.21428573131561279 -0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.2142857164144516 -0.5 0.4285714328289032
      vertex -0.21428573131561279 -0.5 0.5714285671710968
      vertex -0.3571428656578064 -0.5 0.571428582072258
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.2142857164144516 -0.5 0.4285714328289032
      vertex -0.3571428656578064 -0.5 0.571428582072258
      vertex -0.3571428656578064 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex -0.071428582072257996 -0.5 0.5714285671710968
      vertex -0.071428574621677399 -0.5 0.71428570151329041
      vertex -0.21428573131561279 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.071428582072257996 -0.5 0.5714285671710968
      vertex -0.21428573131561279 -0.5 0.71428570151329041
      vertex -0.21428573131561279 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex 0.21428570151329041 -0.5 0.71428570151329041
      vertex 0.21428570151329041 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex 0.21428570151329041 -0.5 0.8571428656578064
      vertex 0.071428582072257996 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex 0.071428582072257996 -0.5 0.8571428656578064
      vertex -0.071428582072257996 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428567171096802 -0.5 0.71428570151329041
      vertex -0.071428582072257996 -0.5 0.8571428656578064
      vertex -0.071428574621677399 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.8571428656578064
      vertex 0.21428570151329041 -0.5 0.8571428656578064
      vertex 0.21428570151329041 -0.5 1
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.8571428656578064
      vertex 0.21428570151329041 -0.5 1
      vertex 0.071428596973419189 -0.5 1
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.071428582072257996 -0.5 0.8571428656578064
      vertex 0.071428596973419189 -0.5 1
      vertex -0.071428567171096802 -0.5 1
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.8571428656578064
      vertex -0.071428567171096802 -0.5 1
      vertex -0.071428582072257996 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.071428596973419189 -0.5 1
      vertex 0.21428570151329041 -0.5 1
      vertex 0.21428570151329041 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428596973419189 -0.5 1
      vertex 0.21428570151329041 -0.3571428656578064 1
      vertex 0.071428596973419189 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428596973419189 -0.5 1
      vertex 0.071428596973419189 -0.3571428656578064 1
      vertex -0.071428559720516205 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428596973419189 -0.5 1
      vertex -0.071428559720516205 -0.3571428656578064 1
      vertex -0.071428567171096802 -0.5 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.071428596973419189 -0.3571428656578064 1
      vertex 0.21428570151329041 -0.3571428656578064 1
      vertex 0.21428570151329041 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428596973419189 -0.3571428656578064 1
      vertex 0.21428570151329041 -0.21428570151329041 1
      vertex 0.071428582072257996 -0.21428573131561279 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428596973419189 -0.3571428656578064 1
      vertex 0.071428582072257996 -0.21428573131561279 1
      vertex -0.071428537368774414 -0.21428573131561279 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428596973419189 -0.3571428656578064 1
      vertex -0.071428537368774414 -0.21428573131561279 1
      vertex -0.071428559720516205 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428582072257996 -0.21428573131561279 1
      vertex 0.21428570151329041 -0.21428570151329041 1
      vertex 0.21428573131561279 -0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428582072257996 -0.21428573131561279 1
      vertex 0.21428573131561279 -0.071428567171096802 1
      vertex 0.071428596973419189 -0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428582072257996 -0.21428573131561279 1
      vertex 0.071428596973419189 -0.071428574621677399 1
      vertex -0.071428507566452026 -0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428582072257996 -0.21428573131561279 1
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex -0.071428537368774414 -0.21428573131561279 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex -0.21428570151329041 -0.071428574621677399 1
      vertex -0.21428570151329041 -0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex -0.21428570151329041 -0.21428570151329041 1
      vertex -0.071428537368774414 -0.21428573131561279 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex 0.071428596973419189 -0.071428574621677399 1
      vertex 0.071428596973419189 0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex 0.071428596973419189 0.071428574621677399 1
      vertex -0.071428507566452026 0.071428559720516205 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex -0.071428507566452026 0.071428559720516205 1
      vertex -0.21428570151329041 0.071428544819355011 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.071428507566452026 -0.071428567171096802 1
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.21428570151329041 -0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.35714280605316162 0.071428567171096802 1
      vertex -0.35714280605316162 -0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.35714280605316162 -0.071428567171096802 1
      vertex -0.21428570151329041 -0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.071428507566452026 0.071428559720516205 1
      vertex -0.071428537368774414 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.071428537368774414 0.21428570151329041 1
      vertex -0.21428570151329041 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.21428570151329041 0.21428570151329041 1
      vertex -0.35714280605316162 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428570151329041 0.071428544819355011 1
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.35714280605316162 0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.5 0.21428570151329041 1
      vertex -0.5 0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.5 0.071428567171096802 1
      vertex -0.35714280605316162 0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.21428570151329041 0.21428570151329041 1
      vertex -0.21428570151329041 0.3571428656578064 1
    endloop
  endfacet
  facet normal -2.0861631266911356e-07 2.0861622562761805e-07 0.99999999999995648
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.35714280605316162 0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 2.0861618210689754e-07 2.0861622562761805e-07 0.99999999999995648
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.35714280605316162 0.3571428656578064 0.99999997019767761
      vertex -0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 1
      vertex -0.5 0.3571428656578064 1
      vertex -0.5 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.21428570151329041 0.21428570151329041 1
      vertex -0.071428537368774414 0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.071428537368774414 0.21428570151329041 1
      vertex -0.071428567171096802 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.071428567171096802 0.3571428656578064 1
      vertex -0.071428596973419189 0.5 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.071428596973419189 0.5 1
      vertex -0.21428570151329041 0.5 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.21428570151329041 0.5 1
      vertex -0.3571428656578064 0.5 1
    endloop
  endfacet
  facet normal -2.0861631266911356e-07 -2.0861635618987039e-07 0.99999999999995648
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 1
      vertex -0.3571428656578064 0.5 1
      vertex -0.35714280605316162 0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 1
      vertex -0.071428596973419189 0.5 1
      vertex -0.071428596973419189 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 1
      vertex -0.071428596973419189 0.5 0.8571428656578064
      vertex -0.21428570151329041 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.8571428656578064
      vertex -0.071428596973419189 0.5 0.8571428656578064
      vertex -0.071428582072257996 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.8571428656578064
      vertex -0.071428582072257996 0.5 0.71428570151329041
      vertex -0.21428570151329041 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.8571428656578064
      vertex -0.21428570151329041 0.5 0.71428570151329041
      vertex -0.3571428656578064 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.8571428656578064
      vertex -0.3571428656578064 0.5 0.71428570151329041
      vertex -0.3571428656578064 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.8571428656578064
      vertex -0.3571428656578064 0.5 1
      vertex -0.21428570151329041 0.5 1
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.3571428656578064 0.5 0.8571428656578064
      vertex -0.21428570151329041 0.5 1
      vertex -0.21428570151329041 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.8571428656578064
      vertex -0.3571428656578064 0.5 0.71428570151329041
      vertex -0.5 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.8571428656578064
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.5 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.5 0.3571428656578064 0.71428570151329041
      vertex -0.5 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.5 0.3571428656578064 0.8571428656578064
      vertex -0.5 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.3571428656578064 0.5 0.71428570151329041
      vertex -0.3571428656578064 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.3571428656578064 0.5 0.5714285671710968
      vertex -0.5 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.5 0.5 0.5714285671710968
      vertex -0.5 0.3571428656578064 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.5 0.71428570151329041
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.5 0.5714285671710968
      vertex -0.5 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.5 0.4285714328289032
      vertex -0.5 0.3571428656578064 0.428571417927742
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.3571428656578064 0.428571417927742
      vertex -0.5 0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.21428570151329041 0.5714285671710968
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.21428570151329041 0.5714285671710968
      vertex -0.5 0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.3571428656578064 0.5714285671710968
      vertex -0.5 0.21428570151329041 0.71428570151329041
      vertex -0.5 0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.5714285671710968
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.071428567171096802 0.42857146263122559
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.5714285671710968
      vertex -0.5 0.071428567171096802 0.42857146263122559
      vertex -0.5 0.071428574621677399 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.5714285671710968
      vertex -0.5 0.071428574621677399 0.5714285671710968
      vertex -0.5 0.071428567171096802 0.71428570151329041
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.5714285671710968
      vertex -0.5 0.071428567171096802 0.71428570151329041
      vertex -0.5 0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.3571428656578064 0.428571417927742
      vertex -0.5 0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.3571428656578064 0.28571429848670959
      vertex -0.5 0.21428570151329041 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.21428570151329041 0.28571429848670959
      vertex -0.5 0.071428574621677399 0.28571429848670959
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.4285714328289032
      vertex -0.5 0.071428574621677399 0.28571429848670959
      vertex -0.5 0.071428567171096802 0.42857146263122559
    endloop
  endfacet
  facet normal -0.99999999999997824 0 -2.086162256276226e-07
    outer loop
      vertex -0.5 0.21428570151329041 0.28571429848670959
      vertex -0.5 0.3571428656578064 0.28571429848670959
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal -0.99999999999997824 2.086162256276226e-07 0
    outer loop
      vertex -0.5 0.21428570151329041 0.28571429848670959
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
      vertex -0.5 0.21428570151329041 0.1428571343421936
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.28571429848670959
      vertex -0.5 0.21428570151329041 0.1428571343421936
      vertex -0.5 0.071428552269935608 0.14285716414451599
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.28571429848670959
      vertex -0.5 0.071428552269935608 0.14285716414451599
      vertex -0.5 0.071428574621677399 0.28571429848670959
    endloop
  endfacet
  facet normal -0.99999999999995648 2.0861622562761805e-07 2.0861626914835673e-07
    outer loop
      vertex -0.5 0.21428570151329041 0.1428571343421936
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
      vertex -0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.1428571343421936
      vertex -0.5 0.3571428656578064 0
      vertex -0.5 0.21428570151329041 0
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.1428571343421936
      vertex -0.5 0.21428570151329041 0
      vertex -0.5 0.071428567171096802 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.1428571343421936
      vertex -0.5 0.071428567171096802 0
      vertex -0.5 0.071428552269935608 0.14285716414451599
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.5 0.21428570151329041 0
      vertex -0.35714280605316162 0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.35714280605316162 0.071428574621677399 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.35714280605316162 0.071428574621677399 0
      vertex -0.35714280605316162 -0.071428582072257996 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.35714280605316162 -0.071428582072257996 0
      vertex -0.5 -0.071428567171096802 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.5 -0.071428567171096802 0
      vertex -0.5 -0.071428567171096802 0.14285716414451599
    endloop
  endfacet
  facet normal -1 -0 0
    outer loop
      vertex -0.5 0.071428567171096802 0
      vertex -0.5 -0.071428567171096802 0.14285716414451599
      vertex -0.5 0.071428552269935608 0.14285716414451599
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.5 0.21428570151329041 0
      vertex -0.5 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.5 0.3571428656578064 0
      vertex -0.35714280605316162 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.35714280605316162 0.3571428656578064 0
      vertex -0.21428570151329041 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.21428570151329041 0.21428570151329041 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.21428570151329041 0.21428570151329041 0
      vertex -0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.35714280605316162 0.21428570151329041 0
      vertex -0.21428570151329041 0.071428567171096802 0
      vertex -0.35714280605316162 0.071428574621677399 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.21428570151329041 0.21428570151329041 0
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.071428567171096802 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.21428570151329041 0.21428570151329041 0
      vertex -0.071428567171096802 0.3571428656578064 0
      vertex -0.071428552269935608 0.21428570151329041 0
    endloop
  endfacet
)";
    const std::string part3 = R"(
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 0.21428570151329041 0
      vertex -0.071428552269935608 0.21428570151329041 0
      vertex -0.071428537368774414 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 0.21428570151329041 0
      vertex -0.071428537368774414 0.071428567171096802 0
      vertex -0.21428570151329041 0.071428567171096802 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.35714280605316162 0.3571428656578064 0
      vertex -0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.3571428656578064 0.5 0
      vertex -0.21428567171096802 0.5 0
    endloop
  endfacet
  facet normal -0 0 -1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.21428567171096802 0.5 0
      vertex -0.071428567171096802 0.5 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.21428570151329041 0.3571428656578064 0
      vertex -0.071428567171096802 0.5 0
      vertex -0.071428567171096802 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex -0.21428567171096802 0.5 0
      vertex -0.21428570151329041 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex -0.21428570151329041 0.5 0.1428571343421936
      vertex -0.071428582072257996 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex -0.071428582072257996 0.5 0.1428571343421936
      vertex 0.071428582072257996 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex 0.071428582072257996 0.5 0.1428571343421936
      vertex 0.071428596973419189 0.5 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex 0.071428596973419189 0.5 0
      vertex 0.071428596973419189 0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex -0.071428567171096802 0.5 0
      vertex 0.071428596973419189 0.3571428656578064 0
      vertex -0.071428567171096802 0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex -0.071428582072257996 0.5 0.1428571343421936
      vertex -0.21428570151329041 0.5 0.1428571343421936
      vertex -0.21428573131561279 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.071428582072257996 0.5 0.1428571343421936
      vertex -0.21428573131561279 0.5 0.28571429848670959
      vertex -0.071428574621677399 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.071428582072257996 0.5 0.1428571343421936
      vertex -0.071428574621677399 0.5 0.28571429848670959
      vertex 0.071428567171096802 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.071428582072257996 0.5 0.1428571343421936
      vertex 0.071428567171096802 0.5 0.28571429848670959
      vertex 0.071428582072257996 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.071428574621677399 0.5 0.28571429848670959
      vertex -0.21428573131561279 0.5 0.28571429848670959
      vertex -0.21428570151329041 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.071428574621677399 0.5 0.28571429848670959
      vertex -0.21428570151329041 0.5 0.4285714328289032
      vertex -0.071428574621677399 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.071428574621677399 0.5 0.28571429848670959
      vertex -0.071428574621677399 0.5 0.4285714328289032
      vertex 0.071428544819355011 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.071428574621677399 0.5 0.28571429848670959
      vertex 0.071428544819355011 0.5 0.4285714328289032
      vertex 0.071428567171096802 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.071428544819355011 0.5 0.4285714328289032
      vertex 0.21428570151329041 0.5 0.4285714328289032
      vertex 0.21428570151329041 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.071428544819355011 0.5 0.4285714328289032
      vertex 0.21428570151329041 0.5 0.28571429848670959
      vertex 0.071428567171096802 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.071428537368774414 0.5 0.5714285671710968
      vertex 0.071428552269935608 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.071428552269935608 0.5 0.71428570151329041
      vertex 0.21428570151329041 0.5 0.71428568661212921
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.21428570151329041 0.5 0.71428568661212921
      vertex 0.3571428656578064 0.5 0.71428567171096802
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.3571428656578064 0.5 0.571428582072258
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.3571428656578064 0.5 0.571428582072258
      vertex 0.3571428656578064 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.3571428656578064 0.5 0.4285714328289032
      vertex 0.21428570151329041 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.21428570151329041 0.5 0.4285714328289032
      vertex 0.071428544819355011 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.21428570151329041 0.5 0.5714285671710968
      vertex 0.071428544819355011 0.5 0.4285714328289032
      vertex 0.071428537368774414 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.21428570151329041 0.5 0.71428568661212921
      vertex 0.21428570151329041 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 0.99999999999995648 2.0861618210689754e-07
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.21428570151329041 0.5 0.8571428656578064
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal -2.0861626914835673e-07 0.99999999999995648 2.0861618210689754e-07
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
      vertex 0.5 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.5 0.5 0.8571428656578064
      vertex 0.5 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.5 0.5 0.71428570151329041
      vertex 0.5 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex 0.3571428656578064 0.5 0.71428567171096802
      vertex 0.5 0.5 0.5714285671710968
      vertex 0.3571428656578064 0.5 0.571428582072258
    endloop
  endfacet
  facet normal 0.99999999999997824 -2.0861626914836125e-07 -0
    outer loop
      vertex 0.5 0.5 0.71428570151329041
      vertex 0.5 0.5 0.8571428656578064
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 0.99999999999997824 -0 2.086162256276226e-07
    outer loop
      vertex 0.5 0.5 0.71428570151329041
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
      vertex 0.5 0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.5 0.71428570151329041
      vertex 0.5 0.3571428656578064 0.71428570151329041
      vertex 0.5 0.3571428656578064 0.571428582072258
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.5 0.71428570151329041
      vertex 0.5 0.3571428656578064 0.571428582072258
      vertex 0.5 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0.99999999999995648 2.0861622562761805e-07 2.0861622562761805e-07
    outer loop
      vertex 0.5 0.3571428656578064 0.71428570151329041
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
      vertex 0.5 0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.71428570151329041
      vertex 0.5 0.21428570151329041 0.8571428656578064
      vertex 0.5 0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.71428570151329041
      vertex 0.5 0.21428570151329041 0.71428570151329041
      vertex 0.5 0.21428570151329041 0.5714285746216774
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.3571428656578064 0.71428570151329041
      vertex 0.5 0.21428570151329041 0.5714285746216774
      vertex 0.5 0.3571428656578064 0.571428582072258
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.71428570151329041
      vertex 0.5 0.21428570151329041 0.8571428656578064
      vertex 0.5 0.071428552269935608 0.8571428656578064
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.71428570151329041
      vertex 0.5 0.071428552269935608 0.8571428656578064
      vertex 0.5 0.071428567171096802 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.71428570151329041
      vertex 0.5 0.071428567171096802 0.71428570151329041
      vertex 0.5 0.071428567171096802 0.5714285746216774
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 0.71428570151329041
      vertex 0.5 0.071428567171096802 0.5714285746216774
      vertex 0.5 0.21428570151329041 0.5714285746216774
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.071428567171096802 0.71428570151329041
      vertex 0.5 0.071428552269935608 0.8571428656578064
      vertex 0.5 -0.071428567171096802 0.8571428656578064
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.8571428656578064
      vertex 0.5 -0.071428567171096802 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.5714285671710968
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 0.071428567171096802 0.5714285746216774
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 -0.071428567171096802 0.4285714328289032
      vertex 0.5 0.071428567171096802 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 0.071428567171096802 0.4285714253783226
      vertex 0.5 0.071428567171096802 0.5714285746216774
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 -0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.21428570151329041 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 -0.21428570151329041 0.71428570151329041
      vertex 0.5 -0.21428570151329041 0.5714285671710968
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 -0.21428570151329041 0.5714285671710968
      vertex 0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.071428567171096802 0.5714285671710968
      vertex 0.5 -0.21428570151329041 0.4285714328289032
      vertex 0.5 -0.071428567171096802 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.5714285671710968
      vertex 0.5 -0.21428570151329041 0.71428570151329041
      vertex 0.5 -0.3571428656578064 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.5714285671710968
      vertex 0.5 -0.3571428656578064 0.71428570151329041
      vertex 0.5 -0.3571428656578064 0.5714285671710968
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.5714285671710968
      vertex 0.5 -0.3571428656578064 0.5714285671710968
      vertex 0.5 -0.3571428656578064 0.4285714253783226
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.5714285671710968
      vertex 0.5 -0.3571428656578064 0.4285714253783226
      vertex 0.5 -0.21428570151329041 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.5714285671710968
      vertex 0.5 -0.3571428656578064 0.71428570151329041
      vertex 0.5 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.3571428656578064 0.5714285671710968
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.5 -0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.3571428656578064 0.5714285671710968
      vertex 0.5 -0.5 0.5714285671710968
      vertex 0.5 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.3571428656578064 0.5714285671710968
      vertex 0.5 -0.5 0.4285714328289032
      vertex 0.5 -0.3571428656578064 0.4285714253783226
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.5714285671710968
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.3571428656578064 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.5714285671710968
      vertex 0.3571428656578064 -0.5 0.71428570151329041
      vertex 0.3571428656578064 -0.5 0.5714285746216774
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.5 -0.5 0.5714285671710968
      vertex 0.3571428656578064 -0.5 0.5714285746216774
      vertex 0.3571428656578064 -0.5 0.428571417927742
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.5 -0.5 0.5714285671710968
      vertex 0.3571428656578064 -0.5 0.428571417927742
      vertex 0.5 -0.5 0.4285714328289032
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.5 -0.3571428656578064 0.71428570151329041
      vertex 0.5 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.5 -0.3571428656578064 0.8571428656578064
      vertex 0.5 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.5 -0.5 0.8571428656578064
      vertex 0.3571428656578064 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.5 -0.5 0.71428570151329041
      vertex 0.3571428656578064 -0.5 0.8571428656578064
      vertex 0.3571428656578064 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.71428570151329041
      vertex 0.5 -0.21428570151329041 0.71428570151329041
      vertex 0.5 -0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.3571428656578064 0.71428570151329041
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.21428570151329041 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.71428570151329041
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.071428567171096802 0.71428570151329041
      vertex 0.5 -0.071428567171096802 0.8571428656578064
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.071428567171096802 0.8571428656578064
      vertex 0.5 -0.071428567171096802 1
    endloop
  endfacet
  facet normal 1 -0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.071428567171096802 1
      vertex 0.5 -0.21428570151329041 1
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.21428570151329041 1
      vertex 0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.8571428656578064
      vertex 0.5 -0.3571428656578064 1
      vertex 0.5 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.5 -0.21428570151329041 1
      vertex 0.5 -0.071428567171096802 1
      vertex 0.3571428656578064 -0.071428582072257996 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.5 -0.21428570151329041 1
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.3571428656578064 -0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 -2.086162256276226e-07 0.99999999999997824
    outer loop
      vertex 0.5 -0.21428570151329041 1
      vertex 0.3571428656578064 -0.21428570151329041 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal -2.0861626914836125e-07 0 0.99999999999997824
    outer loop
      vertex 0.5 -0.21428570151329041 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
      vertex 0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.3571428656578064 -0.21428570151329041 1
      vertex 0.21428570151329041 -0.21428570151329041 1
      vertex 0.21428570151329041 -0.3571428656578064 1
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 -2.0861622562761805e-07 0.99999999999995648
    outer loop
      vertex 0.3571428656578064 -0.21428570151329041 1
      vertex 0.21428570151329041 -0.3571428656578064 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.3571428656578064 -0.21428570151329041 1
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.21428573131561279 -0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.3571428656578064 -0.21428570151329041 1
      vertex 0.21428573131561279 -0.071428567171096802 1
      vertex 0.21428570151329041 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.5 -0.071428567171096802 1
      vertex 0.5 0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.5 0.071428567171096802 1
      vertex 0.3571428656578064 0.071428582072257996 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.3571428656578064 0.071428582072257996 1
      vertex 0.2142857164144516 0.071428574621677399 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.3571428656578064 -0.071428582072257996 1
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.21428573131561279 -0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.071428596973419189 0.071428574621677399 1
      vertex 0.071428596973419189 -0.071428574621677399 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.071428596973419189 -0.071428574621677399 1
      vertex 0.21428573131561279 -0.071428567171096802 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.3571428656578064 0.071428582072257996 1
      vertex 0.3571428656578064 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.21428573131561279 0.21428570151329041 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex -0.071428537368774414 0.21428570151329041 1
      vertex -0.071428507566452026 0.071428559720516205 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex -0.071428507566452026 0.071428559720516205 1
      vertex 0.071428596973419189 0.071428574621677399 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex 0.071428596973419189 0.071428574621677399 1
      vertex 0.2142857164144516 0.071428574621677399 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex 0.2142857164144516 0.071428574621677399 1
      vertex 0.21428573131561279 0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex 0.21428573131561279 0.21428570151329041 1
      vertex 0.21428570151329041 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex 0.21428570151329041 0.3571428656578064 1
      vertex 0.071428574621677399 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex 0.071428574621677399 0.3571428656578064 1
      vertex -0.071428567171096802 0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.071428596973419189 0.21428570151329041 1
      vertex -0.071428567171096802 0.3571428656578064 1
      vertex -0.071428537368774414 0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.071428574621677399 0.3571428656578064 1
      vertex 0.21428570151329041 0.3571428656578064 1
      vertex 0.21428570151329041 0.5 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.071428574621677399 0.3571428656578064 1
      vertex 0.21428570151329041 0.5 1
      vertex 0.071428567171096802 0.5 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.071428574621677399 0.3571428656578064 1
      vertex 0.071428567171096802 0.5 1
      vertex -0.071428596973419189 0.5 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.071428574621677399 0.3571428656578064 1
      vertex -0.071428596973419189 0.5 1
      vertex -0.071428567171096802 0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.071428567171096802 0.5 1
      vertex 0.21428570151329041 0.5 1
      vertex 0.21428570151329041 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428567171096802 0.5 1
      vertex 0.21428570151329041 0.5 0.8571428656578064
      vertex 0.071428582072257996 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex 0.071428567171096802 0.5 1
      vertex 0.071428582072257996 0.5 0.8571428656578064
      vertex -0.071428596973419189 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.071428567171096802 0.5 1
      vertex -0.071428596973419189 0.5 0.8571428656578064
      vertex -0.071428596973419189 0.5 1
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex 0.071428582072257996 0.5 0.8571428656578064
      vertex 0.21428570151329041 0.5 0.8571428656578064
      vertex 0.21428570151329041 0.5 0.71428568661212921
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428582072257996 0.5 0.8571428656578064
      vertex 0.21428570151329041 0.5 0.71428568661212921
      vertex 0.071428552269935608 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428582072257996 0.5 0.8571428656578064
      vertex 0.071428552269935608 0.5 0.71428570151329041
      vertex -0.071428582072257996 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428582072257996 0.5 0.8571428656578064
      vertex -0.071428582072257996 0.5 0.71428570151329041
      vertex -0.071428596973419189 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.21428570151329041 0.5 1
      vertex 0.21428570151329041 0.3571428656578064 1
      vertex 0.3571428656578064 0.3571428656578064 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.21428570151329041 0.5 1
      vertex 0.3571428656578064 0.3571428656578064 1
      vertex 0.3571428656578064 0.5 1
    endloop
  endfacet
  facet normal -0 0.99999999999997824 -2.0861626914836125e-07
    outer loop
      vertex 0.21428570151329041 0.5 1
      vertex 0.3571428656578064 0.5 1
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
    endloop
  endfacet
  facet normal 2.086162256276226e-07 0.99999999999997824 -0
    outer loop
      vertex 0.21428570151329041 0.5 1
      vertex 0.3571428656578064 0.49999997019767761 0.8571428656578064
      vertex 0.21428570151329041 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 1
      vertex 0.21428573131561279 0.21428570151329041 1
      vertex 0.3571428656578064 0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.21428570151329041 0.3571428656578064 1
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.3571428656578064 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.3571428656578064 0.071428582072257996 1
      vertex 0.5 0.071428567171096802 1
    endloop
  endfacet
  facet normal 0 0 1
    outer loop
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.5 0.071428567171096802 1
      vertex 0.5 0.21428570151329041 1
    endloop
  endfacet
  facet normal 0 -0 1
    outer loop
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.5 0.21428570151329041 1
      vertex 0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal -0 0 1
    outer loop
      vertex 0.3571428656578064 0.21428570151329041 1
      vertex 0.5 0.3571428656578064 1
      vertex 0.3571428656578064 0.3571428656578064 1
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 1
      vertex 0.5 0.071428567171096802 1
      vertex 0.5 0.071428552269935608 0.8571428656578064
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.21428570151329041 1
      vertex 0.5 0.071428552269935608 0.8571428656578064
      vertex 0.5 0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal 0.99999999999997824 2.086162256276226e-07 0
    outer loop
      vertex 0.5 0.21428570151329041 1
      vertex 0.5 0.21428570151329041 0.8571428656578064
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 0.99999999999997824 0 -2.0861626914836125e-07
    outer loop
      vertex 0.5 0.21428570151329041 1
      vertex 0.49999997019767761 0.3571428656578064 0.8571428656578064
      vertex 0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.071428567171096802 1
      vertex 0.5 -0.071428567171096802 1
      vertex 0.5 -0.071428567171096802 0.8571428656578064
    endloop
  endfacet
  facet normal 1 0 -0
    outer loop
      vertex 0.5 0.071428567171096802 1
      vertex 0.5 -0.071428567171096802 0.8571428656578064
      vertex 0.5 0.071428552269935608 0.8571428656578064
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428552269935608 0.5 0.71428570151329041
      vertex 0.071428537368774414 0.5 0.5714285671710968
      vertex -0.071428574621677399 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428552269935608 0.5 0.71428570151329041
      vertex -0.071428574621677399 0.5 0.5714285671710968
      vertex -0.071428582072257996 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428537368774414 0.5 0.5714285671710968
      vertex 0.071428544819355011 0.5 0.4285714328289032
      vertex -0.071428574621677399 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex 0.071428537368774414 0.5 0.5714285671710968
      vertex -0.071428574621677399 0.5 0.4285714328289032
      vertex -0.071428574621677399 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex -0.071428574621677399 0.5 0.5714285671710968
      vertex -0.21428570151329041 0.5 0.5714285671710968
      vertex -0.21428570151329041 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.071428574621677399 0.5 0.5714285671710968
      vertex -0.21428570151329041 0.5 0.71428570151329041
      vertex -0.071428582072257996 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.071428574621677399 0.5 0.5714285671710968
      vertex -0.071428574621677399 0.5 0.4285714328289032
      vertex -0.21428570151329041 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 1 0
    outer loop
      vertex -0.071428574621677399 0.5 0.5714285671710968
      vertex -0.21428570151329041 0.5 0.4285714328289032
      vertex -0.21428570151329041 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.5714285671710968
      vertex -0.3571428656578064 0.5 0.5714285671710968
      vertex -0.3571428656578064 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.5714285671710968
      vertex -0.3571428656578064 0.5 0.71428570151329041
      vertex -0.21428570151329041 0.5 0.71428570151329041
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.21428570151329041 0.5 0.5714285671710968
      vertex -0.21428570151329041 0.5 0.4285714328289032
      vertex -0.3571428656578064 0.5 0.428571417927742
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.21428570151329041 0.5 0.5714285671710968
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.3571428656578064 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.5 0.5 0.4285714328289032
      vertex -0.5 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.5 0.5 0.5714285671710968
      vertex -0.3571428656578064 0.5 0.5714285671710968
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.21428570151329041 0.5 0.4285714328289032
      vertex -0.21428573131561279 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.21428573131561279 0.5 0.28571429848670959
      vertex -0.3571428656578064 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.3571428656578064 0.5 0.28571429848670959
      vertex -0.5 0.5 0.28571429848670959
    endloop
  endfacet
  facet normal -0 1 -0
    outer loop
      vertex -0.3571428656578064 0.5 0.428571417927742
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.5 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.5 0.3571428656578064 0.28571429848670959
      vertex -0.5 0.3571428656578064 0.428571417927742
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.5 0.3571428656578064 0.428571417927742
      vertex -0.5 0.5 0.4285714328289032
    endloop
  endfacet
  facet normal -0 0.99999999999997824 -2.086162256276226e-07
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.3571428656578064 0.5 0.28571429848670959
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal 2.0861626914836125e-07 0.99999999999997824 0
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
      vertex -0.5 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0.99999999999997824 -2.0861626914836125e-07 -0
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.5 0.5 0.1428571343421936
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
    endloop
  endfacet
  facet normal -0.99999999999997824 0 -2.086162256276226e-07
    outer loop
      vertex -0.5 0.5 0.28571429848670959
      vertex -0.49999997019767761 0.3571428656578064 0.1428571343421936
      vertex -0.5 0.3571428656578064 0.28571429848670959
    endloop
  endfacet
  facet normal 0 1 0
    outer loop
      vertex -0.3571428656578064 0.5 0.28571429848670959
      vertex -0.21428573131561279 0.5 0.28571429848670959
      vertex -0.21428570151329041 0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -2.0861622562761805e-07 0.99999999999995648 -2.0861622562761805e-07
    outer loop
      vertex -0.3571428656578064 0.5 0.28571429848670959
      vertex -0.21428570151329041 0.5 0.1428571343421936
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.21428570151329041 0.5 0.1428571343421936
      vertex -0.21428567171096802 0.5 0
      vertex -0.3571428656578064 0.5 0
    endloop
  endfacet
  facet normal -2.0861622562761805e-07 0.99999999999995648 2.0861626914835673e-07
    outer loop
      vertex -0.21428570151329041 0.5 0.1428571343421936
      vertex -0.3571428656578064 0.5 0
      vertex -0.3571428656578064 0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.3571428656578064 0.71428570151329041
      vertex -0.5 0.21428570151329041 0.71428570151329041
      vertex -0.5 0.21428570151329041 0.8571428656578064
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 0.3571428656578064 0.71428570151329041
      vertex -0.5 0.21428570151329041 0.8571428656578064
      vertex -0.5 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -0 -0 1
    outer loop
      vertex 0.21428570151329041 -0.3571428656578064 1
      vertex 0.21428570151329041 -0.5 1
      vertex 0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal 2.0861622562761805e-07 2.0861626914835673e-07 0.99999999999995648
    outer loop
      vertex 0.21428570151329041 -0.3571428656578064 1
      vertex 0.3571428656578064 -0.5 1
      vertex 0.3571428656578064 -0.3571428656578064 0.99999997019767761
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.21428570151329041 -0.5 1
      vertex 0.21428570151329041 -0.5 0.8571428656578064
      vertex 0.3571428656578064 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 1
      vertex 0.3571428656578064 -0.5 0.8571428656578064
      vertex 0.3571428656578064 -0.5 1
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.8571428656578064
      vertex 0.21428570151329041 -0.5 0.71428570151329041
      vertex 0.3571428656578064 -0.5 0.71428570151329041
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0.8571428656578064
      vertex 0.3571428656578064 -0.5 0.71428570151329041
      vertex 0.3571428656578064 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 -0
    outer loop
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex -0.071428589522838593 -0.5 0.28571429848670959
      vertex -0.071428596973419189 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex -0.071428596973419189 -0.5 0.1428571343421936
      vertex 0.071428582072257996 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex 0.071428582072257996 -0.5 0.1428571343421936
      vertex 0.21428570151329041 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428552269935608 -0.5 0.28571429848670959
      vertex 0.21428570151329041 -0.5 0.1428571343421936
      vertex 0.21428570151329041 -0.5 0.28571431338787079
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.1428571343421936
      vertex -0.071428596973419189 -0.5 0.1428571343421936
      vertex -0.071428596973419189 -0.5 0
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.1428571343421936
      vertex -0.071428596973419189 -0.5 0
      vertex 0.071428567171096802 -0.5 0
    endloop
  endfacet
  facet normal -0 -1 0
    outer loop
      vertex 0.071428582072257996 -0.5 0.1428571343421936
      vertex 0.071428567171096802 -0.5 0
      vertex 0.21428570151329041 -0.5 0
    endloop
  endfacet
  facet normal -0 -1 -0
    outer loop
      vertex 0.071428582072257996 -0.5 0.1428571343421936
      vertex 0.21428570151329041 -0.5 0
      vertex 0.21428570151329041 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.071428567171096802 -0.5 0
      vertex 0.071428567171096802 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -0 -0 -1
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.071428567171096802 -0.3571428656578064 0
      vertex 0.21428570151329041 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.21428570151329041 -0.3571428656578064 0
      vertex 0.3571428656578064 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.3571428656578064 -0.3571428656578064 0
      vertex 0.3571428656578064 -0.5 0
    endloop
  endfacet
  facet normal 0 -0.99999999999997824 2.0861626914836125e-07
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.3571428656578064 -0.5 0
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
    endloop
  endfacet
  facet normal 2.086162256276226e-07 -0.99999999999997824 -0
    outer loop
      vertex 0.21428570151329041 -0.5 0
      vertex 0.3571428656578064 -0.49999997019767761 0.1428571343421936
      vertex 0.21428570151329041 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 1 -0 -0
    outer loop
      vertex 0.5 -0.21428570151329041 0.4285714328289032
      vertex 0.5 -0.21428570151329041 0.28571432828903198
      vertex 0.5 -0.071428567171096802 0.28571429848670959
    endloop
  endfacet
  facet normal 1 0 0
    outer loop
      vertex 0.5 -0.21428570151329041 0.4285714328289032
      vertex 0.5 -0.071428567171096802 0.28571429848670959
      vertex 0.5 -0.071428567171096802 0.4285714328289032
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.071428574621677399 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.071428567171096802 0.42857146263122559
      vertex -0.5 -0.071428574621677399 0.5714285671710968
      vertex -0.5 0.071428574621677399 0.5714285671710968
    endloop
  endfacet
  facet normal -1 0 -0
    outer loop
      vertex -0.5 0.21428570151329041 0.8571428656578064
      vertex -0.5 0.21428570151329041 1
      vertex -0.5 0.3571428656578064 1
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 0.21428570151329041 0.8571428656578064
      vertex -0.5 0.3571428656578064 1
      vertex -0.5 0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal -0.99999999999995648 -2.0861622562761805e-07 -2.0861626914835673e-07
    outer loop
      vertex -0.5 -0.21428570151329041 0.8571428656578064
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
      vertex -0.5 -0.3571428656578064 1
    endloop
  endfacet
  facet normal -1 -0 -0
    outer loop
      vertex -0.5 -0.21428570151329041 0.8571428656578064
      vertex -0.5 -0.3571428656578064 1
      vertex -0.5 -0.21428570151329041 1
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.3571428656578064 0.71428570151329041
      vertex -0.5 -0.5 0.71428570151329041
      vertex -0.5 -0.5 0.8571428656578064
    endloop
  endfacet
  facet normal -0.99999999999995648 2.0861626914835673e-07 2.0861622562761805e-07
    outer loop
      vertex -0.5 -0.3571428656578064 0.71428570151329041
      vertex -0.5 -0.5 0.8571428656578064
      vertex -0.49999997019767761 -0.3571428656578064 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -0 -1
    outer loop
      vertex 0.21428570151329041 -0.3571428656578064 0
      vertex 0.21428570151329041 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.21428570151329041 0
    endloop
  endfacet
  facet normal 0 0 -1
    outer loop
      vertex 0.21428570151329041 -0.3571428656578064 0
      vertex 0.3571428656578064 -0.21428570151329041 0
      vertex 0.3571428656578064 -0.3571428656578064 0
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.5 0.5 0.8571428656578064
      vertex -0.5 0.5 1
      vertex -0.3571428656578064 0.5 1
    endloop
  endfacet
  facet normal 0 1 -0
    outer loop
      vertex -0.5 0.5 0.8571428656578064
      vertex -0.3571428656578064 0.5 1
      vertex -0.3571428656578064 0.5 0.8571428656578064
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0
      vertex -0.3571428656578064 -0.5 0.1428571343421936
      vertex -0.5 -0.5 0.1428571343421936
    endloop
  endfacet
  facet normal 0 -1 0
    outer loop
      vertex -0.3571428656578064 -0.5 0
      vertex -0.5 -0.5 0.1428571343421936
      vertex -0.5 -0.5 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.5 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0
    endloop
  endfacet
  facet normal -1 0 0
    outer loop
      vertex -0.5 -0.5 0.1428571343421936
      vertex -0.5 -0.3571428656578064 0
      vertex -0.5 -0.5 0
    endloop
  endfacet
endsolid OBJECT
)";
    return part1 + part2 + part3;
}

std::string test_data::big_cube()
{
    std::stringstream data;
    data << "solid STL generated by MeshLab\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  1.250e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 1.250e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -1.250e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  2.50e-01 1.250e-01 0.0e+0\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 -1.250e-01 6.250e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "      vertex  5.0e-01 -1.250e-01 3.750e-01\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  1.250e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 6.250e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "      vertex  1.250e-01 5.0e-01 6.250e-01\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 3.750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 3.750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.750e-01 8.750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 3.750e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.750e-01 1.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 3.750e-01 2.50e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -5.0e-01 8.750e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 5.0e-01 8.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 5.0e-01 1.250e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -3.1250e-01 -4.3750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "      vertex  4.3750e-01 3.1250e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 -3.1250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  -3.750e-01 4.3750e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  3.1250e-01 -4.3750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "      vertex  -4.3750e-01 3.1250e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  3.750e-01 -3.1250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  3.750e-01 4.3750e-01 0.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -4.3750e-01 1.8750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  -5.0e-01 3.1250e-01 9.3750e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -3.1250e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  -5.0e-01 4.3750e-01 1.250e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -4.3750e-01 8.1250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  5.0e-01 3.1250e-01 6.250e-02\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -3.1250e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  5.0e-01 4.3750e-01 8.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -3.1250e-01 -5.0e-01 6.250e-02\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "      vertex  4.3750e-01 -5.0e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 -5.0e-01 1.8750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "      vertex  -3.750e-01 -5.0e-01 9.3750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -3.1250e-01 5.0e-01 9.3750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "      vertex  4.3750e-01 5.0e-01 1.8750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 5.0e-01 8.1250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "      vertex  -3.750e-01 5.0e-01 6.250e-02\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 -4.3750e-01 1.0e+0\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  3.750e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  2.50e-01 -2.50e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -3.1250e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 1.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.1250e-01 -4.3750e-01 0.0e+0\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 -2.50e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 -3.1250e-01 0.0e+0\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 0.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -4.3750e-01 1.8750e-01\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 -3.750e-01 8.750e-01\n"
         << "      vertex  -5.0e-01 -2.50e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.1250e-01 1.250e-01\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 3.750e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 2.50e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -4.3750e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "      vertex  5.0e-01 -3.750e-01 1.250e-01\n"
         << "      vertex  5.0e-01 -2.50e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.1250e-01 8.750e-01\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 3.750e-01 2.50e-01\n"
         << "      vertex  5.0e-01 2.50e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 -5.0e-01 6.250e-02\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  2.50e-01 -5.0e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -5.0e-01 1.8750e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  2.50e-01 -5.0e-01 8.750e-01\n"
         << "      vertex  0.0e+0 -5.0e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 5.0e-01 9.3750e-01\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "      vertex  3.750e-01 5.0e-01 8.750e-01\n"
         << "      vertex  2.50e-01 5.0e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 5.0e-01 8.1250e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "      vertex  2.50e-01 5.0e-01 1.250e-01\n"
         << "      vertex  0.0e+0 5.0e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  4.3750e-01 3.1250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  2.50e-01 -2.50e-01 1.0e+0\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 4.3750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 1.250e-01 1.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -4.3750e-01 3.1250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 -2.50e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  3.750e-01 4.3750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 1.250e-01 0.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 0.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 3.1250e-01 9.3750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -1.250e-01 6.250e-01\n"
         << "      vertex  -5.0e-01 -2.50e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 4.3750e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 3.1250e-01 6.250e-02\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -1.250e-01 3.750e-01\n"
         << "      vertex  5.0e-01 -2.50e-01 2.50e-01\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 4.3750e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  4.3750e-01 -5.0e-01 8.1250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  2.50e-01 -5.0e-01 2.50e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 -5.0e-01 9.3750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 6.250e-01\n"
         << "      vertex  0.0e+0 -5.0e-01 7.50e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  4.3750e-01 5.0e-01 1.8750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 5.0e-01 6.250e-01\n"
         << "      vertex  2.50e-01 5.0e-01 7.50e-01\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 5.0e-01 6.250e-02\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 3.750e-01\n"
         << "      vertex  0.0e+0 5.0e-01 2.50e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  2.50e-01 -2.50e-01 1.0e+0\n"
         << "      vertex  1.250e-01 -1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "      vertex  1.250e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  -2.50e-01 1.250e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 -2.50e-01 0.0e+0\n"
         << "      vertex  -1.250e-01 -1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "      vertex  -1.250e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  0.0e+0 2.50e-01 0.0e+0\n"
         << "      vertex  2.50e-01 1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  2.50e-01 1.250e-01 0.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 -2.50e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 -1.250e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  -5.0e-01 -1.250e-01 6.250e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  -5.0e-01 1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "      vertex  5.0e-01 -2.50e-01 2.50e-01\n"
         << "      vertex  5.0e-01 -1.250e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  5.0e-01 -1.250e-01 3.750e-01\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  5.0e-01 1.250e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  5.0e-01 1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  2.50e-01 -5.0e-01 2.50e-01\n"
         << "      vertex  1.250e-01 -5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "      vertex  1.250e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  0.0e+0 -5.0e-01 7.50e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 6.250e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "      vertex  2.50e-01 5.0e-01 7.50e-01\n"
         << "      vertex  1.250e-01 5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "      vertex  1.250e-01 5.0e-01 6.250e-01\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "      vertex  0.0e+0 5.0e-01 2.50e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 3.750e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -2.50e-01 1.0e+0\n"
         << "      vertex  3.750e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  3.750e-01 -3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 2.50e-01 1.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -2.50e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 -3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 2.50e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 0.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -2.50e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 -3.750e-01 8.750e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -3.750e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  -5.0e-01 3.750e-01 7.50e-01\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 3.750e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -2.50e-01 2.50e-01\n"
         << "      vertex  5.0e-01 -3.750e-01 1.250e-01\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -3.750e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 2.50e-01 5.0e-01\n"
         << "      vertex  5.0e-01 3.750e-01 2.50e-01\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 3.750e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 -5.0e-01 2.50e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  3.750e-01 -5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 -5.0e-01 7.50e-01\n"
         << "      vertex  2.50e-01 -5.0e-01 8.750e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  2.50e-01 -5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  2.50e-01 5.0e-01 7.50e-01\n"
         << "      vertex  3.750e-01 5.0e-01 8.750e-01\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  3.750e-01 5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 5.0e-01 2.50e-01\n"
         << "      vertex  2.50e-01 5.0e-01 1.250e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  2.50e-01 5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 -4.3750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -1.250e-01 -3.750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -3.1250e-01 -4.3750e-01 1.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 1.0e+0\n"
         << "      vertex  3.750e-01 1.250e-01 1.0e+0\n"
         << "      vertex  4.3750e-01 3.1250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  4.3750e-01 3.1250e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -3.1250e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 -1.250e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 -3.1250e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  -2.50e-01 3.750e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 4.3750e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -0.0e+0 1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 4.3750e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.1250e-01 -4.3750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  1.250e-01 -3.750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  3.1250e-01 -4.3750e-01 0.0e+0\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 0.0e+0 0.0e+0\n"
         << "      vertex  -3.750e-01 1.250e-01 0.0e+0\n"
         << "      vertex  -4.3750e-01 3.1250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -4.3750e-01 3.1250e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 -3.1250e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  2.50e-01 -1.250e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  3.750e-01 -3.1250e-01 0.0e+0\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  2.50e-01 3.750e-01 0.0e+0\n"
         << "      vertex  3.750e-01 4.3750e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 0.0e+0 -1.0e+0\n"
         << "    outer loop\n"
         << "      vertex  3.750e-01 4.3750e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -4.3750e-01 1.8750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -3.750e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -4.3750e-01 1.8750e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  -5.0e-01 1.250e-01 8.750e-01\n"
         << "      vertex  -5.0e-01 3.1250e-01 9.3750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 3.1250e-01 9.3750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -3.1250e-01 1.250e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -1.250e-01 2.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 -3.1250e-01 1.250e-01\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 0.0e+0 0.0e+0\n"
         << "      vertex  -5.0e-01 3.750e-01 2.50e-01\n"
         << "      vertex  -5.0e-01 4.3750e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 4.3750e-01 1.250e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -4.3750e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -3.750e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -4.3750e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 5.0e-01\n"
         << "      vertex  5.0e-01 1.250e-01 1.250e-01\n"
         << "      vertex  5.0e-01 3.1250e-01 6.250e-02\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 3.1250e-01 6.250e-02\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -3.1250e-01 8.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -1.250e-01 7.50e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 -3.1250e-01 8.750e-01\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 0.0e+0 1.0e+0\n"
         << "      vertex  5.0e-01 3.750e-01 7.50e-01\n"
         << "      vertex  5.0e-01 4.3750e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 1.0e+0 -0.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  5.0e-01 4.3750e-01 8.750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 -5.0e-01 6.250e-02\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -1.250e-01 -5.0e-01 1.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -3.1250e-01 -5.0e-01 6.250e-02\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 -5.0e-01 5.0e-01\n"
         << "      vertex  3.750e-01 -5.0e-01 6.250e-01\n"
         << "      vertex  4.3750e-01 -5.0e-01 8.1250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  4.3750e-01 -5.0e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -5.0e-01 1.8750e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -2.50e-01 -5.0e-01 3.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 0.0e+0\n"
         << "      vertex  -3.750e-01 -5.0e-01 1.8750e-01\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 -5.0e-01 5.0e-01\n"
         << "      vertex  -2.50e-01 -5.0e-01 8.750e-01\n"
         << "      vertex  -3.750e-01 -5.0e-01 9.3750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 -1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 -5.0e-01 9.3750e-01\n"
         << "      vertex  5.0e-01 -5.0e-01 1.0e+0\n"
         << "      vertex  -5.0e-01 -5.0e-01 1.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.1250e-01 5.0e-01 9.3750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -1.250e-01 5.0e-01 8.750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -3.1250e-01 5.0e-01 9.3750e-01\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  0.0e+0 5.0e-01 5.0e-01\n"
         << "      vertex  3.750e-01 5.0e-01 3.750e-01\n"
         << "      vertex  4.3750e-01 5.0e-01 1.8750e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal -0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  4.3750e-01 5.0e-01 1.8750e-01\n"
         << "      vertex  5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 5.0e-01 8.1250e-01\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -2.50e-01 5.0e-01 6.250e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 1.0e+0\n"
         << "      vertex  -3.750e-01 5.0e-01 8.1250e-01\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -5.0e-01 5.0e-01 5.0e-01\n"
         << "      vertex  -2.50e-01 5.0e-01 1.250e-01\n"
         << "      vertex  -3.750e-01 5.0e-01 6.250e-02\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "  facet normal 0.0e+0 1.0e+0 0.0e+0\n"
         << "    outer loop\n"
         << "      vertex  -3.750e-01 5.0e-01 6.250e-02\n"
         << "      vertex  5.0e-01 5.0e-01 0.0e+0\n"
         << "      vertex  -5.0e-01 5.0e-01 0.0e+0\n"
         << "    endloop\n"
         << "  endfacet\n"
         << "endsolid vcg\n";
    return data.str();
}

std::string test_data::binary_stl()
{
    // This file was generated from a binary STL "cube_b.stl" & the following Python code:
    // file = open("cube_b.stl","rb")
    // list(file.read())
    const std::vector<unsigned char> bytes({32, 115, 111, 108, 105, 100, 32, 80, 82, 79, 50, 83, 84, 76, 32, 118, 101, 114, 115, 105, 111, 110, 32, 49, 46, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 128, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 128, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 128, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 128, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 128, 191, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 128, 191, 0, 0, 0, 128, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 128, 63, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0, 128, 63, 0, 0});
    return std::string(bytes.begin(), bytes.end());
}
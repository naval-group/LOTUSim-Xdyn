#include <cstring>
#include <string>
#include <sstream>
#include <cstdio>
#include <algorithm>

#include "MeshException.hpp"
#include "external_file_formats/stl_reader.hpp"

const int LINE_MAX_LENGTH = 256;
const char TOKEN_vertex[] = "vertex";
const char TOKEN_normal[] = "normal";

struct ParserState;
EPoint read_vertex(char *line, ParserState& state);
void read_normal(char *line, Eigen::Vector3d& vertices, ParserState& state);
VectorOfVectorOfPoints read_ascii_stl(std::istream& input_stream, ParserState &state);

struct ParserState {
    ParserState(std::istream& istream):
        input_stream(istream){}

    std::string parse_error_string(const std::string &token){
        std::ostringstream ostr;
        ostr << "Error parsing \""<< token;
        return ostr.str();
    }

    std::string error_unknown_string(const std::string &token){
        std::ostringstream ostr;
        ostr << "Error unknown token "<< token;
        return ostr.str();
    }

    void get_line(char *line){
        input_stream.getline(line, LINE_MAX_LENGTH);
    }

    std::istream& input_stream;
};

/**
 * \brief reads a vertex from a line
 */
EPoint read_vertex(char *line, ParserState& state)
{
    EPoint ret;
    int width;
    char token[20] = "";
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_vertex))
    {
        THROW(__PRETTY_FUNCTION__, MeshException, state.parse_error_string(TOKEN_vertex));
    }
    sscanf(line+width,"%lf %lf %lf",&ret(0),&ret(1),&ret(2));
    return ret;
}

void read_normal(char *line, Eigen::Vector3d& vertices, ParserState& state){
    int width;
    char token[20];
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_normal)){
        THROW(__PRETTY_FUNCTION__, MeshException, state.parse_error_string(TOKEN_normal));
    }
    sscanf(line+width,"%lf %lf %lf",&vertices(0),&vertices(1),&vertices(2));
}

/**
 * \brief reads the ASCII file from a char input stream
 */
VectorOfVectorOfPoints read_ascii_stl(
    std::istream& input_stream, //!<
    ParserState &state)
{
    VectorOfVectorOfPoints result;
    //Start by storing the current locale. This is retrieved by passing NULL to
    // setlocale
    std::string locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "C");
    double r1 = 0.0, r2 = 0.0, r3 = 0.0, r4 = 0.0;
    bool endReached = false;
    char token[LINE_MAX_LENGTH];
    int  width = 0;
    char *next = NULL;
    char input[LINE_MAX_LENGTH];
    state.get_line(input);
    Eigen::Vector3d normal;

    // while characters still exists and no errors occurs
    while (input_stream.fail() == 0 && input_stream.eof() == 0) {
        // Read the next line of the file into INPUT.
        state.get_line(input);

        // Advance to the first nonspace character in INPUT.
        // 32==SPACE character
        for (next = input; *next != '\0' && *next == 32; next++){}
        // Skip blank lines and comments and linebreaks.
        if (*next == '\0' || *next == '#' || *next == '!' || *next == '$') {
            continue;
        }
        // Extract the first word in this line.
        sscanf(next, "%s%n", token, &width);
        // Set NEXT to point to just after this token.
        next = next + width;
        // FACET
        VectorOfPoints facet;
        if (!strcmp(token, "facet")){
            VectorOfPoints facet;
            // Get the XYZ coordinates of the normal vector to the face.
            read_normal(next, normal, state);
            state.get_line(input);
            // Get the XYZ coordinates of the vertex1 vector
            state.get_line(input);
            facet.push_back(read_vertex(input, state));
            // Get the XYZ coordinates of the vertex2 vector
            state.get_line(input);
            facet.push_back(read_vertex(input, state));
            // Get the XYZ coordinates of the vertex3 vector
            state.get_line(input);
            facet.push_back(read_vertex(input, state));
            // closeloop
            state.get_line(input);
            // endfacet
            state.get_line(input);
            //
            result.push_back(facet);
        } else if (!strcmp(token, "color")) { // COLOR
            sscanf(next, "%*s %lf %lf %lf %lf", &r1, &r2, &r3, &r4);
        } else if (!strcmp(token, "solid")) { // SOLID
            // object_num = object_num + 1;
        } else if (!strcmp(token, "endsolid")){ // ENDSOLID
            endReached = true;
        } else { // Unexpected or unrecognized.
            setlocale(LC_ALL, locale.c_str());
            THROW(__PRETTY_FUNCTION__, MeshException, state.error_unknown_string(token));
        }
    }
    if(!endReached){
        THROW(__PRETTY_FUNCTION__, MeshException,
                "The 'endsolid' keyword was not found before the end of file. "
                 "The file may be damaged or is a binary STL format. "
                 "A binary STL file must not have 'solid' keyword in header.");
    }
    setlocale(LC_ALL, locale.c_str());
    return result;
}

VectorOfVectorOfPoints read_stl(const std::string& input)
{
    switch(identify_stl(input))
    {
        case StlType::ASCII:
        {
            std::istringstream inputStream(input);
            ParserState state(inputStream);
            return read_ascii_stl(inputStream, state);
        }
        case StlType::BINARY:
            return read_binary_stl(input);
        case StlType::UNKNOWN:
            THROW(__PRETTY_FUNCTION__, MeshException, "Unable to identify the type of STL file (binary or ASCII)");
    }
    return VectorOfVectorOfPoints();
}

/* The specification of binary STL files can be found here:
 * https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL
 *
 * UINT8[80]    – Header                 -     80 bytes
 * UINT32       – Number of triangles    -      4 bytes
 *
 * foreach triangle                      - 50 bytes:
 *     REAL32[3] – Normal vector             - 12 bytes
 *     REAL32[3] – Vertex 1                  - 12 bytes
 *     REAL32[3] – Vertex 2                  - 12 bytes
 *     REAL32[3] – Vertex 3                  - 12 bytes
 *     UINT16    – Attribute byte count      -  2 bytes
 * end
 */
VectorOfVectorOfPoints read_binary_stl(std::istream& stream) // Shamelessly copied from http://ravehgonen.wordpress.com/tag/stl-file-format/
{
    char buffer[4] = {0};
    VectorOfVectorOfPoints ret;

    stream.ignore(80);
    stream.read(buffer, sizeof buffer);
    unsigned int nFaces = 0;
    memcpy((void*)&nFaces, (void*)&buffer, sizeof(buffer));

    float v[12]; // normal=3 + vertices=3*3 = 12

    // Every Face is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
    for (size_t i=0; i<nFaces; ++i)
    {
        if (stream.good())
        {
            stream.ignore(12); // Ignore the normal (MeshBuilder recalculates it anyway)
            for (size_t j=0; j<9; ++j)
            {
                // As per http://stackoverflow.com/a/12861635
                for (size_t k = 0 ; k < sizeof buffer ; ++k)
                {
                    char c;
                    stream.get(c);
                    *(((char*)&(v[j])) + k) = c;//fgetc(buffer);
                }
            }
            stream.ignore(2); // Ignore separator
        }

       VectorOfPoints t;
       t.push_back(EPoint(v[0], v[1], v[2]));
       t.push_back(EPoint(v[3], v[4], v[5]));
       t.push_back(EPoint(v[6], v[7], v[8]));
       ret.push_back(t);
    }
    return ret;
}

VectorOfVectorOfPoints read_binary_stl(const std::string& input)
{
    std::stringstream ss(input);
    return read_binary_stl(ss);
}

std::ostream& operator<<(std::ostream& out, const StlType& stl_type)
{
    switch(stl_type)
    {
        case StlType::ASCII:
            out << "ASCII";
            break;
        case StlType::UNKNOWN:
            out << "UNKNOWN";
            break;
        case StlType::BINARY:
            out << "BINARY";
            break;
    }
    return out;
}

int get_nb_of_triangles(const std::string& bytes);
int get_nb_of_triangles(const std::string& bytes)
{
    // "Following the header is a 4-byte little-endian unsigned integer indicating the number of
    // triangular facets in the file" (cf. https://en.wikipedia.org/wiki/STL_(file_format))
    // Little endian: "A little-endian system [...] stores the least-significant byte at the
    // smallest address" (cf. https://en.wikipedia.org/wiki/Endianness)
    if (bytes.size() < 84)
    {
        return 0;
    }

    int nb_of_triangles = bytes[83];
    nb_of_triangles = (nb_of_triangles << 8) + bytes[82];
    nb_of_triangles = (nb_of_triangles << 8) + bytes[81];
    nb_of_triangles = (nb_of_triangles << 8) + bytes[80];
    return nb_of_triangles;
}

bool file_size_is_a_valid_binary_stl_file_size(const std::string& input);
bool file_size_is_a_valid_binary_stl_file_size(const std::string& input)
{
    const size_t header_size = 80;
    const size_t nb_of_bytes_for_nb_of_triangles = 4;
    const size_t expected_binary_stl_size = header_size +
        + nb_of_bytes_for_nb_of_triangles
        + 50 * static_cast<size_t>(get_nb_of_triangles(input));
    return input.size() == expected_binary_stl_size;
}

StlType identify_stl(const std::string& input)
{
    const size_t solid_pos = input.find("solid");
    const size_t endsolid_pos = input.find("endsolid", solid_pos);
    if (solid_pos != std::string::npos)
    {
        if (endsolid_pos != std::string::npos)
        {
            if ((endsolid_pos < 80) && file_size_is_a_valid_binary_stl_file_size(input))
            {
                return StlType::BINARY;
            }
            return StlType::ASCII;
        }
        if (file_size_is_a_valid_binary_stl_file_size(input))
        {
            return StlType::BINARY;
        }
        THROW(__PRETTY_FUNCTION__, MeshException, "Unable to determine if mesh is a binary or an ASCII STL: file contained the 'solid' keyword, but not 'endsolid' (so it can't be an ASCII STL) and its size is not 84 + 50*n (where n is the number of triangles at bytes 81,82,83,84) so it can't be a binary STL either.")
        return StlType::UNKNOWN;
    }
    if (file_size_is_a_valid_binary_stl_file_size(input))
    {
        return StlType::BINARY;
    }
    THROW(__PRETTY_FUNCTION__, MeshException, "Unable to determine if mesh is a binary or an ASCII STL: file did not contain the 'solid' keyword (so it can't be an ASCII STL), and its size is not 84 + 50*n (where n is the number of triangles at bytes 81,82,83,84) so it can't be a binary STL either.")
    return StlType::UNKNOWN;
}

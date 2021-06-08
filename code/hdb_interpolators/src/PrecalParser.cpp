/*
 * PrecalParser.cpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#include "PrecalParser.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <sstream>

/**
 * @brief Add the trimmed contents of the buffer to the list of tokens & reset the buffer.
 *
 * @param buffer
 * @param tokens
 */
void flush(std::string& buffer, std::vector<std::string>& tokens);
void flush(std::string& buffer, std::vector<std::string>& tokens)
{
    boost::trim(buffer);
    if (not(buffer.empty()))
    {
        tokens.push_back(buffer);
    }
    buffer = "";
}

class LineGetter
{
  public:
    virtual ~LineGetter() {}
    virtual std::string get_next_line() = 0;
    virtual bool has_more_lines() const = 0;
};

class GetLineFromString : public LineGetter
{
  public:
    GetLineFromString(const std::string& s)
        : stream(s)
    {
    }
    std::string get_next_line()
    {
        std::string line;
        std::getline(stream, line);
        return line;
    }
    bool has_more_lines() const { return not(stream.eof()); }

  private:
    std::istringstream stream;
};
class Parser
{
  public:
    Parser(LineGetter& line_getter_)
        : line_getter(line_getter_)
        , current_section()
        , parsed_sections()
        , current_vector_value()
        , current_line_number(0)
        , raos()
        , current_rao()
        , started_parsing_raos(false)
    {
    }

    void parse()
    {
        while (line_getter.has_more_lines())
        {
            current_line_number++;
            std::string current_line = line_getter.get_next_line();
            if (current_line == "[RAOs]")
            {
                started_parsing_raos = true;
            }
            else
            {
                if (started_parsing_raos)
                {
                    if (not(current_line.empty()))
                    {
                        if (current_line[0] == ';')
                        {
                            if (not(current_rao.title_line.empty()))
                            {
                                raos.push_back(current_rao);
                                current_rao = RAO();
                            }
                            current_rao.title_line = current_line;
                            current_rao.title_line.erase(0, 2);
                            current_rao.attributes = parse_rao_attributes(current_rao.title_line);
                        }
                        else
                        {
                            double left = 0;
                            double right = 0;
                            boost::trim(current_line);
                            if (current_line.find(' ') != std::string::npos)
                            {
                                sscanf(current_line.c_str(), "%lf   %lf", &left, &right);
                            }
                            else
                            {
                                sscanf(current_line.c_str(), "%lf", &left);
                            }
                            current_rao.left_column.push_back(left);
                            current_rao.right_column.push_back(right);
                        }
                    }
                }
                else
                {
                    const auto tokens = tokenize(current_line);
                    syntax_analysis(tokens);
                }
            }
        }
    }
    std::vector<RAO> get_raos() const { return raos; }

    std::vector<Section> get_sections()
    {
        if (not(current_section.title.empty()))
        {
            if (not(current_vector_value.first.empty()))
            {
                current_section.vector_values[current_vector_value.first]
                    = current_vector_value.second;
            }
            parsed_sections.push_back(current_section);
        }
        current_section.title = "";
        return parsed_sections;
    }

  private:
    std::vector<std::string> tokenize(const std::string& line) const
    {
        std::vector<std::string> tokens;
        std::string buffer;
        for (const char c : line)
        {
            if (c == ';')
            {
                flush(buffer, tokens);
                tokens.push_back(";");
            }
            else if (c == '=')
            {
                flush(buffer, tokens);
                tokens.push_back("=");
            }
            else if (c == '[')
            {
                tokens.push_back("[");
                buffer = "";
            }
            else if (c == ']')
            {
                flush(buffer, tokens);
                tokens.push_back("]");
            }
            else if (c == ',')
            {
                flush(buffer, tokens);
                tokens.push_back(",");
            }
            else if (c == '{')
            {
                flush(buffer, tokens);
                tokens.push_back("{");
            }
            else if (c == '}')
            {
                flush(buffer, tokens);
                tokens.push_back("}");
            }
            else
            {
                buffer.push_back(c);
            }
        }
        boost::trim(buffer);
        if (not(buffer.empty()))
        {
            tokens.push_back(buffer);
        }
        return tokens;
    }

    void add_key_value(const std::string& key, const std::string& value)
    {
        try
        {
            add_scalar_value_to_current_section(key, boost::lexical_cast<double>(value));
        }
        catch (const std::exception&)
        {
            add_string_value_to_current_section(key, value);
        }
    }

    void syntax_analysis(const std::vector<std::string>& tokens)
    {
        bool vector_key_value = false;
        std::string previous_token = "";
        std::string current_key = "";
        for (const auto token : tokens)
        {
            if (token == ";")
            {
                if (not(current_key.empty()))
                {
                    add_key_value(current_key, previous_token);
                    current_key = "";
                }
                else if (vector_key_value)
                {
                    try
                    {
                        append_value_to_current_vector_value(
                            boost::lexical_cast<double>(previous_token));
                    }
                    catch (const std::exception&)
                    {
                    }
                }
                previous_token = "";
                return;
            }
            else if (token == "[")
            {
                previous_token = "";
            }
            else if (token == "]")
            {
                new_section(previous_token);
                previous_token = "";
            }
            else if (token == "{")
            {
                vector_key_value = true;
                if (not(current_key.empty()))
                {
                    new_vector_value(current_key);
                }
            }
            else if (token == "}")
            {
                vector_key_value = false;
                current_key = "";
            }
            else if (token == ",")
            {
            }
            else if (token == "=")
            {
                current_key = previous_token;
                previous_token = token;
            }
            else
            {
                if (not(current_key.empty()))
                {
                    if (vector_key_value)
                    {
                        try
                        {
                            append_value_to_current_vector_value(
                                boost::lexical_cast<double>(token));
                        }
                        catch (const std::exception&)
                        {
                        }
                    }
                    else
                    {
                        add_key_value(current_key, token);
                        current_key = "";
                    }
                }
                previous_token = token;
            }
        }
    }

    void new_section(const std::string& section_title)
    {
        if (not(current_section.title.empty()))
        {
            parsed_sections.push_back(current_section);
        }
        current_section.title = section_title;
    }

    void add_scalar_value_to_current_section(const std::string& key, const double value)
    {
        current_section.scalar_values[key] = value;
    }

    void add_string_value_to_current_section(const std::string& key, const std::string& value)
    {
        current_section.string_values[key] = value;
    }

    void new_vector_value(const std::string& name)
    {
        if (not(current_vector_value.first.empty()))
        {
            current_section.vector_values[current_vector_value.first] = current_vector_value.second;
        }
        current_vector_value.first = name;
        current_vector_value.second = {};
    }

    void append_value_to_current_vector_value(const double value)
    {
        current_vector_value.second.push_back(value);
    }

    LineGetter& line_getter;
    Section current_section;
    std::vector<Section> parsed_sections;
    std::pair<std::string, std::vector<double> > current_vector_value;
    size_t current_line_number;
    std::vector<RAO> raos;
    RAO current_rao;
    bool started_parsing_raos;
};

PrecalFile parse_precal(const std::string& input)
{
    GetLineFromString line_getter(input);
    Parser parser(line_getter);
    parser.parse();
    PrecalFile ret;
    ret.sections = parser.get_sections();
    ret.raos = parser.get_raos();
    return ret;
}

Section::Section()
    : title()
    , scalar_values()
    , string_values()
    , vector_values()
{
}

RAO::RAO()
    : title_line()
    , attributes()
    , left_column()
    , right_column()
{
}

PrecalFile::PrecalFile()
    : sections()
    , raos()
{
}

RAOAttributes::RAOAttributes()
    : name()
    , position()
    , h()
    , h_unit()
    , phi_a()
    , phi_a_unit()
    , U()
    , U_unit()
    , mu()
    , mu_unit()
    , amplitude_unit()
    , phase_unit()
{
}

std::vector<std::string> rao_lexer(const std::string rao_attributes);
std::vector<std::string> rao_lexer(const std::string rao_attributes)
{
    std::vector<std::string> tokens;
    std::string buffer;
    for (const char c : rao_attributes)
    {
        if (c == ':')
        {
            flush(buffer, tokens);
            tokens.push_back(":");
        }
        else if (c == ' ')
        {
            flush(buffer, tokens);
        }
        else if (c == '(')
        {
            flush(buffer, tokens);
            tokens.push_back("[");
        }
        else if (c == ')')
        {
            flush(buffer, tokens);
            tokens.push_back(")");
        }
        else if (c == ',')
        {
            flush(buffer, tokens);
            tokens.push_back(",");
        }
        else if (c == '=')
        {
            flush(buffer, tokens);
            tokens.push_back("=");
        }
        else
        {
            buffer.push_back(c);
        }
    }
    boost::trim(buffer);
    if (not(buffer.empty()))
    {
        tokens.push_back(buffer);
    }
    return tokens;
}

struct UnitValue
{
    UnitValue() = delete;

    std::string name;
    std::string unit;
    double value;

    static UnitValue parse(std::string s)
    {
        std::string key = "";
        double val = 0;
        std::string u = "";
        size_t i = 0;
        while (i < s.length() && s[i] != '=')
        {
            key.append(1, s[i]);
            i++;
        }
        s = s.substr(i + 1, s.size());
        char str[256];
        sscanf(s.c_str(), "%lf%s", &val, str);
        u = str;
        return UnitValue(key, val, u);
    }

  private:
    UnitValue(const std::string& name_, const double value_, const std::string unit_)
        : name(name_)
        , unit(unit_)
        , value(value_)
    {
    }
};

void assign(RAOAttributes& left, const UnitValue& right);
void assign(RAOAttributes& left, const UnitValue& right)
{
    if (right.name == "h")
    {
        left.h = right.value;
        left.h_unit = right.unit;
    }
    else if (right.name == "phi_a")
    {
        left.phi_a = right.value;
        left.phi_a_unit = right.unit;
    }
    else if (right.name == "U")
    {
        left.U = right.value;
        left.U_unit = right.unit;
    }
    else if (right.name == "mu")
    {
        left.mu = right.value;
        left.mu_unit = right.unit;
    }
}

RAOAttributes analyze_rao_tokens(const std::vector<std::string>& tokens);
RAOAttributes analyze_rao_tokens(const std::vector<std::string>& tokens)
{
    RAOAttributes ret;
    std::string token = "";
    size_t i = 0;
    std::string header = "";
    while (tokens[i] != ":" && i < tokens.size())
    {
        if (header.empty())
        {
            header = tokens[i];
        }
        else
        {
            header.append(std::string(" ") + tokens[i]);
        }
        i++;
    }
    i++; // Skip the ":" token itself
    while (tokens[i] != "at" && i < tokens.size())
    {
        if (ret.name.empty())
        {
            ret.name = tokens[i];
        }
        else
        {
            ret.name.append(std::string(" ") + tokens[i]);
        }
        i++;
    }
    i++; // Skip the "at" token itself
    i++; // Skip the "(" token
    ret.position = Eigen::Vector3d(boost::lexical_cast<double>(tokens[i]),
                                   boost::lexical_cast<double>(tokens[i + 2]),
                                   boost::lexical_cast<double>(tokens[i + 4]));
    i += 7; // Move past the 'position' tokens
    assign(ret, UnitValue::parse(tokens[i] + tokens[i + 1] + tokens[i + 2]));
    i += 4;
    assign(ret, UnitValue::parse(tokens[i] + tokens[i + 1] + tokens[i + 2]));
    i += 4;
    assign(ret, UnitValue::parse(tokens[i] + tokens[i + 1] + tokens[i + 2]));
    i += 4;
    assign(ret, UnitValue::parse(tokens[i] + tokens[i + 1] + tokens[i + 2]));

    for (; i < tokens.size() - 3; ++i)
    {
        if ((tokens[i] == "amplitude") && (tokens[i + 1] == "unit"))
        {
            ret.amplitude_unit = tokens[i + 3];
        }
        if ((tokens[i] == "phase") && (tokens[i + 1] == "unit"))
        {
            ret.phase_unit = tokens[i + 3];
        }
    }
    return ret;
}

RAOAttributes parse_rao_attributes(const std::string rao_attributes)
{
    const std::vector<std::string> tokens = rao_lexer(rao_attributes);
    return analyze_rao_tokens(tokens);
}
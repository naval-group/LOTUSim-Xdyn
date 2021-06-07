/*
 * PrecalParser.cpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#include "PrecalParser.hpp"
#include <cstdio>
#include <sstream>

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
            const std::string current_line = line_getter.get_next_line();
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
                            if (not(current_rao.title.empty()))
                            {
                                raos.push_back(current_rao);
                            }
                            current_rao.title = current_line;
                        }
                        else
                        {
                            double left, right;
                            sscanf(current_line.c_str(), "   %lf   %lf", &left, &right);
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
                tokens.push_back(";");
                buffer = "";
            }
            else if (c == '=')
            {
                tokens.push_back(buffer);
                tokens.push_back("=");
                buffer = "";
            }
            else if (c == '[')
            {
                tokens.push_back("[");
                buffer = "";
            }
            else if (c == ']')
            {
                tokens.push_back(buffer);
                tokens.push_back("]");
                buffer = "";
            }
            else if (c == ',')
            {
                tokens.push_back(buffer);
                tokens.push_back(",");
                buffer = "";
            }
            else if (c == '{')
            {
                tokens.push_back(buffer);
                tokens.push_back("{");
                buffer = "";
            }
            else if (c == '}')
            {
                tokens.push_back(buffer);
                tokens.push_back("}");
                buffer = "";
            }
            else
            {
                buffer.push_back(c);
            }
        }
        if (not(buffer.empty()))
        {
            tokens.push_back(buffer);
        }
        return tokens;
    }

    void syntax_analysis(const std::vector<std::string>& tokens)
    {
        bool key_value = false;
        bool vector_key_value = false;
        std::string previous_token = "";
        for (const auto token : tokens)
        {
            if (token == ";")
            {
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
            }
            else if (token == "}")
            {
                vector_key_value = false;
            }
            else if (token == ",")
            {
                if (vector_key_value)
                {
                    append_value_to_current_vector_value(std::stod(previous_token));
                    previous_token = "";
                }
            }
            else if (token == "=")
            {
                key_value = true;
            }
            else
            {
                if (key_value)
                {
                    try
                    {
                        add_scalar_value_to_current_section(previous_token, std::stod(token));
                    }
                    catch (const std::invalid_argument&)
                    {
                        add_string_value_to_current_section(previous_token, token);
                    }
                }
                else if (vector_key_value)
                {
                    append_value_to_current_vector_value(std::stod(previous_token));
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
    : title()
    , left_column()
    , right_column()
{
}

PrecalFile::PrecalFile()
    : sections()
    , raos()
{
}
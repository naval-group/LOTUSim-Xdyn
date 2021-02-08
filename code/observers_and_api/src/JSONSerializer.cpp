#include <iomanip> // std::setprecision

#include <ssc/json.hpp>
#include <ssc/json/rapidjson/writer.h>
#include <ssc/json/rapidjson/stringbuffer.h>

#include "JSONSerializer.hpp"

YamlSimServerInputs deserialize(const std::string& input)
{
    rapidjson::Document document;
    ssc::json::parse(input, document);
    YamlSimServerInputs infos;
    if (not(document.IsObject()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "JSON should be an object (i.e. within curly braces), but it's not (it's a " << ssc::json::print_type(document) << "). The JSON we're looking at was: " << ssc::json::dump(document));
    }
    if (not(document.HasMember("states")))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Missing key 'states' in JSON root.")
    }
    if (not(document["states"].IsArray()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Expecting a JSON array but got '" << ssc::json::dump(document["states"]));
    }
    infos.Dt = ssc::json::find_optional_double("Dt", document, 0);
    for (rapidjson::Value& v:document["states"].GetArray())
    {
        YamlState s;
        s.t = ssc::json::find_double("t", v);
        s.x = ssc::json::find_double("x", v);
        s.y = ssc::json::find_double("y", v);
        s.z = ssc::json::find_double("z", v);
        s.u = ssc::json::find_double("u", v);
        s.v = ssc::json::find_double("v", v);
        s.w = ssc::json::find_double("w", v);
        s.p = ssc::json::find_double("p", v);
        s.q = ssc::json::find_double("q", v);
        s.r = ssc::json::find_double("r", v);
        s.qr = ssc::json::find_double("qr", v);
        s.qi = ssc::json::find_double("qi", v);
        s.qj = ssc::json::find_double("qj", v);
        s.qk = ssc::json::find_double("qk", v);
        infos.states.push_back(s);
    }
    if (document.HasMember("commands"))
    {
        const rapidjson::Value& commands = document["commands"];
        if (not(commands.IsObject()) && not(commands.IsNull()))
        {
            THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "'commands' should be a JSON object (key-values): got " << ssc::json::print_type(commands))
        }
        else
        {
            if (not(commands.IsNull()))
            {
                for (rapidjson::Value::ConstMemberIterator it = commands.MemberBegin(); it != commands.MemberEnd(); ++it)
                {
                    infos.commands[it->name.GetString()] = ssc::json::find_double(it->name.GetString(), commands);
                }
            }
        }
    }
    return infos;
}

typedef rapidjson::Writer<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::kWriteNanAndInfFlag> InfNaNWriter;

void write(InfNaNWriter& writer, const std::map<std::string, double>& m);
void write(InfNaNWriter& writer, const std::map<std::string, double>& m)
{
    writer.StartObject();
    for (const auto key_value:m)
    {
        writer.Key(key_value.first.c_str());
        writer.Double(key_value.second);
    }
    writer.EndObject();
}

#define WRITE_KEY_VALUE(key, value) writer.Key(key); writer.Double(value);

std::string serialize(const std::vector<YamlState>& states)
{
    rapidjson::StringBuffer s;
    InfNaNWriter writer(s);

    writer.StartArray();
    for (auto state:states)
    {
        writer.StartObject();
        WRITE_KEY_VALUE("t", state.t);
        WRITE_KEY_VALUE("x", state.x);
        WRITE_KEY_VALUE("y", state.y);
        WRITE_KEY_VALUE("z", state.z);
        WRITE_KEY_VALUE("u", state.u);
        WRITE_KEY_VALUE("v", state.v);
        WRITE_KEY_VALUE("w", state.w);
        WRITE_KEY_VALUE("p", state.p);
        WRITE_KEY_VALUE("q", state.q);
        WRITE_KEY_VALUE("r", state.r);
        WRITE_KEY_VALUE("qr", state.qr);
        WRITE_KEY_VALUE("qi", state.qi);
        WRITE_KEY_VALUE("qj", state.qj);
        WRITE_KEY_VALUE("qk", state.qk);
        WRITE_KEY_VALUE("phi", state.phi);
        WRITE_KEY_VALUE("theta", state.theta);
        WRITE_KEY_VALUE("psi", state.psi);
        writer.Key("extra_observations");
        write(writer, state.extra_observations);
        writer.EndObject();
    }
    writer.EndArray();

    return s.GetString();
}

std::string serialize(const std::vector<double>& dx_dt)
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writer.StartArray();
    WRITE_KEY_VALUE("dx_dt", dx_dt[0]);
    WRITE_KEY_VALUE("dy_dt", dx_dt[1]);
    WRITE_KEY_VALUE("dz_dt", dx_dt[2]);
    WRITE_KEY_VALUE("du_dt", dx_dt[3]);
    WRITE_KEY_VALUE("dv_dt", dx_dt[4]);
    WRITE_KEY_VALUE("dw_dt", dx_dt[5]);
    WRITE_KEY_VALUE("dp_dt", dx_dt[6]);
    WRITE_KEY_VALUE("dq_dt", dx_dt[7]);
    WRITE_KEY_VALUE("dr_dt", dx_dt[8]);
    WRITE_KEY_VALUE("dqr_dt", dx_dt[9]);
    WRITE_KEY_VALUE("dqi_dt", dx_dt[10]);
    WRITE_KEY_VALUE("dqj_dt", dx_dt[11]);
    WRITE_KEY_VALUE("dqk_dt", dx_dt[12]);
    writer.EndObject();
    return s.GetString();
}



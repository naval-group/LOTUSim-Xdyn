#include <iomanip> // std::setprecision

#include <ssc/json.hpp>
#include <ssc/json/rapidjson/document.h>
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
    if (document.HasMember("requested_output"))
    {
        const rapidjson::Value& commands = document["requested_output"];
        if (not(commands.IsArray()) && not(commands.IsNull()))
        {
            THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "'commands' should be a JSON array: got " << ssc::json::print_type(commands))
        }
        else
        {
            if (not(commands.IsNull()))
            {
                for (rapidjson::Value::ConstValueIterator it = commands.Begin(); it != commands.End(); ++it)
                {
                    infos.requested_output.push_back(it->GetString());
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

#define ADD_ARRAY(key, doc) doc.AddMember(key,rapidjson::Value(rapidjson::kArrayType).Move(), doc.GetAllocator())
#define PUSH_BACK(array, value, doc) array.PushBack(value, doc.GetAllocator())

std::string serialize(const std::vector<YamlState>& states)
{
    rapidjson::Document doc(rapidjson::kObjectType);

    // Initialization
    ADD_ARRAY("t", doc);
    ADD_ARRAY("x", doc);
    ADD_ARRAY("y", doc);
    ADD_ARRAY("z", doc);
    ADD_ARRAY("u", doc);
    ADD_ARRAY("v", doc);
    ADD_ARRAY("w", doc);
    ADD_ARRAY("p", doc);
    ADD_ARRAY("q", doc);
    ADD_ARRAY("r", doc);
    ADD_ARRAY("qr", doc);
    ADD_ARRAY("qi", doc);
    ADD_ARRAY("qj", doc);
    ADD_ARRAY("qk", doc);
    ADD_ARRAY("phi", doc);
    ADD_ARRAY("theta", doc);
    ADD_ARRAY("psi", doc);
    doc.AddMember("extra_observations",rapidjson::Value(rapidjson::kObjectType).Move(), doc.GetAllocator());
    for(auto extra_obs:states.at(0).extra_observations)
    {
        rapidjson::Value field_name(extra_obs.first.c_str(), doc.GetAllocator());
        doc["extra_observations"].AddMember(field_name, rapidjson::Value(rapidjson::kArrayType).Move(), doc.GetAllocator());
    }

    // Writing
    for(auto state:states)
    {
        PUSH_BACK(doc["t"], state.t, doc);
        PUSH_BACK(doc["x"], state.t, doc);
        PUSH_BACK(doc["y"], state.t, doc);
        PUSH_BACK(doc["z"], state.t, doc);
        PUSH_BACK(doc["u"], state.t, doc);
        PUSH_BACK(doc["v"], state.t, doc);
        PUSH_BACK(doc["w"], state.t, doc);
        PUSH_BACK(doc["p"], state.t, doc);
        PUSH_BACK(doc["q"], state.t, doc);
        PUSH_BACK(doc["r"], state.t, doc);
        PUSH_BACK(doc["qr"], state.t, doc);
        PUSH_BACK(doc["qi"], state.t, doc);
        PUSH_BACK(doc["qj"], state.t, doc);
        PUSH_BACK(doc["qk"], state.t, doc);
        PUSH_BACK(doc["phi"], state.t, doc);
        PUSH_BACK(doc["theta"], state.t, doc);
        PUSH_BACK(doc["psi"], state.t, doc);
        for(auto obs:state.extra_observations)
            {
                PUSH_BACK(doc["extra_observations"][obs.first.c_str()], obs.second, doc);
            }
    }

    // Dumping to string
    rapidjson::StringBuffer buffer;
    InfNaNWriter writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

#define WRITE_KEY_VALUE(key, value) writer.Key(key); writer.Double(value);

std::string serialize(const YamlState& d_dt)
{
    rapidjson::StringBuffer s;
    InfNaNWriter writer(s);
    writer.StartArray();
    WRITE_KEY_VALUE("t", d_dt.t);
    WRITE_KEY_VALUE("dx_dt", d_dt.x);
    WRITE_KEY_VALUE("dy_dt", d_dt.y);
    WRITE_KEY_VALUE("dz_dt", d_dt.z);
    WRITE_KEY_VALUE("du_dt", d_dt.u);
    WRITE_KEY_VALUE("dv_dt", d_dt.v);
    WRITE_KEY_VALUE("dw_dt", d_dt.w);
    WRITE_KEY_VALUE("dp_dt", d_dt.p);
    WRITE_KEY_VALUE("dq_dt", d_dt.q);
    WRITE_KEY_VALUE("dr_dt", d_dt.r);
    WRITE_KEY_VALUE("dqr_dt", d_dt.qr);
    WRITE_KEY_VALUE("dqi_dt", d_dt.qi);
    WRITE_KEY_VALUE("dqj_dt", d_dt.qj);
    WRITE_KEY_VALUE("dqk_dt", d_dt.qk);
    WRITE_KEY_VALUE("dphi_dt", d_dt.phi);
    WRITE_KEY_VALUE("dtheta_dt", d_dt.theta);
    WRITE_KEY_VALUE("dpsi_dt", d_dt.psi);
    writer.Key("extra_observations");
    write(writer, d_dt.extra_observations);
    writer.EndObject();
    return s.GetString();
}



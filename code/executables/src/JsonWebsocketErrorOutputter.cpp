#include <boost/algorithm/string/replace.hpp>

#include "JsonWebsocketErrorOutputter.hpp"

std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

JsonWebsocketErrorOutputter::JsonWebsocketErrorOutputter(const ssc::websocket::Message& msg_) : msg(msg_)
{
}

void JsonWebsocketErrorOutputter::output() const
{
    msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + get_message() + "\"}"));
}
#ifndef PARSE_ADDRESS_HPP_
#define PARSE_ADDRESS_HPP_

#include <string>
#include "YamlOutput.hpp"

struct WebsocketURLElements
{
    WebsocketURLElements(const std::string url);
    std::string protocol;
    std::string domain;
    int port;
    WebsocketURLElements() = delete;
};

bool is_a_websocket_url(const std::string& url);
YamlOutput build_YamlOutput_from_WS_URL(const std::string& address);

#endif

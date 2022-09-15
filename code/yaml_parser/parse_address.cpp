#include "parse_address.hpp"
#include <cstdlib> // std::atoi
#include <iostream>
#include <boost/regex.hpp>
#include "xdyn/exceptions/InvalidInputException.hpp"

boost::regex websocket_url_regex();
boost::regex websocket_url_regex()
{
    return boost::regex("(ws|wss)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
}

bool is_a_websocket_url(const std::string& url)
{
    boost::regex ex = websocket_url_regex();
    boost::cmatch what;
    return regex_match(url.c_str(), what, ex);
}

WebsocketURLElements::WebsocketURLElements(const std::string url) :
    protocol(),
    domain(),
    port()
{
    const boost::regex ex = websocket_url_regex();
    boost::cmatch what;
    if(regex_match(url.c_str(), what, ex))
    {
        protocol = std::string(what[1].first, what[1].second);
        domain = std::string(what[2].first, what[2].second);
        port = std::atoi(std::string(what[3].first, what[3].second).c_str());
        if ( port <= 0 || port > 65535 )
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Port: " << port << " is not valid. It should be between 0 and 65535");
        }
        if (protocol!="ws")
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Only 'ws' is valid. wss is not implemented yet");
        }
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "Address: " << url << " is not valid. Here are some examples"<<std::endl
                           << "  ws://localhost:8080"<<std::endl
                           << "  ws://130.66.124.200:8080");
    }
}

/**
 * \brief Create a YamlOutput instance from a websocket URL
 * \param url String representing the URL
 * \return YamlOutput
 * \note Use Boost Regex to parse URL
 *
 * If we want to parse more complex URL, like this one
 * "https://John:Dow@github.com:80/corporateshark/LUrlParser/?&query=ssl#q=frag";
 *
 * we may want to use something like LUrlParser
 * https://github.com/corporateshark/LUrlParser
 * http://130.66.124.6/svn/tools/ThirdParty/LUrlParser
 * \code
 *
 * \endcode
 */
YamlOutput build_YamlOutput_from_WS_URL(const std::string& url)
{
    YamlOutput out;
    const WebsocketURLElements url_elements(url);
    out.port = (short unsigned int)url_elements.port;
    out.format = "ws";
    out.address = url_elements.protocol+"://"+url_elements.domain;
    return out;
}

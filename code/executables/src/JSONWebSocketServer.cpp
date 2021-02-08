#include <iostream>
#include <chrono>
#include <thread>
// For handling Ctrl+C
#include <unistd.h>
#include <cstdio>
#include <csignal>
#include <memory>
#include <boost/algorithm/string/replace.hpp>

#include "report_xdyn_exceptions_to_user.hpp"
#include "JSONSerializer.hpp"

volatile sig_atomic_t stop;

#include "JSONWebSocketServer.hpp"

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234

void inthand(int);
void inthand(int)
{
    stop = 1;
}

template<class ServiceT>
JSONWebSocketServer<ServiceT>::JSONWebSocketServer(const ServiceT& service, const bool verbose):
        handler(service, verbose)
{
}

template<class ServiceT>
JSONWebSocketServer<ServiceT>::~JSONWebSocketServer()
{
}

template<class ServiceT>
void JSONWebSocketServer<ServiceT>::start(const short unsigned int port, const bool debug)
{
    std::cout << "Starting websocket server on " << ADDRESS << ":" << port << " (press Ctrl+C to terminate)" << std::endl;
    std::shared_ptr<ssc::websocket::Server> w(new ssc::websocket::Server(handler, port, debug));
    signal(SIGINT, inthand);
    while(!stop){std::this_thread::sleep_for(std::chrono::milliseconds(500));}
    std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
}

std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

template<class ServiceT>
JSONWebSocketServer<ServiceT>::JSONHandler::JSONHandler(const ServiceT& simserver, const bool verbose_):
        sim_server(simserver),
        verbose(verbose_)
{
}

template<class ServiceT>
JSONWebSocketServer<ServiceT>::JSONHandler::~JSONHandler()
{
}

template<class ServiceT>
void JSONWebSocketServer<ServiceT>::JSONHandler::operator()(const ssc::websocket::Message& msg)
{
    const std::string input_json = msg.get_payload();
    if (verbose)
    {
        std::cout << current_date_time() << " Received: " << input_json << std::endl;
    }
    const std::function<void(const std::string&)> quiet_error_outputter = [&msg](const std::string& what) {msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const std::function<void(const std::string&)> verbose_error_outputter = [&msg](const std::string& what) {std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
    const auto error_outputter = verbose ? verbose_error_outputter : quiet_error_outputter;
    const auto f = [&input_json, this, &msg]()
        {
            SimServerInputs server_inputs(deserialize(input_json), sim_server.get_Tmax());
            const std::string output_json = serialize(sim_server.handle(server_inputs));
            if (verbose)
            {
                std::cout << current_date_time() << " Sending: " << output_json << std::endl;
            }
            msg.send_text(output_json);
        };
    report_xdyn_exceptions_to_user(f, error_outputter);
}

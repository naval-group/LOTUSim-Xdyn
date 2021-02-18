#ifndef EXECUTABLES_INC_JSONWEBSOCKETSERVER_HPP_
#define EXECUTABLES_INC_JSONWEBSOCKETSERVER_HPP_

#include <iostream>
#include <chrono>
#include <thread>
// For handling Ctrl+C
#include <unistd.h>
#include <cstdio>
#include <csignal>
#include <memory>
#include <boost/algorithm/string/replace.hpp>

#include <ssc/websocket.hpp>
#include <ssc/macros.hpp>

#include "report_xdyn_exceptions_to_user.hpp"
#include "JSONSerializer.hpp"

volatile sig_atomic_t stop;

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234

template<class ServiceT>
class JSONWebSocketServer
{
    public:
        JSONWebSocketServer(const ServiceT& service, const bool verbose) : handler(service, verbose)
        {
        }
        virtual ~JSONWebSocketServer()
        {
        }

        void start(const short unsigned int port, const bool debug)
        {
            std::cout << "Starting websocket server on " << ADDRESS << ":" << port << " (press Ctrl+C to terminate)" << std::endl;
            std::shared_ptr<ssc::websocket::Server> w(new ssc::websocket::Server(handler, port, debug));
            signal(SIGINT, stop_simulation);
            while(!stop)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
        }

    private:
        static void stop_simulation(int)
        {
            stop = 1;
        }

        class JSONHandler : public ssc::websocket::MessageHandler
        {
            public:
                JSONHandler(const ServiceT& simserver, const bool verbose_) : sim_server(simserver), verbose(verbose_)
                {
                }
                virtual ~JSONHandler()
                {
                }

                void operator()(const ssc::websocket::Message& msg)
                {
                    const std::string input_json = msg.get_payload();
                    if(verbose)
                    {
                        std::cout << current_date_time() << " Received: " << input_json << std::endl;
                    }
                    const std::function<void(const std::string&)> quiet_error_outputter = [&msg, this](const std::string& what)
                    {   msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
                    const std::function<void(const std::string&)> verbose_error_outputter = [&msg, this](
                            const std::string& what)
                            {   std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
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

            private:
                std::string replace_newlines_by_spaces(std::string str)
                {
                    boost::replace_all(str, "\n", " ");
                    return str;
                }

                ServiceT sim_server;
                const bool verbose;
        };

        JSONHandler handler;
};

#endif /* EXECUTABLES_INC_JSONWEBSOCKETSERVER_HPP_ */

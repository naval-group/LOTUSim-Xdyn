#include <iostream>
#include <chrono>
#include <thread>
// For handling Ctrl+C
#include <unistd.h>
#include <cstdio>
#include <csignal>

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

JSONWebSocketServer::JSONWebSocketServer(const std::shared_ptr<ssc::websocket::MessageHandler> handler_):
        handler(handler_)
{
}

JSONWebSocketServer::~JSONWebSocketServer()
{
}

void JSONWebSocketServer::start(const short unsigned int port, const bool debug)
{
    std::cout << "Starting websocket server on " << ADDRESS << ":" << port << " (press Ctrl+C to terminate)" << std::endl;
    std::shared_ptr<ssc::websocket::Server> w(new ssc::websocket::Server(*handler, port, debug));
    signal(SIGINT, inthand);
    while(!stop){std::this_thread::sleep_for(std::chrono::milliseconds(500));}
    std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
}

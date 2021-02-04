#ifndef OBSERVERS_AND_API_INC_JSONWEBSOCKETSERVER_HPP_
#define OBSERVERS_AND_API_INC_JSONWEBSOCKETSERVER_HPP_

#include <memory>

#include <ssc/websocket/WebSocketServer.hpp>

class JSONWebSocketServer
{
public:
    JSONWebSocketServer(const std::shared_ptr<ssc::websocket::MessageHandler> handler);
    virtual ~JSONWebSocketServer();

    void start(const short unsigned int port, const bool debug);

private:
    std::shared_ptr<ssc::websocket::MessageHandler> handler;
};

#endif /* OBSERVERS_AND_API_INC_JSONWEBSOCKETSERVER_HPP_ */

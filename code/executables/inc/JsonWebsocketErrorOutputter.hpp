#ifndef JSONWEBSOCKETERROROUTPUTTERHPP
#define JSONWEBSOCKETERROROUTPUTTERHPP

#include <ssc/websocket.hpp>
#include "ErrorOutputter.hpp"

class JsonWebsocketErrorOutputter : public ErrorOutputter
{
    public:
        JsonWebsocketErrorOutputter(const ssc::websocket::Message& msg);
    private:
        JsonWebsocketErrorOutputter() = delete;
        void output() const;
        ssc::websocket::Message msg;
};

#endif // GRPCERROROUTPUTTERHPP
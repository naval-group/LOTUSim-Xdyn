#ifndef JSONWEBSOCKETERROROUTPUTTERHPP
#define JSONWEBSOCKETERROROUTPUTTERHPP

#include "ErrorOutputter.hpp"

class JsonWebsocketErrorOutputter : public ErrorOutputter
{
    private:
        void output() const;
};

#endif // GRPCERROROUTPUTTERHPP
#ifndef GRPCERROROUTPUTTERHPP
#define GRPCERROROUTPUTTERHPP

#include "ErrorOutputter.hpp"

class gRPCErrorOutputter : public ErrorOutputter
{
    private:
        void output() const;
};

#endif // GRPCERROROUTPUTTERHPP
#ifndef GRPCCONTROLLERINTERFACEHPP
#define GRPCCONTROLLERINTERFACEHPP

#include <memory>

class GrpcControllerInterface
{
    public:
        struct Input
        {
            Input ();
            std::string url;  //!< URL at which the gRPC controller may be reached,
                              //!< e.g. pid:9002
            std::string name; //!< Name used to disambiguate commands created by
                              //!< the controller
            std::string yaml; //!< The whole YAML node as a string, passed to the
                              //!< controller's set_parameters gRPC method
        };

};



#endif
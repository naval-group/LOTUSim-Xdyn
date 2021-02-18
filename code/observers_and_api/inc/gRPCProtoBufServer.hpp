#ifndef OBSERVERS_AND_API_INC_GRPCPROTOBUFSERVER_HPP_
#define OBSERVERS_AND_API_INC_GRPCPROTOBUFSERVER_HPP_

#include <memory>
#include <grpcpp/impl/codegen/service_type.h>

class gRPCProtoBufServer
{
public:
    gRPCProtoBufServer(const std::shared_ptr<grpc::Service> handler);
    virtual ~gRPCProtoBufServer();

    void start(const short unsigned int port);

private:
    std::shared_ptr<grpc::Service> handler;
};

#endif /* OBSERVERS_AND_API_INC_GRPCPROTOBUFSERVER_HPP_ */

#include "gRPCProtoBufServer.hpp"
#include <grpcpp/grpcpp.h>
#include <sstream>

gRPCProtoBufServer::gRPCProtoBufServer(const std::shared_ptr<grpc::Service> handler_):
        handler(handler_)
{
}

gRPCProtoBufServer::~gRPCProtoBufServer()
{
}

void gRPCProtoBufServer::start(const short unsigned int port)
{
    std::stringstream ss;
    ss << "0.0.0.0:" << port;
    const std::string server_address = ss.str();
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(handler.get());
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "gRPC server listening on " << server_address << std::endl;
    server->Wait();
    std::cout << std::endl << "Gracefully stopping the gRPC server..." << std::endl;
}

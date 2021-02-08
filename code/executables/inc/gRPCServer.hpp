#include <memory>

#include "gRPCProtoBufServer.hpp"
#include "CosimulationServiceImpl.hpp"
#include "ModelExchangeServiceImpl.hpp"

template<class SimServerT>
class gRPCServer
{};

template<>
class gRPCServer<XdynForCS> : public gRPCProtoBufServer
{
public:
    gRPCServer(const XdynForCS& simserver):
            gRPCProtoBufServer(std::make_shared<CosimulationServiceImpl>(simserver)){}
};

template<>
class gRPCServer<XdynForME> : public gRPCProtoBufServer
{
public:
    gRPCServer(const XdynForME& simserver):
            gRPCProtoBufServer(std::make_shared<ModelExchangeServiceImpl>(simserver)){}
};

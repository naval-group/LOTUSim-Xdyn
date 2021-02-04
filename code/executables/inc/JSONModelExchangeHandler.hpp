#ifndef EXECUTABLES_INC_JSONMODELEXCHANGEHANDLER_HPP_
#define EXECUTABLES_INC_JSONMODELEXCHANGEHANDLER_HPP_

#include <ssc/websocket.hpp>
#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "XdynForME.hpp"

class JSONModelExchangeHandler : public ssc::websocket::MessageHandler
{
public:
    JSONModelExchangeHandler(const TR1(shared_ptr)<XdynForME>& simserver, const bool verbose);
    virtual ~JSONModelExchangeHandler();

    void operator()(const ssc::websocket::Message& msg);

private:
    TR1(shared_ptr)<XdynForME> sim_server;
    const bool verbose;
};

#endif /* EXECUTABLES_INC_JSONMODELEXCHANGEHANDLER_HPP_ */

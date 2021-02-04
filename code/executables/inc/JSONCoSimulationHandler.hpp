#ifndef EXECUTABLES_INC_JSONCOSIMULATIONHANDLER_HPP_
#define EXECUTABLES_INC_JSONCOSIMULATIONHANDLER_HPP_

#include <ssc/websocket.hpp>
#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "XdynForCS.hpp"

class JSONCoSimulationHandler : public ssc::websocket::MessageHandler
{
public:
    JSONCoSimulationHandler(const TR1(shared_ptr)<XdynForCS>& simserver, const bool verbose);
    virtual ~JSONCoSimulationHandler();

    void operator()(const ssc::websocket::Message& msg);

private:
    TR1(shared_ptr)<XdynForCS> sim_server;
    const bool verbose;
};

#endif /* EXECUTABLES_INC_JSONCOSIMULATIONHANDLER_HPP_ */

/*
 * ObserverTests.cpp
 *
 *  Created on: May 6, 2015
 *      Author: cady
 */

#include "ObserverTests.hpp"
#include "ListOfObservers.hpp"
#include "MapObserver.hpp"
#include "xdyn/core/Sim.hpp"
#include "xdyn/observers_and_api/simulator_api.hpp"
#include "xdyn/test_data_generator/yaml_data.hpp"
#include "xdyn/test_data_generator/stl_data.hpp"
#include "xdyn/test_data_generator/TriMeshTestData.hpp"
#include "xdyn/yaml_parser/parse_output.hpp"

#include <ssc/solver/steppers.hpp>
#include <ssc/websocket.hpp>

#include <unistd.h> // usleep

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234

struct ListOfStringMessages : public ssc::websocket::MessageHandler
{
    ListOfStringMessages() : messages()
    {
    }
    void operator()(const ssc::websocket::Message& msg)
    {
        messages.push_back(msg.get_payload());
    }

    std::vector<std::string> messages;
};

ObserverTests::ObserverTests() : a(ssc::random_data_generator::DataGenerator(542021))
{
}

ObserverTests::~ObserverTests()
{
}

void ObserverTests::SetUp()
{
}

void ObserverTests::TearDown()
{
}

TEST_F(ObserverTests, can_observe_using_a_websocket)
{
//! [ObserverTests example]
    ListOfStringMessages handler;
    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, WEBSOCKET_PORT));
    {
        const auto yaml = test_data::oscillating_cube_example();
        const auto mesh = test_data::cube();
        Sim sys = get_system(yaml, mesh, 0);
        YamlOutput out;
        out.address = WEBSOCKET_ADDRESS;
        out.port = WEBSOCKET_PORT;
        out.data = {"t", "x(cube)", "theta(cube)"};
        out.format = "ws";
        std::vector<YamlOutput> v(1,out);
        ssc::solver::Scheduler scheduler(0, 1, 0.1);
        ListOfObservers observer(v);
        ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, scheduler, observer);
        usleep(1000); // So the server thread has enough time to process the data
    }
//! [ObserverTests example]
//! [ObserverTests expected output]
    ASSERT_EQ(11, handler.messages.size());
    ASSERT_EQ("{\"t\":0,\"states\":{\"cube\":{\"theta\":-0,\"x\":0}}}", handler.messages.front());
//! [ObserverTests expected output]
}

TEST_F(ObserverTests, can_output_all_results)
{
    // This is to test Observer
    const auto yaml = test_data::oscillating_cube_example();
    const auto mesh = test_data::cube();
    Sim sys = get_system(yaml, mesh, 0);
    MapObserver observer = MapObserver(); // Should output all results when using default constructor
    ssc::solver::Scheduler scheduler(0, 1, 0.1);
    ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, scheduler, observer);
    auto results = observer.get();
    ASSERT_TRUE(results.find("u(cube)")!= results.end());
    ASSERT_TRUE(results.find("v(cube)")!= results.end());
    ASSERT_TRUE(results.find("w(cube)")!= results.end());
    ASSERT_TRUE(results.find("p(cube)")!= results.end());
    ASSERT_TRUE(results.find("q(cube)")!= results.end());
    ASSERT_TRUE(results.find("r(cube)")!= results.end());
    ASSERT_TRUE(results.find("x(cube)")!= results.end());
    ASSERT_TRUE(results.find("y(cube)")!= results.end());
    ASSERT_TRUE(results.find("z(cube)")!= results.end());
    ASSERT_TRUE(results.find("qr(cube)")!= results.end());
    ASSERT_TRUE(results.find("qi(cube)")!= results.end());
    ASSERT_TRUE(results.find("qj(cube)")!= results.end());
    ASSERT_TRUE(results.find("qk(cube)")!= results.end());
    ASSERT_TRUE(results.find("phi(cube)")!= results.end());
    ASSERT_TRUE(results.find("theta(cube)")!= results.end());
    ASSERT_TRUE(results.find("psi(cube)")!= results.end());
    ASSERT_TRUE(results.find("Fx(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fy(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fz(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Mx(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("My(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Mz(gravity,cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fx(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Fy(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Fz(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Mx(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("My(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Mz(gravity,cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Fx(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fy(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fz(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Mx(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("My(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Mz(non-linear hydrostatic (fast),cube,cube)")!= results.end());
    ASSERT_TRUE(results.find("Fx(non-linear hydrostatic (fast),cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Fy(non-linear hydrostatic (fast),cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Fz(non-linear hydrostatic (fast),cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Mx(non-linear hydrostatic (fast),cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("My(non-linear hydrostatic (fast),cube,NED)")!= results.end());
    ASSERT_TRUE(results.find("Mz(non-linear hydrostatic (fast),cube,NED)")!= results.end());
}

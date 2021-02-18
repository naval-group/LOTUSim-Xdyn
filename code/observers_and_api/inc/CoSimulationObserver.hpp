#ifndef OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_
#define OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_

#include <vector>

#include "SimulationServerObserver.hpp"

class CoSimulationObserver : public SimulationServerObserver
{
public:
    CoSimulationObserver(const std::vector<std::string>& extra_fields_to_serialize, const std::string& body_name);
    virtual ~CoSimulationObserver() = default;

private:
    using Observer::get_serializer;
    using Observer::get_initializer;
    std::function<void()> get_serializer(const double val, const DataAddressing& address) override;

    const std::string body_name;
};

#endif /* OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_ */

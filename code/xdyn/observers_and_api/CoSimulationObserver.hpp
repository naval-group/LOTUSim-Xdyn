#ifndef OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_
#define OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_

#include "SimulationServerObserver.hpp"
#include <string>
#include <vector>

class CoSimulationObserver : public SimulationServerObserver
{
public:
    CoSimulationObserver(const std::vector<std::string>& extra_fields_to_serialize, const std::string& body_name);
    virtual ~CoSimulationObserver() = default;

private:
    using Observer::get_serializer;
    std::function<void()> get_serializer(const double val, const DataAddressing& address) override;

    const std::string body_name;
};

#endif /* OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_ */

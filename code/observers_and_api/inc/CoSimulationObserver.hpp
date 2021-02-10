#ifndef OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_
#define OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_

#include <vector>

#include "Observer.hpp"
#include "YamlState.hpp"

class CoSimulationObserver : public Observer
{
public:
    CoSimulationObserver(const std::vector<std::string>& optional_data, const std::string& body_name);
    virtual ~CoSimulationObserver();

    std::vector<YamlState> get() const;

private:
    using Observer::get_serializer;
    using Observer::get_initializer;
    std::function<void()> get_serializer(const double val, const DataAddressing& address) override;
    std::function<void()> get_initializer(const double val, const DataAddressing& address) override;

    void flush_after_initialization() override;
    void flush_after_write() override;
    void flush_value_during_write() override;

    YamlState current_state;
    std::vector<YamlState> states;
    const std::string body_name;
};

#endif /* OBSERVERS_AND_API_COSIMULATIONOBSERVER_HPP_ */

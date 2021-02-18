#include "SimulationServerObserver.hpp"

SimulationServerObserver::SimulationServerObserver(const std::vector<std::string>& extra_fields_to_serialize):
    Observer(extra_fields_to_serialize),
    current_state(),
    states()
{
}

std::vector<YamlState> SimulationServerObserver::get() const
{
    return states;
}

std::function<void()> SimulationServerObserver::get_serializer(const double val, const DataAddressing& address)
{
    return [this, val, address]()
    {
        current_state.extra_observations[address.name] = val;
    };
}

std::function<void()> SimulationServerObserver::get_initializer(const double, const DataAddressing&)
{
    return [](){};
}

void SimulationServerObserver::flush_after_initialization()
{
}

void SimulationServerObserver::flush_after_write()
{
    states.push_back(current_state);
    current_state = YamlState();
}

void SimulationServerObserver::flush_value_during_write()
{
}

#ifndef OBSERVERS_AND_API_INC_SIMULATIONSERVEROBSERVER_HPP_
#define OBSERVERS_AND_API_INC_SIMULATIONSERVEROBSERVER_HPP_

#include "xdyn/core/Observer.hpp"
#include "external_data_structures/YamlState.hpp"

class SimulationServerObserver : public Observer
{
    public:
        SimulationServerObserver(const std::vector<std::string>& extra_fields_to_serialize);
        virtual ~SimulationServerObserver() = default;

        std::vector<YamlState> get() const;

    protected:
        using Observer::get_serializer;
        using Observer::get_initializer;
        std::function<void()> get_serializer(const double val, const DataAddressing& address) override;

        YamlState current_state;
        std::vector<YamlState> states;

    private:
        std::function<void()> get_initializer(const double val, const DataAddressing& address) override;

        void flush_after_initialization() override;
        void flush_after_write() override;
        void flush_value_during_write() override;
};

#endif /* OBSERVERS_AND_API_INC_SIMULATIONSERVEROBSERVER_HPP_ */

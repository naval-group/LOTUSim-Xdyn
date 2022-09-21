#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include "xdyn/core/Observer.hpp"
#include "xdyn/environment_models/DiscreteDirectionalWaveSpectrum.hpp"
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include "H5Cpp.h"

struct Hdf5Addressing
{
    std::string address;
    Hdf5Addressing():address(){};
    Hdf5Addressing(
            const DataAddressing& addressing,
            const std::string& basename=""
            );
};

class Hdf5WaveObserver;
class Hdf5Observer : public Observer
{
    public:
        Hdf5Observer(const std::string& filename);
        Hdf5Observer(const std::string& filename, const std::vector<std::string>& data);
        void write_before_simulation(const MeshPtr mesh, const DataAddressing& address) override;
        void write_before_simulation(const std::string& data, const DataAddressing& address) override;
        void write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& val, const DataAddressing& address) override;
        void write_command_line_before_simulation(const std::string& command_line) override;
        void write_yaml_before_simulation(const std::string& yaml) override;
        void write_matlab_script_before_simulation() override;
        void write_python_script_before_simulation() override;
    private:
        void flush_after_initialization() override;
        void flush_after_write() override;
        void flush_value_during_write() override;

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address) override;
        std::function<void()> get_initializer(const double val, const DataAddressing& address) override;

        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address) override;
        std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address) override;

        H5::H5File h5File;
        std::string basename;
        std::map<std::string, std::string > name2address;
        std::map<std::string, H5::DataSet> name2dataset;
        std::map<std::string, H5::DataType> name2datatype;
        std::map<std::string, H5::DataSpace> name2dataspace;
        TR1(shared_ptr)<Hdf5WaveObserver> wave_serializer;
        std::string filename;
};

#endif
